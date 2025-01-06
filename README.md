# Heinerbloch

[![.github/workflows/ci.yaml](https://github.com/MatzeS/heinerbloch/actions/workflows/ci.yaml/badge.svg?branch=main&event=push)](https://github.com/MatzeS/heinerbloch/actions/workflows/ci.yaml)

Experimental playground for Raspberry Pi Pico related firmware development focusing on the RP2350.

## Development Setup
This repository contains submodule. Clone recursively or run `git submodule update --init --recursive --remote` if you forgot.

### pre-commit
The repository uses [pre-commit](https://pre-commit.com/) to assert basic code compliance during development.
The pre-commit hooks include formatting and simple linting operations if suitatble.
Additional linting can be run manually and is enforced by the CI.

All pre-commit hooks can be run outside the devcontainer (next section).
Of course it is possible to commit inside the dev container, and your git config is passed through
to the container to simplify that.

### Devcontainer
The full development toolchain is installed and bundled as devcontainer that can be opened in VSCode.

If you prefer a commandline approach, consider using the devcontainer CLI tool and using the dev container relatetd `rake` tasks.
```
npm install -g @devcontainers/cli
rake dev:build && rake dev:start && rake dev:enter
rake dev:stop && rake dev:remove
```

### Rake
Common development operations are scripted using `rake`.
Install `ruby` and the `rake` gem to make the rake utility available (follow ruby's instructions regarding your `$PATH`).
```
apt install ruby
gem install rake
```

Check out the available rake tasks with `rake --tasks` or `rake -T`.

### Debug Probe USB Permissions
For programming over OpenOCD the devcontainer needs access to the debugging probe.
If openocd cannot open the device due to insufficient permissions, it will simply output that no probe is available.

The following udev rules open the permissions for the Pico Debug probe:
```
SUBSYSTEM=="usb", ATTRS{idVendor}=="2e8a", ATTRS{idProduct}=="000c", MODE="0666"
```
Do not forget to reload and trigger the rules after changing them.

This rule may be automatically installed in `/etc/udev/rules.d/` and triggerd through `rake setup:udev_rules`.
