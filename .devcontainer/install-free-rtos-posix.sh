#!/bin/sh
set -e -x

cd /opt/
git clone --recursive https://github.com/FreeRTOS/Lab-Project-FreeRTOS-POSIX FreeRTOS-POSIX
cd FreeRTOS-POSIX
git checkout e6b133f3b3e017d67acbe2713b1890357d576fe0
