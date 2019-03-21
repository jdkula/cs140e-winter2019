//
// Created by jonak on 3/9/19.
//

#include <gpio.h>
#include <timer.h>
#include <printf.h>
#include <uart.h>
#include <softuart.h>
#include "xbee.h"

void xbee_init(uint8_t reset_pin) {
    gpio_init();

    gpio_set_pullup(reset_pin);
}

static bool xbee_enter_at_command_mode() {
    delay_ms(1000);
    printk("+++");
    delay_ms(1000);
    return false;
    delay_ms(2000);
    printk("+++");

    soft_uart_puts("Sent +++, about to get O...\r\n");
    char o = uart_getc();
    soft_uart_puts("Got o...\r\n");
    char k = uart_getc();
    soft_uart_puts("Got k...\r\n");
    char r = uart_getc();
    soft_uart_puts("Got return.\r\n");

    return o == 'O' && k == 'K' && r == '\r';
}

void xbee_line_flush() {
    while(uart_getc() != '\r') ;
}

uint32_t read_hex_integer() {
    unsigned char read[8];
    int i = 0;
    while (i < 8 && (read[i] = uart_getc()) != '\r') {
        soft_uart_putc(read[i]);
        if(read[i] == 'R') {  // from ERROR
            xbee_line_flush();
            return 0;
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

static const char* command_prefix = "AT";
static const char* command_suffix = "\r";

static void send_read_command(const char* command) {
    char buff[64];
    snprintk(buff, 64, "About to send %s\r\n", command);
    soft_uart_puts(buff);
    printk("%s%s%s", command_prefix, command, command_suffix);
}

static bool send_write_command(const char* command, uint32_t argument) {
    printk("%s%s%x%s", command_prefix, command, argument, command_suffix);
    char result = uart_getc();
    xbee_line_flush();
    return result == 'O'; // "OK" (vs ERROR)
}

struct xbee_addresses xbee_get_address_info() {
    struct xbee_addresses info = { 0 };

    xbee_enter_at_command_mode();

    send_read_command("DH");
    info.destination_high = read_hex_integer();

    send_read_command("DL");
    info.destination_low = read_hex_integer();

    send_read_command("MY");
    info.network_address = read_hex_integer();

    send_read_command("MP");
    info.parent_network_address = read_hex_integer();

    send_read_command("NC");
    info.remaining_children = read_hex_integer();

    send_read_command("SH");
    info.serial_num_high = read_hex_integer();

    send_read_command("SL");
    info.serial_num_low = read_hex_integer();

    send_read_command("NI");
    for(int i = 0; i < 20; i++) {
        char c = uart_getc();
        info.node_identifier[i] = c;
        if(c == '\r') {
            info.node_identifier[i] = '\0';
            break;
        }
    }

    send_read_command("SE");
    info.source_endpoint = read_hex_integer();

    send_read_command("DE");
    info.destination_endpoint = read_hex_integer();

    send_read_command("CI");
    info.cluster_identifier = read_hex_integer();

    send_read_command("TO");
    info.transmit_options.packed = read_hex_integer();

    send_read_command("NP");
    info.max_rf_payload_bytes = read_hex_integer();

    send_read_command("DD");
    info.device_type_identifier = read_hex_integer();

    send_read_command("CR");
    info.conflict_report = read_hex_integer();

    send_read_command("CN"); // exit

    return info;
}

void xbee_send_address_info(struct xbee_addresses info) {
    xbee_enter_at_command_mode();

    send_write_command("DH", info.destination_high);
    send_write_command("DL", info.destination_low);

    printk("%sNI%s%s", command_prefix, info.node_identifier, command_suffix);

    send_write_command("SE", info.source_endpoint);
    send_write_command("DE", info.destination_endpoint);
    send_write_command("CI", info.cluster_identifier);
    send_write_command("TO", info.transmit_options.packed);
    send_write_command("DD", info.device_type_identifier);
    send_write_command("CR", info.conflict_report);
}

void xbee_commit() {
    xbee_enter_at_command_mode();

    send_read_command("WR");
}