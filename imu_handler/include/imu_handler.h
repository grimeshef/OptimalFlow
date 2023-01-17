//
// Created by main on 17.01.23.
//

#pragma once

#include "mbed.h"

const uint8_t BUF_SIZE = 8;

#pragma pack(push, 1)
struct DataIMU {
    uint32_t gyro_x;
    uint32_t gyro_y;
    uint32_t gyro_z;
};
#pragma pack(pop)

class IMUHandler {
public:
    IMUHandler(PinName tx, PinName rx, int baud = 115200);
    void process();
    const DataIMU &get_data();

private:
    BufferedSerial _buffered_serial;
    char _buf[BUF_SIZE];
    DataIMU _data_imu;

    void _get_gyro_x();
    void _get_gyro_y();
    void _get_gyro_z();
};
