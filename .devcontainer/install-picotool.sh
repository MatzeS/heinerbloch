#!/bin/sh
set -e -x

cd /opt/
git clone https://github.com/raspberrypi/picotool
cd picotool/
git checkout 2.0.0
mkdir build
cd build/
cmake -D PICO_SDK_PATH=/opt/pico-sdk/ ..
make -j
make install
