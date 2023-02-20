//
// Created by main on 10.01.23.
//

#pragma once

#include "mbed.h"
#include "EthernetInterface.h"
#include "UDPSocket.h"
#include "crc.h"
#include "imu_handler.h"


const uint16_t BUFFER_REQ = 4;
const uint16_t BUFFER_RES = 300;


enum class ECmd : uint8_t {
    STOP = 0,
    RUN = 1
};

#pragma pack(push, 1)
struct Cmd {
    ECmd cmd;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Request {
    Cmd cmd;
    uint16_t crc;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Data {
    DataIMU data_imu_first;
    DataIMU data_imu_second;
    DataIMU data_imu_third;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Response {
    Data data;
    Kernel::Clock::duration time;
    uint16_t crc;
};
#pragma pack(pop)

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
    void set_data(const Data &data);
    void set_data_imu_first(const DataIMU &data);
    void set_data_imu_second(const DataIMU &data);
    void set_data_imu_third(const DataIMU &data);
    const Cmd &get_cmd() const;

private:
    SocketAddress _client_addr;
    EthernetInterface _ethernet;
    UDPSocket _udp_socket;

    void _respond(const SocketAddress &addr);
    void _parse_request(const Request &req);

    uint8_t _in_buf[BUFFER_REQ];
    uint8_t _out_buf[BUFFER_RES];

    const char *_ip;
    const char *_mask;
    const char *_gateway;
    uint16_t _port;

    Response _resp;
    Request _req;
    Data _data;
    Cmd _cmd;

    DataIMU _data_imu_first;
    DataIMU _data_imu_second;
    DataIMU _data_imu_third;

    Timer _timer;
};

