//
// Created by main on 17.01.23.
//

#pragma once

#include "mbed.h"

const uint8_t BUF_SIZE_REQ = 6;
const uint8_t BUF_SIZE_REC = 32;

const uint16_t RX_COMPLETED_FLAG = 0x340;
const uint16_t TX_COMPLETED_FLAG = 0x341;

const uint8_t TX_CALLBACK_TIMEOUT_ms = 10;
const uint8_t RX_CALLBACK_TIMEOUT_ms = 10;


#pragma pack(push, 1)
struct DataIMU {
//    uint8_t cmd_1;
//    uint8_t cmd_2;
//    uint8_t cmd_3;
//    uint8_t cmd_4;
//
//    uint8_t sign_roll;
//    uint8_t angle_roll;
//    uint8_t minutes_roll;
//
//    uint8_t sign_pitch;
//    uint8_t angle_pitch;
//    uint8_t minutes_pitch;
//
//    uint8_t sign_yaw;
//    uint8_t angle_yaw;
//    uint8_t minutes_yaw;
//
//    uint8_t sign_acc_x;
//    uint8_t whole_acc_x;
//    uint8_t fraction_acc_x;
//
//    uint8_t sign_acc_y;
//    uint8_t whole_acc_y;
//    uint8_t fraction_acc_y;
//
//    uint8_t sign_acc_z;
//    uint8_t whole_acc_z;
//    uint8_t fraction_acc_z;
//
//    uint8_t sign_gyro_x;
//    uint8_t angle_gyro_x;
//    uint8_t minutes_gyro_x;
//
//    uint8_t sign_gyro_y;
//    uint8_t angle_gyro_y;
//    uint8_t minutes_gyro_y;
//
//    uint8_t sign_gyro_z;
//    uint8_t angle_gyro_z;
//    uint8_t minutes_gyro_z;

    double roll;
    double pitch;
    double yaw;

    double acc_x;
    double acc_y;
    double acc_z;

    double gyro_x;
    double gyro_y;
    double gyro_z;
//    uint8_t crc;
};
#pragma pack(pop)

class RawSerial :
        public SerialBase {
public:
    RawSerial(PinName tx, PinName rx, int baud);
};

class IMUHandler {
public:
    IMUHandler(PinName tx, PinName rx, PinName dir, int baud = 115200);
    void process();
    const DataIMU &get_data();

private:
    void _rx_callback(int event);
    void _tx_callback(int event);
    void _receive();
    void _transmit();

    RawSerial _communication;
    EventFlags _tx_completed;
    EventFlags _rx_completed;

    DigitalOut _dir;
    uint8_t _buf_req[BUF_SIZE_REQ];
    uint8_t _buf_rec[BUF_SIZE_REC];
    DataIMU _data_imu;

    void _set_cmd();
    double _three_bytes_to_double(const uint8_t sign, const uint8_t whole, const uint8_t fraction) const;
    std::uint32_t convert_to_dec(const uint8_t number) const;
};
