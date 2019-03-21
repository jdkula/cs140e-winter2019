/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: xbee.c
 * --------------
 * Provides AT configuration commands for the XBee.
 */

#include <gpio.h>
#include <timer.h>
#include <printf.h>
#include <uart.h>
#include <softuart.h>
#include "xbee.h"

void xbee_init(uint8_t reset_pin) {
    gpio_init();

    gpio_set_pullup(reset_pin);  // XBee resets when the reset pin is pulled low. pg. 16
}

/** Enters AT Command mode by waiting the required guard times and sending the control sequence. */
static void xbee_enter_at_command_mode() {
    delay_ms(1000);  // Rest for the guard time (default 1s) -- XBee manual pg. 31
    printk("+++");   // Send 3x command sequence char (default +) -- pg. 31
    delay_ms(1000);  // Additional guard time
}

/** Flushes the remainder of a response line from the XBee, which ends in \r. */
void xbee_line_flush() {
    while(uart_getc() != '\r') ;
}

/** The XBees respond with an ASCII hex value; this parses that into an actual number. */
uint32_t read_hex_integer() {
    unsigned char read[8];
    int i = 0;

    // read the value
    while (i < 8 && (read[i] = uart_getc()) != '\r') {
        soft_uart_putc(read[i]);
        if(read[i] == 'R') {  // from ERROR, a possible value to receive.
            xbee_line_flush();
            return 0;
        } else if(read[i] == 'O') {  // from OK, a possible value to receive.
            xbee_line_flush();
            return 1;
        }

        if(read[i] >= 'A' && read[i] <= 'F') {
            read[i] -= 'A';
        } else {
            read[i] -= '0';
        }
        i++;
    }

    uint32_t final = 0;
    for(int loc = 0; loc < i; loc++) {
        uint32_t add = read[loc];
        for(int pow = (i - loc - 1); pow > 0; pow--) {
            add *= 16;
        }
        final += add;
    }

    return final;
}

static const char* command_prefix = "AT";  // pg 31
static const char* command_suffix = "\r";  // pg 31

static void send_command(const char* command) {
    printk("%s%s%s", command_prefix, command, command_suffix);
}

static void send_hex_command(const char* command, uint32_t argument) {
    printk("%s%s%x%s", command_prefix, command, argument, command_suffix);
}

struct xbee_addresses xbee_get_address_info() {
    struct xbee_addresses info = { 0 };

    xbee_enter_at_command_mode();

    // all commands available on XBee page 154

    send_command("DH");
    info.destination_high = read_hex_integer();

    send_command("DL");
    info.destination_low = read_hex_integer();

    send_command("MY");
    info.network_address = read_hex_integer();

    send_command("MP");
    info.parent_network_address = read_hex_integer();

    send_command("NC");
    info.remaining_children = read_hex_integer();

    send_command("SH");
    info.serial_num_high = read_hex_integer();

    send_command("SL");
    info.serial_num_low = read_hex_integer();

    send_command("NI"); // 20ch string.
    for(int i = 0; i < 21; i++) {
        char c = uart_getc();
        info.node_identifier[i] = c;
        if(c == '\r') {
            info.node_identifier[i] = '\0';
            break;
        }
    }

    send_command("SE");
    info.source_endpoint = read_hex_integer();

    send_command("DE");
    info.destination_endpoint = read_hex_integer();

    send_command("CI");
    info.cluster_identifier = read_hex_integer();

    send_command("TO");
    info.transmit_options.packed = read_hex_integer();

    send_command("NP");
    info.max_rf_payload_bytes = read_hex_integer();

    send_command("DD");
    info.device_type_identifier = read_hex_integer();

    send_command("CR");
    info.conflict_report = read_hex_integer();

    send_command("CN"); // exit

    return info;
}

/** Sets information that is writable from the given info struct. */
void xbee_send_address_info(struct xbee_addresses info) {
    xbee_enter_at_command_mode();

    send_hex_command("DH", info.destination_high);  // pages 154-157
    send_hex_command("DL", info.destination_low);

    printk("%sNI%s%s", command_prefix, info.node_identifier, command_suffix);

    send_hex_command("SE", info.source_endpoint);
    send_hex_command("DE", info.destination_endpoint);
    send_hex_command("CI", info.cluster_identifier);
    send_hex_command("TO", info.transmit_options.packed);
    send_hex_command("DD", info.device_type_identifier);
    send_hex_command("CR", info.conflict_report);

    send_command("CN"); // exit
}

/** Commits settings to non-volatile memory. Should be used sparingly -- there's a limited write cycle count. */
void xbee_commit() {
    xbee_enter_at_command_mode();

    send_command("WR"); // page 171
    xbee_line_flush();  // wait until write finishes.

    send_command("CN"); // exit
}