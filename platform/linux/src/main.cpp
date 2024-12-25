#include "heinerbloch/calculator.hpp"

#include <cstdio>
#include <vector>

int main() {
    std::vector<uint32_t> numbers{1337, 3, 60, 600, 7000, 1};
    auto const result = add(numbers);

    printf("result: %d\n", result);

    experiment();
}
