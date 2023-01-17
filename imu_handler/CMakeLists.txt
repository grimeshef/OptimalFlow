set(LIB_NAME imu_handler)
set(SOURCES src/imu_handler.cpp include/imu_handler.h)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC
        include
        )

target_link_libraries(${LIB_NAME} PUBLIC
        mbed-os
        )