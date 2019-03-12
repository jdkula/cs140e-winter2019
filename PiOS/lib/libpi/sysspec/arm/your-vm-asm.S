@ Must carefully read B2 of the armv6 manual.  You cannot rely on "well it
@ worked on my test case": mistakes cause stale data, which may or may not
@ show up in your test case (likely not) despite being wildly broken.
@
@ Note: Rd is allegedly a read-only parameter to these instructions, but
@ the manual says SBZ (should be zero).  I think (and the linux source seems
@ to think) that this means we have to explicitly set it to 0.  Not setting
@ Rd=0 is an easy mistake to make.  Perhaps is worth building in (Linux does
@ not, but) --- the cost of an extra mov is negligible given our other 
@ overheads.
@
@ Alot of people put these instructions in inline assembly, wrapped up by 
@ function calls (e.g., cs107e's MMU code).  This practice is dangerous. 
@ For example, if you are flushing caches, you cannot be sure that the 
@ function return, etc does not then reload them, esp w.r.t. the BTB.  I 
@ think the only safe approach is to put directly in assembly so you are 
@ guaranteed no branch, indirect jump, load/store happens in between.
@
@ A lot of MMU code out there appears pretty broken b/c they don't do enough
@ flushing operations and/or rely on gcc code generation to not do the above.

@ used to clear register before CP15 operation.
#define CLR(reg) mov reg, #0 

@ used to be called "drain write buffer"
@ includes all cache operations.  is a superset of
@ > DMB
#define DSB(Rd)             mcr p15, 0, Rd, c7, c10, 4

#define DMB(Rd)             mcr p15, 0, Rd, c7, c10, 5 

@ must flush the prefetch buffer whenever you change a virtual 
@ mapping (ASID, PTE, etc) since it will have stale instructions.
@
@ if you are doing this, likely have to do a DSB before to make
@ sure whatever invalidation you did has completed.
#define ISB(Rd) PREFETCH_FLUSH(Rd)
#define PREFETCH_FLUSH(Rd)  mcr p15, 0, Rd, c7, c5, 4  

@ must do this after changing any MMU stuff, ASID, etc.
#define FLUSH_BTB(Rd)         mcr p15, 0, Rd, c7, c5, 6

@ need to figure out if have a unified or separate I/D cache/TLB

@ Work-around for bug in ARMv6 if we have seperate I/D.  Taken from:
@   https://elixir.bootlin.com/linux/latest/source/arch/arm/mm/cache-v6.S
@ MUST HAVE INTERRUPTS DISABLED!
@ XXX: patch feedback implies we need this for other operations too?
#define INV_ICACHE(Rd)                                           \
    mov Rd, #0                  ;                                   \
    mcr p15, 0, Rd, c7, c5, 0   ; /* invalidate entire I-cache */   \
    mcr p15, 0, Rd, c7, c5, 0;  ; /* invalidate entire I-cache */   \
    mcr p15, 0, Rd, c7, c5, 0;  ; /* invalidate entire I-cache */   \
    mcr p15, 0, Rd, c7, c5, 0;  ; /* invalidate entire I-cache */   \
    .rept   11                  ; /* ARM Ltd recommends at least 11 nops */\
    nop                         ;                                   \
    .endr                     

#define INV_DCACHE(Rd)      mcr p15, 0, Rd, c7, c6, 0  
#define INV_ALL_CACHES(Rd)  mcr p15, 0, Rd, c7, c7, 0  

@ invalidate TLB entries.
#define INV_ITLB(Rd)        mcr p15, 0, Rd, c8, c5, 0 
#define INV_DTLB(Rd)        mcr p15, 0, Rd, c8, c6, 0 
@ invalidate unified TLB or both I/D TLB
#define INV_TLB(Rd)         mcr p15, 0, Rd, c8, c7, 0

#define SET_ASID(Rd)   mcr p15, 0, Rd, c13, c0, 1
#define GET_ASID(Rd)   mrc p15, 0, Rd, c13, c0, 1
#define SET_TTBR0(Rd)  mcr p15, 0, Rd, c2, c0, 0
#define GET_TTBR0(Rd)  mrc p15, 0, Rd, c2, c0, 0
#define SET_TTBR1(Rd)  mcr p15, 0, Rd, c2, c0, 1
#define GET_TTBR1(Rd)  mrc p15, 0, Rd, c2, c0, 1
#define SET_TTBRC(Rd)  mcr p15, 0, Rd, c2, c0, 2
#define GET_TTBRC(Rd)  mrc p15, 0, Rd, c2, c0, 2

.globl get_cache_type
get_cache_type:
    mrc p15, 0, r0, c0, c0, 1
    bx lr

.globl write_control_reg1
write_control_reg1:
    mcr p15, 0, r0, c1, c0, 0
    CLR(r0)
    DSB(r0)
    bx lr

.globl read_control_reg1
read_control_reg1:
    CLR(r0)
    mrc p15, 0, r0, c1, c0, 0
    bx lr

.globl tlb_get_procid
tlb_get_procid:
    CLR(r0)
    GET_ASID(r0)
    bx lr

.globl read_tlb_base_ctrl
read_tlb_base_ctrl:
    CLR(r0)
    GET_TTBRC(r0)  ;@ B3-7 - TLB type register
    bx lr

.globl read_tlb_config
read_tlb_config:
    CLR(r0)
    mrc p15, 0, r0, c0, c0, 0b11  ;@ B3-7 - TLB type register
    bx lr

.globl read_ttbr0
read_ttbr0:
    CLR(r0)
    GET_TTBR0(r0)
    bx lr

.globl read_ttbr1
read_ttbr1:
    CLR(r0)
    GET_TTBR1(r0)
    bx lr

.globl read_domain_access_ctrl
read_domain_access_ctrl:
    CLR(r0)
    mrc p15, 0, r0, c3, c0, 0    ;@ B4-42 - register 3. Store to r0.
    bx lr

.globl write_domain_access_ctrl
write_domain_access_ctrl:
    CLR(r1)                    ;@ B6-21 - data SBZ
    mcr p15, 0, r0, c3, c0, 0  ;@ B4-42 - register 3. r0 has our value.
    CLR(r2)
    DSB(r2)                    ;@ Various examples seem to have this, so just in case...
    PREFETCH_FLUSH(r1)         ;@ B2-24 - CP15 changes need a prefetch flush.
    bx lr


.globl set_procid_ttbr0
set_procid_ttbr0:
    CLR(r2)
    SET_ASID(r2)          ;@= "cookbook" from B2-25 under "Example Solution"
    PREFETCH_FLUSH(r2)
    SET_TTBR1(r2)         ;@ We're not using TTBR1.
    SET_TTBRC(r2)         ;@ B4-41 - have N = 0 to only use TTBR0.
    SET_TTBR0(r1)
    DSB(r2)               ;@ Various examples seem to have this, so just in case...
    PREFETCH_FLUSH(r2)
    SET_ASID(r0)          ;@= End cookbook
    FLUSH_BTB(r2)         ;@ B2-24 - Flush BTB after changing TTBR*
    DSB(r2)               ;@ Various examples seem to have this, so just in case...
    PREFETCH_FLUSH(r2)    ;@ B2-24 - Flush prefetch after BTB
    bx lr

.globl enable_mmu
enable_mmu:
    CLR(r2)
    INV_TLB(r2)         ;@
    INV_ICACHE(r2)      ;@ B2-20 (cache management sequence)
    INV_DCACHE(r2)      ;@ note: "drain write buffer" == "data sync barrier" B6-22
    PREFETCH_FLUSH(r2)  ;@ B2-22 (TLB operations observed after flush)
    DSB(r2)             ;@ B2-22 (DSB required after TLB operations)

    orr r0, r0, #1             ;@ B3-14 (set M  = 1; write 1 to byte 0)
    mcr p15, 0, r0, c1, c0, 0  ;@ B3-14 (write new processor values)

    CLR(r2)
    FLUSH_BTB(r2)       ;@ B2-24 (should flush after MMU enable)
    DSB(r2)             ;@ B2-22 (DSB required after BTB flush)
    PREFETCH_FLUSH(r2)  ;@ B2-24 (prefetch flush should follow BTB flush)

    bx lr

.globl disable_mmu
disable_mmu:
    CLR(r2)
    INV_TLB(r2)         ;@
    INV_ICACHE(r2)      ;@ B2-20 (cache management sequence)
    INV_DCACHE(r2)      ;@ note: "drain write buffer" == "data sync barrier" B6-22
    PREFETCH_FLUSH(r2)  ;@ B2-22 (TLB operations observed after flush)
    DSB(r2)             ;@ B2-22 (DSB required after TLB operations)

    and r0, r0, #0xFFFFFF8     ;@ B3-14 and 6-9 (set M  = 0; write 0 to bytes 2-0)
    mcr p15, 0, r0, c1, c0, 0  ;@ B3-14 (write new processor values)

    CLR(r2)
    FLUSH_BTB(r2)       ;@ B2-24 (should flush after MMU enable)
    DSB(r2)             ;@ B2-22 (DSB required after BTB flush)
    PREFETCH_FLUSH(r2)  ;@ B2-24 (prefetch flush should follow BTB flush)

    bx lr
