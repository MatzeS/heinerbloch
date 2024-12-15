#pragma once

#include <cstdint>
#include <cstddef>

namespace Sht35 {

enum class Repeatability { High, Medium, Low };

enum class ClockStretching { Enabled, Disabled };

struct Command {
  std::byte mostSignificantByte;
  std::byte leastSignificantByte;

  Command(uint16_t hexCode);
};

Command buildSingleShotCommand(Repeatability repeatability, ClockStretching clockStretching);
                    
/// Additional pins: Addr configured in hadware, needs to be considered here
//  alert pin
// nReset
// i2C clock stretching is supported
// fast mode is supported
//
// Address  is 0x44
// 0or 0x45 if high
// wait 1ms between i2c commands
class Sensor {
private:
  I2CSlave& i2cSlave;

public:
  using EncodedHumidty = uint16_t;
  using EncodedTemperature = uint16_t;

  Sht35(I2CSlave& i2cSlave) : i2cSlave{i2cSlave} {}

 private:
  startMeasurement() {}

};

// enum class Address {
//     High, Low
// };

// Commands  are mapped to 16bit
// repeatability high medium low, with clock stretching enabled disabled

}  // namespace Sht35
