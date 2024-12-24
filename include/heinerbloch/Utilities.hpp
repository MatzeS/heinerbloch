#pragma once

#include <cstddef>

/// Tests that all bits set in 'test' are set in 'input'.
bool testBits(std::byte input, std::byte test);
