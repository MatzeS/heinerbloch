#include <gtest/gtest.h>

#include "heinerbloch/I2C.hpp"

TEST(I2CTest, EightBitAddressesThrowException) {
  EXPECT_THROW({ I2C::Address{0xFF}; }, std::invalid_argument);
}
