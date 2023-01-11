//
// Created by main on 10.01.23.
//

#pragma once

#include "mbed.h"
#include "EthernetInterface.h"
#include "UDPSocket.h"
#include "crc.h"

const uint16_t BUFFER_REQ = 100;
const uint16_t BUFFER_RES = 100;

class UDPServer {
public:
    explicit UDPServer(const char *mask = "255.255.255.0",
                       const char *gateway = "192.168.2.1",
                       uint16_t port = 5000,
                       const char *ip = "192.168.2.2");
    ~UDPServer();

    bool process_request();
    void connect();
    bool is_connected() const;
    void set_data();
    void get_data();

private:
    SocketAddress _client_addr;
    EthernetInterface _ethernet;
    UDPSocket _udp_socket;

    uint8_t _in_buf[BUFFER_REQ];
    uint8_t _out_buf[BUFFER_RES];

    const char *_ip;
    const char *_mask;
    const char *_gateway;
    uint16_t _port;

};

