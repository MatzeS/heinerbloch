

/* main.c
Copyright 2021 Carl John Kugler III

Licensed under the Apache License, Version 2.0 (the License); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at

   http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
*/

#include <pico/filesystem.h>
#include <pico/stdio.h>
#include <format>
#include <iostream>

struct Foo {};

template <>
struct std::formatter<Foo> : std::formatter<string_view> {
  auto format(const Foo&, std::format_context& ctx) const {
    std::string temp;
    std::format_to(std::back_inserter(temp), "nothing but Foo");
    return std::formatter<string_view>::format(temp, ctx);
  }
};

#include <fstream>
#include <iostream>

extern "C" bool fs_init(void);

int main() {
  stdio_init_all();

  auto f = Foo{};
  std::string message = std::format("The answer is {} and {}.", 42, f);
  printf("%s", message.c_str());

  auto result = fs_init();
  printf("\n\nfs init: %s\n", result ? "true" : "false");

  std::ofstream myfile;
  myfile.open("/somefile.txt");
  myfile << "some file content.\n";
  myfile.close();

  printf("done");
}
