//
// Created by main on 17.01.23.
//

#include "imu_handler.h"
#include "math.h"

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
    _data_imu.start_pitch = 200;
}


void IMUHandler::process() {
    _set_cmd();
    _transmit();
    _receive();
}


void IMUHandler::_filtering_data() {
    if (_data_imu.start_pitch == 200) {
        _data_imu.start_pitch = asin(_data_imu.acc_y/9.81)*57.2958;
    }
    else {
        _data_imu.abs_pitch = _data_imu.start_pitch + _data_imu.pitch;
    }
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
    _data_imu.roll = _three_bytes_to_double(_buf_rec[4], _buf_rec[5], _buf_rec[6]);
    _data_imu.pitch = _three_bytes_to_double(_buf_rec[7], _buf_rec[8], _buf_rec[9]);
    _data_imu.yaw = _three_bytes_to_double(_buf_rec[10], _buf_rec[11], _buf_rec[12]);

    _data_imu.acc_x = _three_bytes_to_double(_buf_rec[13], _buf_rec[14], _buf_rec[15]);
    _data_imu.acc_y = _three_bytes_to_double(_buf_rec[16], _buf_rec[17], _buf_rec[18]);
    _data_imu.acc_z = _three_bytes_to_double(_buf_rec[19], _buf_rec[20], _buf_rec[21]);

    _data_imu.gyro_x = _three_bytes_to_double(_buf_rec[22], _buf_rec[23], _buf_rec[24]);
    _data_imu.gyro_y = _three_bytes_to_double(_buf_rec[25], _buf_rec[26], _buf_rec[27]);
    _data_imu.gyro_z = _three_bytes_to_double(_buf_rec[28], _buf_rec[29], _buf_rec[30]);

//    _data_imu.crc = _buf_rec[31];
    _rx_completed.set(RX_COMPLETED_FLAG);
}


const DataIMU &IMUHandler::get_data() {
    return _data_imu;
}


void IMUHandler::_set_cmd() {
    _buf_req[0] = 0x68;
    _buf_req[1] = 0x1F;
    _buf_req[2] = 0x00;
    _buf_req[3] = 0x84;
    _buf_req[4] = 0xA3;
//
//    _buf_req[0] = 0x68;
//    _buf_req[1] = 0x05;
//    _buf_req[2] = 0x00;
//    _buf_req[3] = 0x0C;
//    _buf_req[4] = 0x06;
//    _buf_req[5] = 0x17;
}


const double IMUHandler::_three_bytes_to_double(const uint8_t sign, const uint8_t whole, const uint8_t fraction) const {
    double number = 0.0;
    switch (sign) {
        case 0x00: number = convert_to_dec(whole) + convert_to_dec(fraction) * 0.01; break;
        case 0x01: number = 100 + convert_to_dec(whole) + convert_to_dec(fraction) * 0.01; break;
        case 0x10: number = -(convert_to_dec(whole) + convert_to_dec(fraction) * 0.01); break;
        case 0x11: number = -(100 + convert_to_dec(whole) + convert_to_dec(fraction) * 0.01); break;
        default: break;
    }
    return number;
}


std::uint32_t IMUHandler::convert_to_dec(const uint8_t number) const {
    uint32_t dec_num = 0;
    if (number/16 > 0) {
        dec_num = number - number/16 * 6;
    }
    else {
        dec_num = number;
    }
    return dec_num;
}
