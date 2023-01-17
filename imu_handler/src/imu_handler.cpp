//
// Created by main on 17.01.23.
//

#include "imu_handler.h"

IMUHandler::IMUHandler(PinName tx, PinName rx, int baud) :
        _buffered_serial(tx, rx, baud),
        _buf{}
{
}

void IMUHandler::process() {

    if (_buffered_serial.writable()) {
        _buffered_serial.write(_buf, sizeof(_buf));
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




