FROM mcr.microsoft.com/devcontainers/base:bookworm

# System
RUN apt-get -y update
RUN apt-get -y upgrade

# Install arm gnu toolchain
# (Moved to the top because it is the largest download)
COPY install-arm-gnu-toolchain.sh /opt/install-arm-gnu-toolchain.sh
RUN /opt/install-arm-gnu-toolchain.sh
RUN rm /opt/install-arm-gnu-toolchain.sh

# System packages
RUN apt-get -y install \
    git \
    vim \
    tar \
    python3 \
    python3-pip \
    python3-venv \
    pre-commit

RUN apt-get -y install \
    build-essential \
    cmake \
    clang-format \
    clang-tidy \
    ninja-build \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    libstdc++-arm-none-eabi-newlib

# Install Rake
RUN apt-get -y install ruby
RUN gem install rake

# Install OpenOCD
COPY install-openocd.sh /opt/install-openocd.sh
RUN /opt/install-openocd.sh
RUN rm /opt/install-openocd.sh

# Install Pico SDK
COPY install-pico-sdk.sh /opt/install-pico-sdk.sh
RUN /opt/install-pico-sdk.sh
RUN rm /opt/install-pico-sdk.sh

# Install picotool
COPY install-picotool.sh /opt/install-picotool.sh
RUN /opt/install-picotool.sh
RUN rm /opt/install-picotool.sh

# Install FreeRTOS-Kernel
COPY install-free-rtos-kernel.sh /opt/install-free-rtos-kernel.sh
RUN /opt/install-free-rtos-kernel.sh
RUN rm /opt/install-free-rtos-kernel.sh

# Install FreeRTOS-POSIX
COPY install-free-rtos-posix.sh /opt/install-free-rtos-posix.sh
RUN /opt/install-free-rtos-posix.sh
RUN rm /opt/install-free-rtos-posix.sh

# Apply Pico SDK/POSIX compatibility patch
COPY pico-sdk-0001-refactor-prefix-semaphore-methods-with-pico_.patch /opt/pico-sdk/0001.patch
COPY tinyusb-0001-refactor-prefix-semaphore-methods-with-pico_.patch /opt/pico-sdk/lib/tinyusb/0001.patch
# git am requires committer identity (although it is not used).
RUN git config --global user.email "root@devcontainer" && git config --global user.name "root@devcontainer"
RUN cd /opt/pico-sdk; git am 0001.patch
RUN cd /opt/pico-sdk/lib/tinyusb; git am 0001.patch

# Install quicktype
COPY install-quicktype.sh /opt/install-quicktype.sh
RUN /opt/install-quicktype.sh
RUN rm /opt/install-quicktype.sh

# Install gtest
RUN apt-get -y install \
    libgtest-dev
