#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <tl/expected.hpp>
#include <utility>
#include "Utilities.hpp"

namespace I2C {

class Address {
 private:
  std::byte address;

 public:
  explicit Address(std::byte address);

  explicit Address(uint8_t address) : Address{std::byte{address}} {}

  // NOLINTNEXTLINE(google-explicit-constructor) intentional conversion
  operator std::byte() const;
};

enum class Error {
  /// Either triggered if the slave rejects the communication intentionally
  /// or if no slave is present that acknowledged the communication.
  NotAcknowledged
};

using TransferResult = tl::expected<std::size_t, Error>;

class Bus {
 public:
  virtual ~Bus() = default;
  virtual TransferResult read(Address slave, std::span<std::byte> buffer) = 0;
  virtual TransferResult write(Address slave,
                               std::span<std::byte const> buffer) = 0;
};

class Slave {
 private:
  Bus& bus;
  Address address;

 public:
  Slave(Bus& bus, Address address);

  TransferResult read(std::span<std::byte> buffer);

  TransferResult write(std::span<const std::byte> buffer);
};

}  // namespace I2C
