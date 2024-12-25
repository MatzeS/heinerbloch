#include "heinerbloch/I2C.hpp"

#include <hardware/i2c.h>

namespace I2C::RP2350 {

enum class Instance { Zero, One };

using GpioPin = uint8_t;

class Bus : public ::I2C::Bus {
private:
    i2c_inst_t* instance;

public:
    Bus(Instance instance, GpioPin clock, GpioPin data);
    Bus(Bus&&) = default;
    Bus(Bus const&) = delete;
    Bus& operator=(Bus&&) = default;
    Bus& operator=(Bus const&) = delete;
    ~Bus() override;

    TransferResult read(Address slave, std::span<std::byte> buffer) override;
    TransferResult write(Address slave,
                         std::span<std::byte const> buffer) override;
};

}  // namespace I2C::RP2350
