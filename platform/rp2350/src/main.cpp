#include "I2C.hpp"
#include "heinerbloch/BME280.hpp"
#include "heinerbloch/calculator.hpp"

#include <pico/filesystem.h>
#include <pico/stdio.h>

#include <cstdio>
#include <vector>

int main() {
    stdio_init_all();

    uint8_t foo = 0xAB;
    uint16_t bar = 0xABCD;

    uint8_t* bp = (uint8_t*)&bar;

    printf("%.2X \n", foo);
    printf("%.2X \n", bar);
    printf("%.2X \n", *bp);
    printf("%.2X \n", *(bp + 1));

    std::vector<uint32_t> numbers{1337, 3, 60, 600, 7000, 1};
    auto const result = add(numbers);

    printf("result: %d\n", result);

    I2C::RP2350::Bus bus{I2C::RP2350::Instance::Zero, 5, 4};

    BME280::Sensor bme{bus, BME280::Address::High};
    // while(1)
    bme.triggerSingleSample();

    auto const data = bme.readData();
    printf("temp %f\n", data->temperature);
    printf("pressure %f\n", data->pressure);
    printf("humidity %f\n", data->humidity);
}
