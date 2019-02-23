#include "rpi.h"
#include "pi-shell.h"

// read characters until we hit a newline.
static int readline(char *buf, int sz) {
	for(int i = 0; i < sz; i++) {
		if((buf[i] = uart_getc()) == '\n') {
			buf[i] = 0;
			return i;
		}
	}
	// just return partial read?
	panic("size too small\n");
}

void notmain() { 
	uart_init();
    int n;
    char buf[1024];

    while ((n = readline(buf, sizeof buf))) {
        if (strncmp(buf, "echo ", 4) == 0) {
            printk("%s\n", buf);
        } else if (strncmp(buf, "PI REBOOT!!!", 12) == 0) {
            printk("PI REBOOT!!!\n");
            delay_ms(100);
            clean_reboot();
        } else if(strncmp(buf, "BOOT", 4) == 0) {
            load_code();
        } else {
            printk("Found unrecognized command! %s\n", buf);
        }
    }
	clean_reboot();
}
