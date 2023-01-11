//
// Created by main on 10.01.23.
//
#include "udp_server.h"

UDPServer::UDPServer(const char *mask, const char *gateway, uint16_t port, const char *ip) :
        _ip{ip},
        _mask{mask},
        _gateway{gateway},
        _port{port}
        {}

UDPServer::~UDPServer() {
    _udp_socket.close();
    _ethernet.disconnect();
}

bool UDPServer::process_request() {
    _udp_socket.recvfrom(&_client_addr, _in_buf, BUFFER_REQ);
//    memcpy((void *) &_last_msg_type, (void *) (_in_buf + 1), sizeof(Header::type));
    return false;
}

void UDPServer::connect() {
    _ethernet.set_network(_ip, _mask, _gateway);
    _ethernet.connect();
    _udp_socket.open(&_ethernet);
    _udp_socket.bind(_port);
}

bool UDPServer::is_connected() const {
    return _ethernet.get_connection_status() == NSAPI_STATUS_CONNECTING;;
}

void UDPServer::set_data() {

}

void UDPServer::get_data() {

}

