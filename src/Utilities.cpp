#include "heinerbloch/Utilities.hpp"

bool testBits(std::byte input, std::byte test) {
    auto const masked = input & test;
    return masked == test;
}
