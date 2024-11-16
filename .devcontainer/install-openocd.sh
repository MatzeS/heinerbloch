#!/bin/sh
set -e -x

apt install -y libtool pkg-config libhidapi-libusb0 libhidapi-dev libusb-dev libusb-1.0-0-dev

cd /opt/
git clone --recursive https://github.com/raspberrypi/openocd
cd openocd

./bootstrap
./configure --enable-cmsis-dap
make -j
make install
