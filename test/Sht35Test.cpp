#include <gtest/gtest.h>

#include "Sht35.hpp"

using namespace Sht35;

TEST(Sht35, command_code_decodes_correctly) {
    auto const command = Command(0x1234);
    EXPECT_EQ(command.mostSignificantByte, std::byte{0x12});
    EXPECT_EQ(command.leastSignificantByte, std::byte{0x34});    
}

TEST(Sht35, single_shot_command_is_mapped_correctly) {
    // This test is not exhaustive.
    // The goal is only to verify the mapping mechanism is working,
    // the correct command codes should be reviewed in the implementation.

    auto const command = buildSingleShotCommand(Repeatability::Medium, ClockStretching::Disabled);
    EXPECT_EQ(command.mostSignificantByte, std::byte{0x24});
    EXPECT_EQ(command.leastSignificantByte, std::byte{0x0B});    
}
