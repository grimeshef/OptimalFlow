//
// Created by main on 17.01.23.
//

#pragma once

#include "mbed.h"

const uint8_t BUF_SIZE_REQ = 5;
const uint8_t BUF_SIZE_REC = 29;

#pragma pack(push, 1)
struct DataIMU {
//    float roll;
//    float pitch;
//    float yaw;
//    float ACC_X;
//    float ACC_Y;
//    float ACC_Z;
//    float gyro_x;
//    float gyro_y;
//    float gyro_z;
    uint8_t sign_roll;
    uint8_t angle_roll;
    uint8_t minutes_roll;

    uint8_t sign_pitch;
    uint8_t angle_pitch;
    uint8_t minutes_pitch;

    uint8_t sign_yaw;
    uint8_t angle_yaw;
    uint8_t minutes_yaw;

    uint8_t sign_acc_x;
    uint8_t whole_acc_x;
    uint8_t fraction_acc_x;

    uint8_t sign_acc_y;
    uint8_t whole_acc_y;
    uint8_t fraction_acc_y;

    uint8_t sign_acc_z;
    uint8_t whole_acc_z;
    uint8_t fraction_acc_z;

    uint8_t sign_gyro_x;
    uint8_t angle_gyro_x;
    uint8_t minutes_gyro_x;

    uint8_t sign_gyro_y;
    uint8_t angle_gyro_y;
    uint8_t minutes_gyro_y;

    uint8_t sign_gyro_z;
    uint8_t angle_gyro_z;
    uint8_t minutes_gyro_z;
};
#pragma pack(pop)

class IMUHandler {
public:
    IMUHandler(PinName tx, PinName rx, PinName dir, int baud = 115200);
    void process();
    const DataIMU &get_data();

private:
    BufferedSerial _buffered_serial;
    DigitalOut _dir;
    uint8_t _buf_req[BUF_SIZE_REQ];
    uint8_t _buf_rec[BUF_SIZE_REC];
    DataIMU _data_imu;

    BufferedSerial _print;

    int8_t _checksum(const uint8_t buf[], int size);
    void _rec_parse(const uint8_t buf[]);
    void _get_gyro_x();
    void _get_gyro_y();
    void _get_gyro_z();
    void _set_cmd();
};
