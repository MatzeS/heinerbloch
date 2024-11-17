# Heinerbloch

Experimental playground for Raspberry Pi Pico related firmware development focusing on the RP2350.

## Development Setup
This repository contains submodule. Clone recursively or `git submodule update --init --recursive --remote`

### Rake
Common devops tasks are scripted using `rake`.
Install `ruby` and the `rake` gem to make the rake utility available.
```
apt install ruby
gem install rake
```

Check out the available rake tasks with `rake --tasks` or `rake -T`.

### Devcontainer
The development toolchain is installed and bundled as devcontainer.
Install the devcontainer CLI tool:
```
npm install -g @devcontainers/cli
```
The important devcontainer related operations are provided in the `dev` tasks group: `rake --tasks dev:`


### Debug Probe USB Permissions
For programming over OpenOCD the devcontainer needs access to the debugging probe.
If openocd cannot open the device due to insufficient permissions, it will simply output that no probe is available.

The following udev rules open the permissions for the Pico Debug probe:
```
SUBSYSTEM=="usb", ATTRS{idVendor}=="2e8a", ATTRS{idProduct}=="000c", MODE="0666"
```
Do not forget to reload and trigger the rules.

This rule may be automatically installed in `/etc/udev/rules.d/` through `rake setup:udev_rules`.
