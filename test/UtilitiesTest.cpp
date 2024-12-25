#include "heinerbloch/Utilities.hpp"

#include <gtest/gtest.h>

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

TEST(Utilities, ByteModification_value_and_mask_apply_correctly) {
    auto const data = std::byte{0b00001111};
    auto const modification =
        ByteModification{std::byte{0b00110011}, std::byte{0b00010001}};

    auto const result = modification.apply(data);

    EXPECT_EQ(result, std::byte{0b00011101});
}

TEST(Utilities, ByteModification_construction_from_bit_section) {
    auto const data = std::byte{0b10101010};

    auto const modification = ByteModification{2, 4, std::byte{0b0101}};

    auto const result = modification.apply(data);

    EXPECT_EQ(result, std::byte{0b10010110});
}
