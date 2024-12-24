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
