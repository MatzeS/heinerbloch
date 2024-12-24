#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <ranges>
#include <type_traits>

/// Tests that all bits set in 'test' are set in 'input'.
bool testBits(std::byte input, std::byte test);

// Copied from https://en.cppreference.com/w/cpp/numeric/byteswap
template <std::integral T>
constexpr T byteswap(T value) noexcept {
  static_assert(std::has_unique_object_representations_v<T>,
                "T may not have padding bits");
  auto value_representation =
      std::bit_cast<std::array<std::byte, sizeof(T)>>(value);
  std::ranges::reverse(value_representation);
  return std::bit_cast<T>(value_representation);
}

class ByteModification {
 private:
  std::byte mask;
  std::byte value;

 public:
  //  TODO move to cpp
  ByteModification(std::byte mask, std::byte value) : mask{mask}, value{value} {
    auto const excessBits = value & ~mask;
    if (excessBits != std::byte{0x00}) {
      throw std::runtime_error(
          "The value byte contains data which would be masked, this is "
          "probably not intentional.");
    }
  }

  ByteModification(uint8_t firstBitPosition, uint8_t bitWidth, std::byte value)
      : mask{0x00}, value{value << firstBitPosition} {
    if (firstBitPosition + bitWidth > 8) {
      throw std::runtime_error{"Bit modificications exceed byte width."};
    }
    for (uint8_t i = 0; i < bitWidth; i++) {
      mask |= std::byte{0b1} << (firstBitPosition + i);
    }
  }

  std::byte apply(std::byte data) const {
    // clear all masked bits
    data &= ~mask;
    // set all value bits
    data |= value;
    return data;
  }
};
