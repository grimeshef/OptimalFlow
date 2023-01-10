/* mbed Microcontroller Library
 * Copyright (c) 2023 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

Thread blink;


[[noreturn]] void blink_loop(){
    DigitalOut led1(PF_13, 1);
    while (true) {
        auto start_time = Kernel::Clock::now().time_since_epoch();
        led1 = !led1;
        auto delta_time = start_time - Kernel::Clock::now().time_since_epoch();;
        ThisThread::sleep_for(chrono::milliseconds(500ms - delta_time));
    }
}


int main()
{
    blink.start(callback(blink_loop));
    return 0;
}