#include <gtest/gtest.h>

#include "I2C.hpp"
#include "Sht35.hpp"

TEST(I2CTest, EightBitAddressesThrowException) {
  EXPECT_THROW({ I2C::Address{0xFF}; }, std::invalid_argument);
}
