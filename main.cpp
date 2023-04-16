/* mbed Microcontroller Library
 * Copyright (c) 2023 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "udp_server.h"
#include "imu_handler.h"
#include <chrono>

const auto SYSTEM_TIMEOUT = 25ms;
Mutex udp_watchdog;
int watchdog_udp_connection_timer = 0;


Thread blink{osPriorityBelowNormal, 256};
Thread udp_communication_thread{osPriorityNormal, 4096};
Thread logic_thread;
Thread first_imu_handler_thread;
Thread second_imu_handler_thread;
Thread third_imu_handler_thread;

Mail<Cmd, 1> cmd_box;
Mail<Data, 1> data_box;

Mail<DataIMU, 1> first_imu_data_box;
Mail<DataIMU, 1> second_imu_data_box;
Mail<DataIMU, 1> third_imu_data_box;

using std_duration = std::chrono::duration<int64_t, std::micro>;
using std_ms = std::chrono::milliseconds;


std_ms calc_idle_time(
        const std_ms &system_timeout,
        const std_duration &last_time) {
    /// calculate how many time thread should be in idle state
    auto
            cur_time = std::chrono::duration<int32_t, std::micro>{us_ticker_read()};
    auto idle_timeout = system_timeout
                        - std::chrono::duration_cast<std_ms>(cur_time - last_time);
    if (idle_timeout < 0ms) {
        idle_timeout = 0ms;
    } else if (idle_timeout > system_timeout) {
        idle_timeout = system_timeout;
    }
    return idle_timeout;
}


[[noreturn]] void blink_loop() {
    DigitalOut led1(PF_13, 1);
    auto start_time = std_duration{us_ticker_read()};
    while (true) {
        start_time = std_duration{us_ticker_read()};
        led1 = !led1;
        ThisThread::sleep_for(calc_idle_time(SYSTEM_TIMEOUT * 20, start_time));
    }
}


[[noreturn]] void udp_communication_loop() {
    auto start_time = std_duration{us_ticker_read()};
    UDPServer server;
    Cmd *cmd = nullptr;
    Data *data = nullptr;

    DataIMU *data_imu_first = nullptr;
    DataIMU *data_imu_second = nullptr;
    DataIMU *data_imu_third = nullptr;

    while (true) {
        start_time = std_duration{us_ticker_read()};
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
        ThisThread::sleep_for(calc_idle_time(SYSTEM_TIMEOUT, start_time));
    }
}


[[noreturn]] void first_imu_handler_loop() {
    auto start_time = std_duration{us_ticker_read()};
    IMUHandler imu(PA_9, PA_10, PA_8);
    DataIMU *data = nullptr;
    while (true) {
        start_time = std_duration{us_ticker_read()};

        imu.process();
        data = first_imu_data_box.try_alloc();
        if (data != nullptr) {
            *data = imu.get_data();
            first_imu_data_box.put(data);
        }
        auto delta_time = start_time - Kernel::Clock::now().time_since_epoch();;
        ThisThread::sleep_for(calc_idle_time(SYSTEM_TIMEOUT, start_time));
    }
}


[[noreturn]] void second_imu_handler_loop() {
    auto start_time = std_duration{us_ticker_read()};
    IMUHandler imu(PC_10_ALT0, PC_11_ALT0, PA_15);
    DataIMU *data = nullptr;
    while (true) {
        start_time = std_duration{us_ticker_read()};

        imu.process();
        data = second_imu_data_box.try_alloc();
        if (data != nullptr) {
            *data = imu.get_data();
            second_imu_data_box.put(data);
        }
        auto delta_time = start_time - Kernel::Clock::now().time_since_epoch();;
        ThisThread::sleep_for(calc_idle_time(SYSTEM_TIMEOUT, start_time));
    }
}


[[noreturn]] void third_imu_handler_loop() {
    auto start_time = std_duration{us_ticker_read()};
    IMUHandler imu(PE_1, PE_0, PB_9);
    DataIMU *data = nullptr;
    while (true) {
        start_time = std_duration{us_ticker_read()};

        imu.process();
        data = third_imu_data_box.try_alloc();
        if (data != nullptr) {
            *data = imu.get_data();
            third_imu_data_box.put(data);
        }
        auto delta_time = start_time - Kernel::Clock::now().time_since_epoch();;
        ThisThread::sleep_for(calc_idle_time(SYSTEM_TIMEOUT, start_time));
    }
}


//[[noreturn]] void logic_loop() {
//
//    DataIMU *data_imu = nullptr;
//
//    while (true) {
//        auto start_time = Kernel::Clock::now().time_since_epoch();
//
//
//        auto delta_time = start_time - Kernel::Clock::now().time_since_epoch();;
//        ThisThread::sleep_for(chrono::milliseconds(SYSTEM_TIMEOUT - delta_time));
//    }
//}


int main() {
    blink.start(callback(blink_loop));
    udp_communication_thread.start(callback(udp_communication_loop));
//    logic_thread.start(callback(logic_loop));
    first_imu_handler_thread.start(callback(first_imu_handler_loop));
    second_imu_handler_thread.start(callback(second_imu_handler_loop));
    third_imu_handler_thread.start(callback(third_imu_handler_loop));

    return 0;
}