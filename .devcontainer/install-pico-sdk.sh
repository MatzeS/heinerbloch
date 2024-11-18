#!/bin/sh
set -e -x

cd /opt/
git clone --recursive https://github.com/raspberrypi/pico-sdk
cd pico-sdk
git checkout 2.0.0

# Switch to VFS pull request
# This can be removed once VFS has been merged fully
# See: https://github.com/raspberrypi/pico-sdk/pull/1715
git remote add oyama https://github.com/oyama/pico-sdk
git fetch oyama
git checkout 5cc96b743a57523ad6f7eb3fb26b03fd9e666022
git submodule update --recursive
