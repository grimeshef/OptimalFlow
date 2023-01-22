//
// Created by main on 17.01.23.
//

#include "imu_handler.h"

RawSerial::RawSerial(PinName tx, PinName rx, int baud)
        :
        SerialBase(tx, rx, baud) {
}

IMUHandler::IMUHandler(PinName tx, PinName rx, PinName dir, int baud) :
        _communication(tx, rx, baud),
        _buf_req{},
        _buf_rec{},
        _dir(dir) {
    _communication.format(8, mbed::SerialBase::None, 1);
}

void IMUHandler::process() {
    _set_cmd();
    _transmit();
    _receive();
}

void IMUHandler::_transmit() {
    _dir.write(1);
    _communication.write(_buf_req, sizeof(_buf_req),
                         callback(this, &IMUHandler::_tx_callback),
                         SERIAL_EVENT_TX_COMPLETE);
    _tx_completed.wait_any(TX_COMPLETED_FLAG, TX_CALLBACK_TIMEOUT_ms);
    _tx_completed.clear();
}

void IMUHandler::_receive() {
    _communication.abort_read();
    _communication.read(_buf_rec, sizeof(_buf_rec),
                        callback(this, &IMUHandler::_rx_callback),
                        SERIAL_EVENT_RX_COMPLETE);
    _rx_completed.wait_any(RX_COMPLETED_FLAG, RX_CALLBACK_TIMEOUT_ms,
                                      false);
    _rx_completed.clear();
}

void IMUHandler::_tx_callback(int event) {
    _dir.write(0);
    _tx_completed.set(TX_COMPLETED_FLAG);
}

void IMUHandler::_rx_callback(int event) {
    _data_imu.cmd_1 = _buf_rec[0];
    _data_imu.cmd_2 = _buf_rec[1];
    _data_imu.cmd_3 = _buf_rec[2];
    _data_imu.cmd_4 = _buf_rec[3];

    _data_imu.sign_roll = _buf_rec[4];
    _data_imu.angle_roll = _buf_rec[5];
    _data_imu.minutes_roll = _buf_rec[6];

    _data_imu.sign_pitch = _buf_rec[7];
    _data_imu.angle_pitch = _buf_rec[8];
    _data_imu.minutes_pitch = _buf_rec[9];

    _data_imu.sign_yaw = _buf_rec[10];
    _data_imu.angle_yaw = _buf_rec[11];
    _data_imu.minutes_yaw = _buf_rec[12];

    _data_imu.sign_acc_x = _buf_rec[13];
    _data_imu.whole_acc_x = _buf_rec[14];
    _data_imu.fraction_acc_x = _buf_rec[15];

    _data_imu.sign_acc_y = _buf_rec[16];
    _data_imu.whole_acc_y = _buf_rec[17];
    _data_imu.fraction_acc_y = _buf_rec[18];

    _data_imu.sign_acc_z = _buf_rec[19];
    _data_imu.whole_acc_z = _buf_rec[20];
    _data_imu.fraction_acc_z = _buf_rec[21];

    _data_imu.sign_gyro_x = _buf_rec[22];
    _data_imu.angle_gyro_x = _buf_rec[23];
    _data_imu.minutes_gyro_x = _buf_rec[24];

    _data_imu.sign_gyro_y = _buf_rec[25];
    _data_imu.angle_gyro_y = _buf_rec[26];
    _data_imu.minutes_gyro_y = _buf_rec[27];

    _data_imu.sign_gyro_z = _buf_rec[28];
    _data_imu.angle_gyro_z = _buf_rec[29];
    _data_imu.minutes_gyro_z = _buf_rec[30];

    _data_imu.crc = _buf_rec[31];

    _rx_completed.set(RX_COMPLETED_FLAG);
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
    for (int i = 1; i < size - 1; i++) {
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

//    _buf_req[0] = 0x68;
//    _buf_req[1] = 0x05;
//    _buf_req[2] = 0x00;
//    _buf_req[3] = 0x0C;
//    _buf_req[4] = 0x06;
//    _buf_req[5] = 0x17;
}

void IMUHandler::_rec_parse(const uint8_t *buf) {
//    _data_imu.sign_roll = buf[1];
//    _data_imu.angle_roll = buf[2];
//    _data_imu.minutes_roll = buf[3];
//
//    _data_imu.sign_pitch = buf[4];
//    _data_imu.angle_pitch = buf[5];
//    _data_imu.minutes_pitch = buf[6];
//
//    _data_imu.sign_yaw = buf[7];
//    _data_imu.angle_yaw = buf[8];
//    _data_imu.minutes_yaw = buf[9];
//
//    _data_imu.sign_acc_x = buf[10];
//    _data_imu.whole_acc_x = buf[11];
//    _data_imu.fraction_acc_x = buf[12];
//
//    _data_imu.sign_acc_y = buf[13];
//    _data_imu.whole_acc_y = buf[14];
//    _data_imu.fraction_acc_y = buf[15];
//
//    _data_imu.sign_acc_z = buf[16];
//    _data_imu.whole_acc_z = buf[17];
//    _data_imu.fraction_acc_z = buf[18];
//
//    _data_imu.sign_gyro_x = buf[19];
//    _data_imu.angle_gyro_x = buf[20];
//    _data_imu.minutes_gyro_x = buf[21];
//
//    _data_imu.sign_gyro_y = buf[22];
//    _data_imu.angle_gyro_y = buf[23];
//    _data_imu.minutes_gyro_y = buf[23];
//
//    _data_imu.sign_gyro_z = buf[25];
//    _data_imu.angle_gyro_z = buf[26];
//    _data_imu.minutes_gyro_z = buf[27];
}
