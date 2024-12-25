#include "heinerbloch/BME280.hpp"
#include "heinerbloch/I2C.hpp"

#include <gtest/gtest.h>

using namespace I2C;
using namespace BME280;

/// A simple fake sensor implementation providing a static register memory and
/// impelments read operations. The static data was collected from a real world
/// sensor, use this fake device to run unit tests and verify correct read out
/// of sensor values including the compensation conversion. The data in here
/// corresponds to the following sensor values:
/// - Temperature: 22.03 C
/// - Humidity: 47.750977 %
/// - Pressure: 102488.96 Pa
///
/// The validity of these values has been verified experimentally by obseving
/// value changes when inducing environmental changes, i.e. touching the sensor
/// for temperature, breathing on the sensor for humidity and walking up and
/// down the stairs for pressure.
class FakeSensor {
    uint8_t const startAddress = 0x88;
    std::vector<std::byte> const data{
        std::byte{0x42}, std::byte{0x6E}, std::byte{0xA2}, std::byte{0x67},
        std::byte{0x32}, std::byte{0x00}, std::byte{0xC4}, std::byte{0x91},
        std::byte{0x43}, std::byte{0xD6}, std::byte{0xD0}, std::byte{0x0B},
        std::byte{0x1B}, std::byte{0x23}, std::byte{0x74}, std::byte{0xFF},
        std::byte{0xF9}, std::byte{0xFF}, std::byte{0xAC}, std::byte{0x26},
        std::byte{0x0A}, std::byte{0xD8}, std::byte{0xBD}, std::byte{0x10},
        std::byte{0x00}, std::byte{0x4B}, std::byte{0x51}, std::byte{0x00},
        std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00},
        std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00},
        std::byte{0x33}, std::byte{0x00}, std::byte{0x00}, std::byte{0xC0},
        std::byte{0x00}, std::byte{0x54}, std::byte{0x00}, std::byte{0x00},
        std::byte{0x00}, std::byte{0x00}, std::byte{0x60}, std::byte{0x02},
        std::byte{0x00}, std::byte{0x01}, std::byte{0xFF}, std::byte{0xFF},
        std::byte{0x1F}, std::byte{0x60}, std::byte{0x03}, std::byte{0x00},
        std::byte{0x08}, std::byte{0x00}, std::byte{0x24}, std::byte{0xFF},
        std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00},
        std::byte{0x01}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00},
        std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00},
        std::byte{0x60}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00},
        std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00},
        std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00},
        std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00},
        std::byte{0x00}, std::byte{0x6F}, std::byte{0x01}, std::byte{0x00},
        std::byte{0x13}, std::byte{0x22}, std::byte{0x03}, std::byte{0x1E},
        std::byte{0x2D}, std::byte{0x41}, std::byte{0xFF}, std::byte{0xFF},
        std::byte{0xFF}, std::byte{0xFF}, std::byte{0xFF}, std::byte{0xFF},
        std::byte{0xFF}, std::byte{0x00}, std::byte{0x01}, std::byte{0x00},
        std::byte{0x24}, std::byte{0x10}, std::byte{0x00}, std::byte{0x46},
        std::byte{0xA7}, std::byte{0xE0}, std::byte{0x7F}, std::byte{0x42},
        std::byte{0xA0}, std::byte{0x6D}, std::byte{0xB5}};

    uint8_t readAddress = startAddress;

public:
    TransferResult read(std::span<std::byte> buffer) {
        for (size_t i = 0; i < buffer.size(); i++) {
            auto const offset = readAddress - startAddress;
            buffer[i] = data[offset];
            readAddress++;
        }

        return {buffer.size()};
    }

    TransferResult write(std::span<std::byte const> buffer) {
        if (buffer.size() == 1) {
            // single byte encodes a read request for that address
            readAddress = std::to_integer<uint8_t>(buffer[0]);
        } else {
            // everything else is simply ignored
        }

        return {buffer.size()};
    }
};

/// This is a simple bus wrapper for the fake sensor,
/// which forwards every I2C operation to the fake device.
/// Not even the address is checked.
///
/// Structurally, this does nothing but the BME280::Sensor requires the bus as
/// argument (not the slave) and the underlying client structure is here
/// reproduced on the fake side.
class FakeBus : public I2C::Bus {
private:
    FakeSensor& sensor;

public:
    explicit FakeBus(FakeSensor& sensor) : sensor{sensor} {}

    TransferResult read(I2C::Address, std::span<std::byte> buffer) override {
        return sensor.read(buffer);
    }

    TransferResult write(I2C::Address,
                         std::span<std::byte const> buffer) override {
        return sensor.write(buffer);
    }
};

class BME280Test : public testing::Test {
public:
    FakeSensor fakeSensor;
    FakeBus fakeBus{fakeSensor};
    Sensor sensor{fakeBus, BME280::Address::Low};
};

TEST_F(BME280Test, read_data_works_with_fake) {
    EXPECT_TRUE(sensor.readData().has_value());
}

TEST_F(BME280Test, temperature_decodes_correctly) {
    EXPECT_NEAR(sensor.readData()->temperature, 22.03, 0.01);
}

TEST_F(BME280Test, humidity_decodes_correctly) {
    EXPECT_NEAR(sensor.readData()->humidity, 47.750, 0.001);
}

TEST_F(BME280Test, pressure_decodes_correctly) {
    EXPECT_NEAR(sensor.readData()->pressure, 102488.96, 0.01);
}

TEST_F(BME280Test, configuration_encodes_correctly) {
    // This test is not exhaustive to all combinations of configuration
    // parameters and their correct encoding. It only verifies the basic
    // structure and examplary correct encoding.
    auto const config =
        Configuration{.temperatureOversampling = Oversampling::Skipped,
                      .pressureOversampling = Oversampling::TwoSamples,
                      .humidityOversampling = Oversampling::EightSamples,
                      .mode = Mode::Normal};

    auto const encoded = encode(config);

    EXPECT_EQ(encoded.ctrl_hum, std::byte{0b00000100});
    EXPECT_EQ(encoded.ctrl_meas, std::byte{0b00001011});
}
