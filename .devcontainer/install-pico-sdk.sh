#!/bin/sh
set -e -x

cd /opt/
git clone --recursive https://github.com/raspberrypi/pico-sdk
cd pico-sdk
git checkout 2.0.0
