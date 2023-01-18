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
        _in_buf{},
        buf(PD_8, PD_9, 115200)
{
    buf.set_format(8,mbed::SerialBase::None, 1);
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

    _resp.data.data_imu_first.sign_roll = _data_imu_first.sign_roll;
    _resp.data.data_imu_first.angle_roll = _data_imu_first.angle_roll;
    _resp.data.data_imu_first.minutes_roll = _data_imu_first.minutes_roll;

    _resp.data.data_imu_first.sign_pitch = _data_imu_first.sign_pitch;
    _resp.data.data_imu_first.angle_pitch = _data_imu_first.angle_pitch;
    _resp.data.data_imu_first.minutes_pitch = _data_imu_first.minutes_pitch;

    _resp.data.data_imu_first.sign_yaw = _data_imu_first.sign_yaw;
    _resp.data.data_imu_first.angle_yaw = _data_imu_first.angle_yaw;
    _resp.data.data_imu_first.minutes_yaw = _data_imu_first.minutes_yaw;

    _resp.data.data_imu_first.sign_acc_x = _data_imu_first.sign_acc_x;
    _resp.data.data_imu_first.whole_acc_x = _data_imu_first.whole_acc_x;
    _resp.data.data_imu_first.fraction_acc_x = _data_imu_first.fraction_acc_x;

    _resp.data.data_imu_first.sign_acc_y = _data_imu_first.sign_acc_y;
    _resp.data.data_imu_first.whole_acc_y = _data_imu_first.whole_acc_y;
    _resp.data.data_imu_first.fraction_acc_y = _data_imu_first.fraction_acc_y;

    _resp.data.data_imu_first.sign_acc_z = _data_imu_first.sign_acc_z;
    _resp.data.data_imu_first.whole_acc_z = _data_imu_first.whole_acc_z;
    _resp.data.data_imu_first.fraction_acc_z = _data_imu_first.fraction_acc_z;

    _resp.data.data_imu_first.sign_gyro_x = _data_imu_first.sign_gyro_x;
    _resp.data.data_imu_first.angle_gyro_x = _data_imu_first.angle_gyro_x;
    _resp.data.data_imu_first.minutes_gyro_x = _data_imu_first.minutes_gyro_x;

    _resp.data.data_imu_first.sign_gyro_y = _data_imu_first.sign_gyro_y;
    _resp.data.data_imu_first.angle_gyro_y = _data_imu_first.angle_gyro_y;
    _resp.data.data_imu_first.minutes_gyro_y = _data_imu_first.minutes_gyro_y;

    _resp.data.data_imu_first.sign_gyro_z = _data_imu_first.sign_gyro_z;
    _resp.data.data_imu_first.angle_gyro_z = _data_imu_first.angle_gyro_z;
    _resp.data.data_imu_first.minutes_gyro_z = _data_imu_first.minutes_gyro_z;

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

