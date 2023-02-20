/* mbed Microcontroller Library
 * Copyright (c) 2023 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "udp_server.h"
#include "imu_handler.h"

const auto SYSTEM_TIMEOUT = 25ms;
Mutex udp_watchdog;
int watchdog_udp_connection_timer = 0;


Thread blink;
Thread udp_communication_thread{osPriorityBelowNormal, 4096};
Thread logic_thread;
Thread first_imu_handler_thread;
Thread second_imu_handler_thread;
Thread third_imu_handler_thread;

Mail<Cmd, 1> cmd_box;
Mail<Data, 1> data_box;

Mail<DataIMU, 1> first_imu_data_box;
Mail<DataIMU, 1> second_imu_data_box;
Mail<DataIMU, 1> third_imu_data_box;


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
    Data *data = nullptr;

    DataIMU *data_imu_first = nullptr;
    DataIMU *data_imu_second = nullptr;
    DataIMU *data_imu_third = nullptr;

    while(true) {
        auto start_time = Kernel::Clock::now().time_since_epoch();

        if (!server.is_connected()) {
            server.connect();
        }

//        data = data_box.try_get();
//        if (data != nullptr) {
//            server.set_data(*data);
//            data_box.free(data);
//        }

        data_imu_first = first_imu_data_box.try_get();
        if (data_imu_first != nullptr) {
            server.set_data_imu_first(*data_imu_first);
            first_imu_data_box.free(data_imu_first);
        }

        data_imu_second = second_imu_data_box.try_get();
        if (data_imu_second != nullptr) {
            server.set_data_imu_second(*data_imu_second);
            second_imu_data_box.free(data_imu_second);
        }

        data_imu_third = third_imu_data_box.try_get();
        if (data_imu_third != nullptr) {
            server.set_data_imu_third(*data_imu_third);
            third_imu_data_box.free(data_imu_third);
        }

        if (server.process_request()) {
            udp_watchdog.lock();
            watchdog_udp_connection_timer = 0;
            udp_watchdog.unlock();
        }

    //    cmd = cmd_box.try_alloc();
    //    if (cmd != nullptr) {
    //        *cmd = server.get_cmd();
   //         cmd_box.put(cmd);
    //    }

        auto delta_time = start_time - Kernel::Clock::now().time_since_epoch();
        ThisThread::sleep_for(chrono::milliseconds(SYSTEM_TIMEOUT - delta_time));
    }
}


[[noreturn]] void first_imu_handler_loop() {

    IMUHandler imu(PA_9, PA_10, PA_8);
    DataIMU *data = nullptr;
    while (true) {
        auto start_time = Kernel::Clock::now().time_since_epoch();

        imu.process();
            data = first_imu_data_box.try_alloc();
            if (data != nullptr) {
                *data = imu.get_data();
                first_imu_data_box.put(data);
            }
        auto delta_time = start_time - Kernel::Clock::now().time_since_epoch();;
        ThisThread::sleep_for(chrono::milliseconds(SYSTEM_TIMEOUT - delta_time));
    }
}


[[noreturn]] void second_imu_handler_loop() {

    IMUHandler imu(PC_10_ALT0, PC_11_ALT0, PA_15);
    DataIMU *data = nullptr;
    while (true) {
        auto start_time = Kernel::Clock::now().time_since_epoch();

        imu.process();
        data = second_imu_data_box.try_alloc();
        if (data != nullptr) {
            *data = imu.get_data();
            second_imu_data_box.put(data);
        }
        auto delta_time = start_time - Kernel::Clock::now().time_since_epoch();;
        ThisThread::sleep_for(chrono::milliseconds(SYSTEM_TIMEOUT - delta_time));
    }
}


[[noreturn]] void third_imu_handler_loop() {

    IMUHandler imu(PE_1, PE_0, PB_9);
    DataIMU *data = nullptr;
    while (true) {
        auto start_time = Kernel::Clock::now().time_since_epoch();

        imu.process();
        data = third_imu_data_box.try_alloc();
        if (data != nullptr) {
            *data = imu.get_data();
            third_imu_data_box.put(data);
        }
        auto delta_time = start_time - Kernel::Clock::now().time_since_epoch();;
        ThisThread::sleep_for(chrono::milliseconds(SYSTEM_TIMEOUT - delta_time));
    }
}


[[noreturn]] void logic_loop() {

    DataIMU *data_imu = nullptr;

    while (true) {
        auto start_time = Kernel::Clock::now().time_since_epoch();


        auto delta_time = start_time - Kernel::Clock::now().time_since_epoch();;
        ThisThread::sleep_for(chrono::milliseconds(SYSTEM_TIMEOUT - delta_time));
    }
}


int main()
{
    blink.start(callback(blink_loop));
    udp_communication_thread.start(callback(udp_communication_loop));
    logic_thread.start(callback(logic_loop));
    first_imu_handler_thread.start(callback(first_imu_handler_loop));
    second_imu_handler_thread.start(callback(second_imu_handler_loop));
    third_imu_handler_thread.start(callback(third_imu_handler_loop));

    return 0;
}