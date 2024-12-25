#pragma once

#include "I2C.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>

/// Standby time and IIR filter are not yet implemented.
namespace BME280 {

enum class Address { Low, High };

enum class Mode {
    Sleep,

    /// In this mode a single measurement is performed.
    /// The sensor returns to sleep mode when the measurement is finished.
    Forced,

    Normal
};

enum class Oversampling {
    Skipped,
    OneSample,
    TwoSamples,
    FourSamples,
    EightSamples,
    SixteenSamples
};

struct Configuration {
    Oversampling temperatureOversampling;
    Oversampling pressureOversampling;
    Oversampling humidityOversampling;
    Mode mode;
};

struct EncodedConfiguration {
    std::byte ctrl_meas;
    std::byte ctrl_hum;
};

EncodedConfiguration encode(Configuration const& configuration);

/// Calibration values required to compute the SI measurement values from the
/// sensors raw ADC data. Based on BME280 datasheet "BST-BME280-DS001-23
/// Revision_1.23_012022", Table 16.
struct CalibrationData {
    uint16_t T1;
    int16_t T2;
    int16_t T3;

    uint16_t P1;
    int16_t P2;
    int16_t P3;
    int16_t P4;
    int16_t P5;
    int16_t P6;
    int16_t P7;
    int16_t P8;
    int16_t P9;

    uint8_t H1;
    int16_t H2;
    uint8_t H3;
    int16_t H4;
    int16_t H5;
    int8_t H6;
};

struct RawData {
    uint8_t pressure1;
    uint8_t pressure2;
    uint8_t pressure3;

    uint8_t humidity1;
    uint8_t humidity2;

    uint8_t temperature1;
    uint8_t temperature2;
    uint8_t temperature3;
};

enum class Error {
    /// Communication with the BME280 failed.
    /// Typically this would be caused because the bus
    /// connection to the sensor is broken and the sensor does not respond to
    /// communication attempts.
    ///
    /// Additionally, this error is also geneated if the BME does answer,
    /// but responds with unexpected data (for example incomplete calibration
    /// information).
    /// This should never happen if the BME behaves correctly.
    CommunicationFailed
};

using CommunicationResult = tl::expected<void, Error>;

struct Data {
    double temperature;
    double humidity;
    double pressure;
};

class Sensor {
public:
    Sensor(I2C::Bus& bus, Address address);

    CommunicationResult setConfiguration(Configuration const& config);

    CommunicationResult triggerSingleSample();

    tl::expected<Data, Error> readData();

private:
    CommunicationResult readRegisters(std::byte startAddress,
                                      std::span<std::byte> dataBuffer);
    CommunicationResult writeRegister(std::byte address, std::byte data);

    CommunicationResult readCalibrationData();

    I2C::Slave slave;
    std::optional<CalibrationData> calibrationData;
};

}  // namespace BME280
