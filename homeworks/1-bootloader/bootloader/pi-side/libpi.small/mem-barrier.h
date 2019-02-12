#ifndef __LIBPI_MEM_BARRIER__
#define __LIBPI_MEM_BARRIER__

#define data_sync_barrier() dsb()
#define data_memory_barrier() dmb()

#endif // __LIBPI_MEM_BARRIER

void mb(void);
void dmb(void);
void dsb(void);
