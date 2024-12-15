#include "Sht35.hpp"

#include <stdexcept>
#include <utility>
#include <map>

namespace Sht35 {

Command::Command(uint16_t hexCode)
      : mostSignificantByte{static_cast<uint8_t>(hexCode >> 8)},
        leastSignificantByte{static_cast<uint8_t>(hexCode)} {}

Command buildSingleShotCommand(Repeatability repeatability, ClockStretching clockStretching){
    std::map<std::pair<Repeatability, ClockStretching>, Command> commands{
      {{Repeatability::High, ClockStretching::Enabled}, Command{0x2C06}},
      {{Repeatability::Medium, ClockStretching::Enabled}, Command{0x2C0D}},
      {{Repeatability::Low, ClockStretching::Enabled}, Command{0x2C10}},
      {{Repeatability::High, ClockStretching::Disabled}, Command{0x2400}},
      {{Repeatability::Medium, ClockStretching::Disabled}, Command{0x240B}},
      {{Repeatability::Low, ClockStretching::Disabled}, Command{0x2416}}
    };

    auto const result = commands.find({repeatability, clockStretching});
    if(result == commands.end()) {
        throw std::runtime_error("Command map incomplete!");
    }

    return result->second;
}

}  // namespace Sht35
