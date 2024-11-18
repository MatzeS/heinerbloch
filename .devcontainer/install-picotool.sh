#!/bin/sh
set -e -x

cd /opt/
git clone https://github.com/raspberrypi/picotool
cd picotool/
mkdir build
cd build/
cmake -D PICO_SDK_PATH=/opt/pico-sdk/ ..
make -j
make install
