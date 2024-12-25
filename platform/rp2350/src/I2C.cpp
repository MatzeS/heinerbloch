#include "I2C.hpp"

#include <pico/stdlib.h>

namespace I2C::RP2350 {

Bus::Bus(Instance instance, GpioPin clock, GpioPin data) : instance{nullptr} {
    switch (instance) {
        case Instance::Zero:
            this->instance = i2c0;
            break;
        case Instance::One:
            this->instance = i2c1;
            break;
        default:
            throw new std::runtime_error(
                "Invalid enum value for I2C instance.");
    }

    i2c_init(this->instance, 100000);

    gpio_set_function(data, GPIO_FUNC_I2C);
    gpio_set_function(clock, GPIO_FUNC_I2C);
    gpio_pull_up(data);
    gpio_pull_up(clock);
}

Bus::~Bus() {
    i2c_deinit(instance);
}

TransferResult Bus::read(Address slave, std::span<std::byte> buffer) {
    static_assert(std::is_same_v<uint8_t, unsigned char>);
    int result = i2c_read_blocking(instance, std::to_integer<uint8_t>(slave),
                                   reinterpret_cast<uint8_t*>(buffer.data()),
                                   buffer.size(), false);
    if (result == PICO_ERROR_GENERIC) {
        return tl::make_unexpected(Error::NotAcknowledged);
    }
    return result;
}

TransferResult Bus::write(Address slave, std::span<std::byte const> buffer) {
    static_assert(std::is_same_v<uint8_t, unsigned char>);
    int result = i2c_write_blocking(
        instance, std::to_integer<uint8_t>(slave),
        reinterpret_cast<uint8_t const*>(buffer.data()), buffer.size(), false);
    if (result == PICO_ERROR_GENERIC) {
        return tl::make_unexpected(Error::NotAcknowledged);
    }
    return result;
}

}  // namespace I2C::RP2350
