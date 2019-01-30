ARCH=arm
PIOSPATH=$(HOME)/cs140e-winter2019/PiOS
LIBPATH=$(PIOSPATH)/out-$(ARCH)/lib

LPI_INC=$(PIOSPATH)/lib/libpi/include-common
LTYPE_INC=$(PIOSPATH)/lib/libtypes/include-common
LTYPE_ARCH=$(PIOSPATH)/lib/libtypes/include-$(ARCH)

LPI=$(LIBPATH)/libpi.a

LIBPREPROC=$(PIOSPATH)/lib/libpreprocessor/include-common

PIBOOT=$(PIOSPATH)/out-x86/bin/piboot

DEFINES=-Ddmb=data_memory_barrier -Ddsb=data_sync_barrier

CFLAGS =  -I$(LPI_INC) -I$(LTYPE_INC) -I$(LTYPE_ARCH) -I$(LIBPREPROC) -I. -Wall -Og -std=gnu99 -nostdlib -nostartfiles -ffreestanding $(FPFLAGS) $(DEFINES)
