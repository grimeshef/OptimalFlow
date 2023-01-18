set(LIB_NAME communication)
set(SOURCES
        src/udp_server.cpp
        src/crc.cpp)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC
        include
        )

target_link_libraries(${LIB_NAME} PUBLIC
        mbed-os
        mbed-nanostack
        imu_handler
        )