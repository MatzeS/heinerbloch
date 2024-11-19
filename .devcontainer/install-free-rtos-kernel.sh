#!/bin/sh
set -e -x

cd /opt/
git clone --recursive https://github.com/raspberrypi/FreeRTOS-Kernel
cd FreeRTOS-Kernel
git checkout 4f7299d6ea746b27a9dd19e87af568e34bd65b15
