FROM debian:bookworm

# System
RUN apt-get -y update
RUN apt-get -y upgrade

# Install Rake
RUN apt-get -y install ruby
RUN gem install rake

RUN apt-get -y install openocd

# RUN apt-get -y install git make autoconf automake texinfo

# RUN apt install -y libtool pkg-config libhidapi-libusb0 libhidapi-dev libusb-dev libusb-1.0-0-dev


# Install OpenOCD
# COPY install-openocd.sh /opt/install-openocd.sh
# RUN /opt/install-openocd.sh
# RUN rm /opt/install-openocd.sh

# ENTRYPOINT ["tail", "-f", "/dev/null"]
