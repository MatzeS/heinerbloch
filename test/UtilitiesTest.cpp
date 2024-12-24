#include <gtest/gtest.h>

#include "heinerbloch/Utilities.hpp"

TEST(Utilities, test_bits_identifies_set_bits) {
  EXPECT_TRUE(testBits(std::byte{0xFF}, std::byte{0b00000001}));
  EXPECT_TRUE(testBits(std::byte{0xFF}, std::byte{0b01000001}));
}

TEST(Utilities, test_bits_rejects_unset_bits) {
  EXPECT_FALSE(testBits(std::byte{0x0F}, std::byte{0b01000000}));
  EXPECT_FALSE(testBits(std::byte{0x0F}, std::byte{0b01000001}));
}

TEST(Utilities, byteswap) {
  EXPECT_EQ(byteswap(uint8_t{0x12}), uint8_t{0x12});
  EXPECT_EQ(byteswap(uint16_t{0xABCD}), uint16_t{0xCDAB});
}
