#include <cstdio>
#include <vector>

#include <pico/filesystem.h>
#include <pico/stdio.h>

#include "heinerbloch/calculator.hpp"

int main() {
  stdio_init_all();

  std::vector<uint32_t> numbers{1337, 3, 60, 600, 7000, 1};
  auto const result = add(numbers);

  printf("result: %d\n", result);

  experiment();
}
