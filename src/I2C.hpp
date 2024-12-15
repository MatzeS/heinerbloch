#pragma once

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <span>
#include <utility>
#include <tl/expected.hpp>

namespace {

// todo move to utils
// test
bool testBit(std::byte input, std::byte test) {
  auto const masked = input & test;
  return masked == test;
}

}  // namespace

namespace I2C {

class Address {
 private:
  std::byte address;

 public:

  explicit Address(std::byte address) : address(address) {
    if (testBit(address, std::byte{0b10000000})) {
      throw std::invalid_argument{
          "I2C addresses are 7bit, but the 8th bit is set."};
    }
  }

  explicit Address(uint8_t address) : Address{std::byte{address}} {}

};

enum class Error {
    NotAcknowledged
};

using TransferResult = tl::expected<std::size_t, Error>;

class Bus {
public:
    virtual ~Bus() = default;
    virtual TransferResult read(Address slave, std::span<std::byte> buffer) = 0;
    virtual TransferResult write(Address slave, std::span<std::byte const> buffer) = 0;
};

// namespace Slave{
//     class Interface {

//     };
// }

class Slave{
public:
    virtual ~Slave() = default;
    virtual TransferResult read(std::span<std::byte> buffer) = 0;
    virtual TransferResult write(std::span<std::byte const> buffer) = 0;
}; 

class SlaveImplementation{
private:
    Bus& bus;
    Address address;

public:
    SlaveImplementation(Bus&bus, Address address) : bus{bus}, address{std::move(address)} {}

    void read(std::span<std::byte> buffer) {
        bus.read(address, buffer);
    }

    void write(std::span<const std::byte> buffer) {
        bus.write(address, buffer);
    }

};

}
