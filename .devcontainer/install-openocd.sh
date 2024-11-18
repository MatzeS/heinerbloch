#!/bin/sh
set -e -x

apt install -y libtool pkg-config libhidapi-libusb0 libhidapi-dev libusb-dev libusb-1.0-0-dev

cd /opt/
git clone --recursive https://github.com/raspberrypi/openocd
cd openocd
# hash suitable for the 2.0.0 pico-SDK
git checkout ebec9504d7ad2fbd7a64d60dace013267d80172d
./bootstrap
./configure --enable-cmsis-dap
make -j
make install
