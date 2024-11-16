FROM mcr.microsoft.com/devcontainers/base:bookworm

# System
RUN apt-get -y update
RUN apt-get -y upgrade

# System packages
## General stuff
RUN apt-get -y install \
    git \
    vim \
    tar \
    python3 \
    python3-pip \
    python3-venv

RUN apt-get -y install \
    build-essential \
    cmake \
    ninja-build \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    libstdc++-arm-none-eabi-newlib

# Install Pico SDK
COPY install-pico-sdk.sh /opt/install-pico-sdk.sh
RUN /opt/install-pico-sdk.sh
RUN rm /opt/install-pico-sdk.sh

# Install OpenOCD
COPY install-openocd.sh /opt/install-openocd.sh
RUN /opt/install-openocd.sh
RUN rm /opt/install-openocd.sh

# Install Rake
RUN apt-get -y install ruby
RUN gem install rake
