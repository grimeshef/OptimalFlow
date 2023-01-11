/* mbed Microcontroller Library
 * Copyright (c) 2023 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "udp_server.h"

Thread blink;
Thread udp_communication_thread;

const auto SYSTEM_TIMEOUT = 25ms;

[[noreturn]] void blink_loop(){
    DigitalOut led1(PF_13, 1);
    while(true) {
        auto start_time = Kernel::Clock::now().time_since_epoch();
        led1 = !led1;
        auto delta_time = start_time - Kernel::Clock::now().time_since_epoch();;
        ThisThread::sleep_for(chrono::milliseconds(SYSTEM_TIMEOUT*20 - delta_time));
    }
}

[[noreturn]] void udp_communication_loop() {
    while(true) {

    }
}


int main()
{
    blink.start(callback(blink_loop));
    udp_communication_thread.start(callback(udp_communication_loop));
    return 0;
}