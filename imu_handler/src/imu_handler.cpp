//
// Created by main on 17.01.23.
//

#include "imu_handler.h"

IMUHandler::IMUHandler(PinName tx, PinName rx, PinName dir, int baud) :
        _buffered_serial(tx, rx, baud),
        _buf_req{},
        _buf_rec{},
        _dir(dir),
        _print(PD_8, PD_9)
{
    _buffered_serial.set_format();
    _print.set_format();
}

void IMUHandler::process() {
    _set_cmd();
    if (_buffered_serial.writable()) {
        _dir.write(0);
        _buffered_serial.write(_buf_req, sizeof(_buf_req));
    }
    if (_buffered_serial.readable()) {
        _dir.write(1);
        _buffered_serial.read(_buf_rec, sizeof(_buf_rec));
        _rec_parse(_buf_rec);
    }
}


void IMUHandler::_get_gyro_x() {

}

void IMUHandler::_get_gyro_y() {

}

void IMUHandler::_get_gyro_z() {

}

const DataIMU &IMUHandler::get_data() {
    return _data_imu;
}

int8_t IMUHandler::_checksum(const uint8_t buf[], int size) {
    int8_t sum = 0;
    for (int i = 1; i < size-1; i++) {
        sum += buf[i];
    }
    return sum;
}

void IMUHandler::_set_cmd() {
    _buf_req[0] = 0x68;
    _buf_req[1] = 0x1F;
    _buf_req[2] = 0x00;
    _buf_req[3] = 0x84;
    _buf_req[4] = 0xA3;
}

void IMUHandler::_rec_parse(const uint8_t *buf) {
    _data_imu.sign_roll = buf[1];
    _data_imu.angle_roll = buf[2];
    _data_imu.minutes_roll = buf[3];

    _data_imu.sign_pitch = buf[4];
    _data_imu.angle_pitch = buf[5];
    _data_imu.minutes_pitch = buf[6];

    _data_imu.sign_yaw = buf[7];
    _data_imu.angle_yaw = buf[8];
    _data_imu.minutes_yaw = buf[9];

    _data_imu.sign_acc_x = buf[10];
    _data_imu.whole_acc_x = buf[11];
    _data_imu.fraction_acc_x = buf[12];

    _data_imu.sign_acc_y = buf[13];
    _data_imu.whole_acc_y = buf[14];
    _data_imu.fraction_acc_y = buf[15];

    _data_imu.sign_acc_z = buf[16];
    _data_imu.whole_acc_z = buf[17];
    _data_imu.fraction_acc_z = buf[18];

    _data_imu.sign_gyro_x = buf[19];
    _data_imu.angle_gyro_x = buf[20];
    _data_imu.minutes_gyro_x = buf[21];

    _data_imu.sign_gyro_y = buf[22];
    _data_imu.angle_gyro_y = buf[23];
    _data_imu.minutes_gyro_y = buf[23];

    _data_imu.sign_gyro_z = buf[25];
    _data_imu.angle_gyro_z = buf[26];
    _data_imu.minutes_gyro_z = buf[27];
}

