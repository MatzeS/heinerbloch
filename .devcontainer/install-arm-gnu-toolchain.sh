#!/bin/sh
set -e -x

cd /opt/
wget https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi.tar.xz
xz -d arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi.tar.xz
tar -xvf arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi.tar
rm arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi.tar
mv arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi arm-gnu-toolchain
