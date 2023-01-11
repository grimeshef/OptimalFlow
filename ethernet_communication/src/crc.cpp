//
// Created by main on 10.01.23.
//
#include "crc.h"
uint16_t calculate_crc16_ccitt(uint8_t* data_p, uint16_t len) {
    unsigned char i;
    uint16_t wData;
    uint16_t wCrc = 0xffff;

    if (len == 0)
        return wCrc;

    do {
        for (i = 0, wData = ((unsigned int) 0xff & *data_p++) << 8; i < 8; i++, wData <<= 1) {

            if ((wCrc & 0x8000) ^ (wData & 0x8000))
                wCrc = (wCrc << 1) ^ 0x1021;
            else wCrc <<= 1;
        }
    } while (--len);
    return wCrc & 0xffff;
}
