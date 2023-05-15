//
// Created by main on 10.01.23.
//
#include "udp_server.h"

UDPServer::UDPServer(const char *mask, const char *gateway, uint16_t port, const char *ip) :
        _ip{ip},
        _mask{mask},
        _gateway{gateway},
        _port{port},
        _out_buf{},
        _req{},
        _in_buf{}
{
    _timer.start();
}


UDPServer::~UDPServer() {
    _udp_socket.close();
    _ethernet.disconnect();
}


bool UDPServer::process_request() {
    _udp_socket.recvfrom(&_client_addr, _in_buf, BUFFER_REQ);
    memcpy((void *) &_req, (void *) _in_buf, sizeof(Request));

    auto crc = calculate_crc16_ccitt(_in_buf, sizeof(_req) - sizeof(_req.crc));
    if (crc == _req.crc) {
        _parse_request(_req);
        _respond(_client_addr);
        return true;
    }
    return false;
}


void UDPServer::_parse_request(const Request &req) {
    _cmd.cmd = static_cast<ECmd>(req.cmd.cmd);
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


void UDPServer::set_data(const Data &data) {
    _data = data;
}


const Cmd &UDPServer::get_cmd() const {
    return _cmd;
}


void UDPServer::_respond(const SocketAddress &addr) {
    _resp.time = chrono::milliseconds(_timer.elapsed_time().count());
    _resp.data.data_imu_first.roll = _data_imu_first.roll;
    _resp.data.data_imu_first.pitch = _data_imu_first.pitch;
    _resp.data.data_imu_first.yaw = _data_imu_first.yaw;

    _resp.data.data_imu_first.acc_x = _data_imu_first.acc_x;
    _resp.data.data_imu_first.acc_y = _data_imu_first.acc_y;
    _resp.data.data_imu_first.acc_z = _data_imu_first.acc_z;

    _resp.data.data_imu_first.gyro_x = _data_imu_first.gyro_x;
    _resp.data.data_imu_first.gyro_y = _data_imu_first.gyro_y;
    _resp.data.data_imu_first.gyro_z = _data_imu_first.gyro_z;

    _resp.data.data_imu_second.roll = _data_imu_second.roll;
    _resp.data.data_imu_second.pitch = _data_imu_second.pitch;
    _resp.data.data_imu_second.yaw = _data_imu_second.yaw;

    _resp.data.data_imu_second.acc_x = _data_imu_second.acc_x;
    _resp.data.data_imu_second.acc_y = _data_imu_second.acc_y;
    _resp.data.data_imu_second.acc_z = _data_imu_second.acc_z;

    _resp.data.data_imu_second.gyro_x = _data_imu_second.gyro_x;
    _resp.data.data_imu_second.gyro_y = _data_imu_second.gyro_y;
    _resp.data.data_imu_second.gyro_z = _data_imu_second.gyro_z;

    _resp.data.data_imu_third.roll = _data_imu_third.roll;
    _resp.data.data_imu_third.pitch = _data_imu_third.pitch;
    _resp.data.data_imu_third.yaw = _data_imu_third.yaw;

    _resp.data.data_imu_third.acc_x = _data_imu_third.acc_x;
    _resp.data.data_imu_third.acc_y = _data_imu_third.acc_y;
    _resp.data.data_imu_third.acc_z = _data_imu_third.acc_z;

    _resp.data.data_imu_third.gyro_x = _data_imu_third.gyro_x;
    _resp.data.data_imu_third.gyro_y = _data_imu_third.gyro_y;
    _resp.data.data_imu_third.gyro_z = _data_imu_third.gyro_z;

    uint16_t crc_offset = sizeof(_resp) - sizeof(_resp.crc);
    memcpy(reinterpret_cast<void *>(_out_buf),
           reinterpret_cast<void *>(&_resp), crc_offset);

    _resp.crc = calculate_crc16_ccitt(_out_buf, crc_offset);

    memcpy(reinterpret_cast<uint16_t *>(_out_buf + crc_offset),
           reinterpret_cast<void *>(&_resp.crc), sizeof(_resp.crc));

    _udp_socket.sendto(addr, _out_buf, sizeof(Response));
}

void UDPServer::set_data_imu_first(const DataIMU &data) {
    _data_imu_first = data;
}

void UDPServer::set_data_imu_second(const DataIMU &data) {
    _data_imu_second = data;
}

void UDPServer::set_data_imu_third(const DataIMU &data) {
    _data_imu_third = data;
}