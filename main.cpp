/* mbed Microcontroller Library
 * Copyright (c) 2023 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "udp_server.h"
#include "imu_handler.h"
#include "linear_encoder_handler.h"

const auto SYSTEM_TIMEOUT = 25ms;
Mutex udp_watchdog;
int watchdog_udp_connection_timer = 0;


Thread blink{osPriorityBelowNormal, 256};;
Thread udp_communication_thread{osPriorityNormal, 4096};
Thread logic_thread;

Mail<Cmd, 1> cmd_box;
Mail<double, 1> data_box;


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
    UDPServer server;
    Cmd *cmd = nullptr;
    double *data = nullptr;

    while(true) {

        if (!server.is_connected()) {
            server.connect();
        }

        data = data_box.try_get();
        if (data != nullptr) {
            server.set_data_lin_enc(*data);
            data_box.free(data);
        }

        if (server.process_request()) {
            udp_watchdog.lock();
            watchdog_udp_connection_timer = 0;
            udp_watchdog.unlock();
        }

        ThisThread::sleep_for(chrono::milliseconds(SYSTEM_TIMEOUT));
    }
}


[[noreturn]] void logic_loop() {

    LinEnc lin_enc(PC_11, PE_0, PA_10);
    double *data = nullptr;
    DigitalOut dir1(PA_15, 0);
    DigitalOut dir2(PB_7, 0);
    DigitalOut dir3(PB_9, 0);
    while (true) {
        auto start_time = Kernel::Clock::now().time_since_epoch();
        dir1.write(0);
        dir2.write(0);
        dir3.write(0);
        data = data_box.try_alloc();
        if (data != nullptr) {
            *data = lin_enc.get_data();
             data_box.put(data);
        }
        auto delta_time = start_time - Kernel::Clock::now().time_since_epoch();;
        ThisThread::sleep_for(chrono::milliseconds(SYSTEM_TIMEOUT - delta_time));
    }
}


int main()
{
    blink.start(callback(blink_loop));
    udp_communication_thread.start(callback(udp_communication_loop));
    logic_thread.start(callback(logic_loop));

    return 0;
}