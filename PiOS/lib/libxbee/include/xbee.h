//
// Created by jonak on 3/9/19.
//

#ifndef PIOS_XBEE_H
#define PIOS_XBEE_H


union transmit_options {
    uint8_t packed;

    struct transmit_options_unpacked {
        bool disable_retries:1;
        bool _none_a:4;
        bool enable_aps_encrypt:1;
        bool use_extended_timeout:1;
        bool _none_b:1;
    } unpacked;
};

struct xbee_addresses {
    uint32_t destination_high;
    uint32_t destination_low;
    uint16_t network_address;
    uint16_t parent_network_address;
    uint32_t remaining_children;
    uint32_t serial_num_high;
    uint32_t serial_num_low;
    char node_identifier[21];
    uint8_t source_endpoint;
    uint8_t destination_endpoint;
    uint16_t cluster_identifier;
    union transmit_options transmit_options;
    uint16_t max_rf_payload_bytes;
    uint32_t device_type_identifier;
    uint8_t conflict_report;
};

void xbee_init(uint8_t reset_pin);

struct xbee_addresses xbee_get_address_info();

void xbee_send_address_info(struct xbee_addresses info);

#endif //PIOS_XBEE_H
