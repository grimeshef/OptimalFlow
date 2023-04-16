set(LIB_NAME linear_encoder_handler)
set(SOURCES src/linear_encoder_handler.cpp include/linear_encoder_handler.h src/QEI.cpp include/QEI.h)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC
        include
        )

target_link_libraries(${LIB_NAME} PRIVATE
        mbed-os
        )