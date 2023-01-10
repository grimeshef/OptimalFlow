#!/bin/bash
if [ $# -eq 0 ]; then
  path="./cmake-build-debug"
else
  path=$1
fi
echo $path
mbed-tools configure -m NUCLEO_F429ZI -t GCC_ARM \
        -o $path \
        --mbed-os-path ./mbed-os \
        --app-config ./mbed_app.json
