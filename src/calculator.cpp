#include <fmt/format.h>
#include <fmt/ranges.h>
#include <data-models/foo.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

#include "heinerbloch/calculator.hpp"

uint32_t add(std::span<uint32_t> numbers) {
  std::cout << fmt::format("well {}\n", fmt::join(numbers, ", "));
  uint32_t sum = 0;
  for (auto const e : numbers) {
    sum += e;
  }
  return sum;
}

void experiment() {
  using nlohmann::json;

  heinerbloch::Foo foo;
  foo.bar = 3.14;
  foo.baz = "asdf";
  foo.subFoo.emplace();
  foo.subFoo->innerValue = heinerbloch::InnerValue::A;

  json j;
  heinerbloch::to_json(j, foo);
  printf("%s\n", j.dump().c_str());
}
