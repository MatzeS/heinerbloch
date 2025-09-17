#include "I2C.hpp"
#include "heinerbloch/BME280.hpp"
#include "heinerbloch/calculator.hpp"
#include "Task.h"

#include <pico/filesystem.h>
#include <pico/stdio.h>

#include <cstdio>
#include <vector>

int main() {
    stdio_init_all();
    sleep_ms(4000);
    printf("Starting\n");
    sleep_ms(1000);
    printf("Starting\n");
    sleep_ms(1000);
    printf("Starting now\n");
    
    auto const foo = 42;


        Task foo_tasktisch([foo](){
            printf("Hi there I am the task: %d\n", foo);
        });

    printf("End of main\n");
    vTaskStartScheduler();
    printf("Post Scheduler\n");

    while(1) {


    
        sleep_ms(2000);
    }

    // I2C::RP2350::Bus bus{I2C::RP2350::Instance::Zero, 5, 4};

    // BME280::Sensor bme{bus, BME280::Address::High};
    // // while(1)
    // bme.triggerSingleSample();

    // auto const data = bme.readData();
    // printf("temp %f\n", data->temperature);
    // printf("pressure %f\n", data->pressure);
    // printf("humidity %f\n", data->humidity);
}
