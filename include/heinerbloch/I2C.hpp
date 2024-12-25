#pragma once

#include "Utilities.hpp"

#include <tl/expected.hpp>

#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <utility>

namespace I2C {

class Address {
private:
    std::byte address;

public:
    /// I2C slaves are addressed with one byte containing the 7-bit address and
    /// the read/write direction in the least-significant bit. This address is
    /// constructed form the seven lower bit of the 'address' parameter. Hence,
    /// the 8th bit can never be set. Make sure the address is represented
    /// correctly and not accidentally shifted on bit higher. The I2C
    /// implementation will eventaully combine this address with the read/write
    /// bit when transmitting on the bus.
    explicit Address(std::byte address);

    /// See 'Address(std::byte address)'
    explicit Address(uint8_t address) : Address{std::byte{address}} {}

    // NOLINTNEXTLINE(google-explicit-constructor) intentional conversion
    operator std::byte() const;
};

enum class Error {
    /// Either triggered if the slave rejects the communication intentionally
    /// or if no slave is present that acknowledged the communication.
    NotAcknowledged
};

/// The number of transferred bytes on success or the error on failure.
using TransferResult = tl::expected<std::size_t, Error>;

class Bus {
public:
    virtual ~Bus() = default;
    // TODO change terminology to transmit receive?
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
