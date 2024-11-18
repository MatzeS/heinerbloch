#!/bin/sh
set -e -x

# There current 13.3 version was removed from the download server (?).
# In the meantime the 13.2 is used...

# cd /opt/
# wget https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi.tar.xz
# xz -d arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi.tar.xz
# tar -xvf arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi.tar
# rm arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi.tar
# mv arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi arm-gnu-toolchain

cd /opt/
wget https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz
xz -d arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz
tar -xvf arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar
rm arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar
mv arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi arm-gnu-toolchain
