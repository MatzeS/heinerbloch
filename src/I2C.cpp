#include "heinerbloch/I2C.hpp"

namespace I2C {

Address::Address(std::byte address) : address(address) {
    if (testBits(address, std::byte{0b10000000})) {
        throw std::invalid_argument{
            "I2C addresses are 7-bit, but the 8th bit is set."};
    }
}

Address::operator std::byte() const {
    return address;
}

Slave::Slave(Bus& bus, Address address)
    : bus{bus},
      address{std::move(address)} {}

TransferResult Slave::read(std::span<std::byte> buffer) {
    return bus.read(address, buffer);
}

TransferResult Slave::write(std::span<const std::byte> buffer) {
    return bus.write(address, buffer);
}

}  // namespace I2C
