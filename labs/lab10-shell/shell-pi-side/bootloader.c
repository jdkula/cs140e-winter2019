/* simplified bootloader.  */
#include "rpi.h"
#include "pi-shell.h"

#define __SIMPLE_IMPL__
#include "../bootloader/shared-code/simple-boot.h"

static void send_byte(unsigned char uc) {
	uart_putc(uc);
}
static unsigned char get_byte(void) { 
        return uart_getc();
}

unsigned get_uint(void) {
	unsigned u = get_byte();
        u |= get_byte() << 8;
        u |= get_byte() << 16;
        u |= get_byte() << 24;
	return u;
}
void put_uint(unsigned u) {
        send_byte((u >> 0)  & 0xff);
        send_byte((u >> 8)  & 0xff);
        send_byte((u >> 16) & 0xff);
        send_byte((u >> 24) & 0xff);
}

static void die(unsigned err) {
	put_uint(err);
	delay_ms(100); 	// let the output queue drain.
	rpi_reboot();
}

// load_code:
//	1. figure out if the requested address range is available.
//	2. copy code to that region.
//	3. return address of first executable instruction: note we have
//	a 8-byte header!  (see ../hello-fixed/memmap)
int load_code(void) {
	unsigned addr=0;

	// let unix know we are ready.
	put_uint(ACK);
	
	    uint32_t version = get_uint();
    if(version != 2) {
        die(NAK);
    }
    addr = get_uint();

    uint32_t numBytes = get_uint();       // Take in the number of bytes.
    uint32_t msgCrc = get_uint();         // Take in the message checksum.

    uint32_t nCrc = crc32(&numBytes, 4);  // CRC the number of bytes...
    put_uint(nCrc);                     // ...and send it back to verify.
    put_uint(msgCrc);                   // ...also send back the CRC we were given.

    if (get_uint() != ACK) {             // If the UNIX side doesn't like what we sent, reboot.
        die(NAK);
    }

    put_uint(ACK);

    for(int i = 0; i < numBytes; i += 4) {
        unsigned data = get_uint();
        put32(addr + i, data);
    }
    
    if (get_uint() != EOT) die(TOO_BIG);

    if (crc32((void*) addr, numBytes) != msgCrc) die(BAD_CKSUM);


    put_uint(ACK);

        // give time to flush out; ugly.   implement `uart_flush()`
	delay_ms(100);  
	
	BRANCHTO(addr + 8); // +8 for version + address!

	/* return address */
}
