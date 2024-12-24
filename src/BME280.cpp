// Parts of this file are based on
// https://github.com/boschsensortec/BME280_SensorAPI/tree/master Hence the
// following license applies to this file:

// Copyright (c) 2020 Bosch Sensortec GmbH. All rights reserved.
// BSD-3-Clause
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.

// 2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.

// 3. Neither the name of the copyright holder nor the names of its
//     contributors may be used to endorse or promote products derived from
//     this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "heinerbloch/BME280.hpp"

#include <algorithm>
#include <bit>
#include <cstring>
#include <iterator>
#include <vector>

#include "heinerbloch/Utilities.hpp"

namespace BME280 {

I2C::Address encode(Address address) {
  switch (address) {
    case Address::Low:
      return I2C::Address{0x76};
    case Address::High:
      return I2C::Address{0x77};
    default:
      throw std::runtime_error{"Invalid enum value for BME280::Address."};
  }
}

std::byte encode(Mode mode) {
  switch (mode) {
    case Mode::Sleep:
      return std::byte{0b00};
    case Mode::Forced:
      return std::byte{0b01};
    case Mode::Normal:
      return std::byte{0b11};
    default:
      throw std::runtime_error{"Invalid enum value for BME280::Mode."};
  }
}

std::byte encode(Oversampling sampling) {
  switch (sampling) {
    case Oversampling::Skipped:
      return std::byte{0b000};
    case Oversampling::OneSample:
      return std::byte{0b001};
    case Oversampling::TwoSamples:
      return std::byte{0b010};
    case Oversampling::FourSamples:
      return std::byte{0b011};
    case Oversampling::EightSamples:
      return std::byte{0b100};
    case Oversampling::SixteenSamples:
      return std::byte{0b101};
    default:
      throw std::runtime_error{"Invalid enum value for BME280::Oversampling."};
  }
}

EncodedConfiguration encode(Configuration const& configuration) {
  // This method uses the register field naming from the datashet.

  auto ctrl_meas = std::byte{0x00};
  auto ctrl_hum = std::byte{0x00};

  auto const osrs_h =
      ByteModification{0, 2, encode(configuration.humidityOversampling)};
  ctrl_hum = osrs_h.apply(ctrl_hum);

  auto const mode = ByteModification{0, 2, encode(configuration.mode)};
  ctrl_meas = mode.apply(ctrl_meas);

  auto const osrs_p =
      ByteModification{2, 3, encode(configuration.pressureOversampling)};
  ctrl_meas = osrs_p.apply(ctrl_meas);

  auto const osrs_t =
      ByteModification{5, 3, encode(configuration.temperatureOversampling)};
  ctrl_meas = osrs_t.apply(ctrl_meas);

  return EncodedConfiguration{.ctrl_meas = ctrl_meas, .ctrl_hum = ctrl_hum};
}

Sensor::Sensor(I2C::Bus& bus, Address address) : slave{bus, encode(address)} {}

CommunicationResult Sensor::readRegisters(std::byte startAddress,
                                          std::span<std::byte> dataBuffer) {
  // send the start address to the slave
  auto const writeResult = slave.write(std::array{startAddress});
  if (not writeResult.has_value() || *writeResult != 1) {
    return tl::make_unexpected(Error::CommunicationFailed);
  }

  auto const readResult = slave.read(dataBuffer);
  if (not readResult.has_value() || *readResult != dataBuffer.size()) {
    return tl::make_unexpected(Error::CommunicationFailed);
  }

  return {};
}

CommunicationResult Sensor::writeRegister(std::byte address, std::byte data) {
  auto const result = slave.write(std::array{address, data});
  if (not result.has_value() || *result != 2) {
    return tl::make_unexpected(Error::CommunicationFailed);
  }

  return {};
}

CommunicationResult Sensor::setConfiguration(Configuration const& config) {
  auto const encodedConfig = encode(config);

  auto const ctrl_hum_address = std::byte{0xF2};
  auto const ctrl_meas_address = std::byte{0xF4};
  {
    auto const result = writeRegister(ctrl_hum_address, encodedConfig.ctrl_hum);
    if (not result) {
      return tl::make_unexpected(result.error());
    }
  }

  {
    auto const result =
        writeRegister(ctrl_meas_address, encodedConfig.ctrl_meas);
    if (not result) {
      return tl::make_unexpected(result.error());
    }
  }

  return {};
}

template <typename T>
inline T decode(std::span<std::byte const> data, std::endian dataEndianness) {
  T value;
  std::memcpy(&value, data.data(), sizeof(value));

  if (std::endian::native != dataEndianness) {
    value = byteswap(value);
  }

  return value;
}

template <typename T>
void decodeInto(std::span<std::byte const> data,
                std::endian dataEndianness,
                T& destination) {
  destination = decode<T>(data, dataEndianness);
}

CommunicationResult Sensor::readCalibrationData() {
  if (calibrationData.has_value()) {
    // The calibration data on the device never changes.
    // If we have read it before, there is no point in reading it again.
    return {};
  }

  std::array<std::byte, 25> firstCalibrationDataSectionBuffer;
  std::array<std::byte, 8> secondCalibrationDataSectionBuffer;
  auto const firstCalibrationDataSection =
      std::span{firstCalibrationDataSectionBuffer};
  auto const secondCalibrationDataSection =
      std::span{secondCalibrationDataSectionBuffer};

  {
    auto const result =
        readRegisters(std::byte{0x88}, firstCalibrationDataSection);
    if (not result.has_value()) {
      return tl::make_unexpected(Error::CommunicationFailed);
    }
  }
  {
    auto const result =
        readRegisters(std::byte{0xE1}, secondCalibrationDataSection);
    if (not result.has_value()) {
      return tl::make_unexpected(Error::CommunicationFailed);
    }
  }

  calibrationData.emplace();

  decodeInto(firstCalibrationDataSection.subspan(0), std::endian::little,
             calibrationData->T1);
  decodeInto(firstCalibrationDataSection.subspan(2), std::endian::little,
             calibrationData->T2);
  decodeInto(firstCalibrationDataSection.subspan(4), std::endian::little,
             calibrationData->T3);

  decodeInto(firstCalibrationDataSection.subspan(6), std::endian::little,
             calibrationData->P1);
  decodeInto(firstCalibrationDataSection.subspan(8), std::endian::little,
             calibrationData->P2);
  decodeInto(firstCalibrationDataSection.subspan(10), std::endian::little,
             calibrationData->P3);
  decodeInto(firstCalibrationDataSection.subspan(12), std::endian::little,
             calibrationData->P4);
  decodeInto(firstCalibrationDataSection.subspan(14), std::endian::little,
             calibrationData->P5);
  decodeInto(firstCalibrationDataSection.subspan(16), std::endian::little,
             calibrationData->P6);
  decodeInto(firstCalibrationDataSection.subspan(18), std::endian::little,
             calibrationData->P7);
  decodeInto(firstCalibrationDataSection.subspan(20), std::endian::little,
             calibrationData->P8);
  decodeInto(firstCalibrationDataSection.subspan(22), std::endian::little,
             calibrationData->P9);

  decodeInto(firstCalibrationDataSection.subspan(24), std::endian::little,
             calibrationData->H1);
  decodeInto(secondCalibrationDataSection.subspan(0), std::endian::little,
             calibrationData->H2);
  decodeInto(secondCalibrationDataSection.subspan(2), std::endian::little,
             calibrationData->H3);

  // For reasons unbeknownst to mankind the H4 value is big endien and
  // horrifically sliced.
  auto const h4_msb = static_cast<int16_t>(
      std::to_integer<int16_t>(secondCalibrationDataSection[3]) << 4);
  auto const h4_lsb = std::to_integer<int16_t>(secondCalibrationDataSection[4] &
                                               std::byte{0x0F});
  calibrationData->H4 = h4_msb | h4_lsb;

  // H5 wants to be yet anotherkind of special and is big endian again, but also
  // sliced.
  decodeInto(secondCalibrationDataSection.subspan(4), std::endian::little,
             calibrationData->H5);
  calibrationData->H5 >>= 4;

  decodeInto(secondCalibrationDataSection.subspan(6), std::endian::little,
             calibrationData->H6);

  return {};
}

CommunicationResult Sensor::triggerSingleSample() {
  return setConfiguration(
      Configuration{.temperatureOversampling = Oversampling::OneSample,
                    .pressureOversampling = Oversampling::OneSample,
                    .humidityOversampling = Oversampling::OneSample,
                    .mode = Mode::Forced});
}

tl::expected<Data, Error> Sensor::readData() {
  {
    auto const calibrationDataResult = readCalibrationData();
    if (not calibrationDataResult.has_value()) {
      return tl::make_unexpected(calibrationDataResult.error());
    }
  }

  auto const measurementDataAddress = std::byte{0xF7};
  std::array<std::byte, 8> dataBuffer;
  auto const result = readRegisters(measurementDataAddress, dataBuffer);
  if (not result.has_value()) {
    return tl::make_unexpected(result.error());
  }

  auto const data = std::span{dataBuffer};

  uint32_t uncompensatedPressure;
  decodeInto(data.subspan(0), std::endian::big, uncompensatedPressure);
  uncompensatedPressure >>= 12;

  uint32_t uncompensatedTemperature;
  decodeInto(data.subspan(3), std::endian::big, uncompensatedTemperature);
  uncompensatedTemperature >>= 12;

  uint16_t uncompensatedHumidity;
  decodeInto(data.subspan(6), std::endian::big, uncompensatedHumidity);

  // While the BME280 datasheet an reference implementation are a disgusting
  // piece of garbage, cleaning up the following cluster fuck of compensation
  // math would be an endless waste of time without proper documentation of the
  // underlying math.
  // The code was only modified to fix linter offenses.
  int32_t temperature;
  uint32_t humidity;
  uint32_t pressure;

  int32_t t_fine;
  {
    int32_t var1;
    int32_t var2;
    int32_t temperature_min = -4000;
    int32_t temperature_max = 8500;

    var1 =
        (int32_t)((uncompensatedTemperature / 8) - (calibrationData->T1 * 2));
    var1 = (var1 * ((int32_t)calibrationData->T2)) / 2048;
    var2 = (int32_t)(uncompensatedTemperature / 16 - calibrationData->T1);
    var2 = (((var2 * var2) / 4096) * ((int32_t)calibrationData->T3)) / 16384;
    t_fine = var1 + var2;
    temperature = (t_fine * 5 + 128) / 256;

    if (temperature < temperature_min) {
      temperature = temperature_min;
    } else if (temperature > temperature_max) {
      temperature = temperature_max;
    }
  }
  {
    int32_t var1;
    int32_t var2;
    int32_t var3;
    int32_t var4;
    int32_t var5;
    uint32_t humidity_max = 102400;

    var1 = t_fine - ((int32_t)76800);
    var2 = (int32_t)(uncompensatedHumidity * 16384);
    var3 = (int32_t)(((int32_t)calibrationData->H4) * 1048576);
    var4 = ((int32_t)calibrationData->H5) * var1;
    var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
    var2 = (var1 * ((int32_t)calibrationData->H6)) / 1024;
    var3 = (var1 * ((int32_t)calibrationData->H3)) / 2048;
    var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
    var2 = ((var4 * ((int32_t)calibrationData->H2)) + 8192) / 16384;
    var3 = var5 * var2;
    var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
    var5 = var3 - ((var4 * ((int32_t)calibrationData->H1)) / 16);
    var5 = (var5 < 0 ? 0 : var5);
    var5 = (var5 > 419430400 ? 419430400 : var5);
    humidity = (uint32_t)(var5 / 4096);

    if (humidity > humidity_max) {
      humidity = humidity_max;
    }
  }
  {
    int64_t var1;
    int64_t var2;
    int64_t var3;
    int64_t var4;
    uint32_t pressure_min = 3000000;
    uint32_t pressure_max = 11000000;

    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)calibrationData->P6;
    var2 = var2 + ((var1 * (int64_t)calibrationData->P5) * 131072);
    var2 = var2 + (((int64_t)calibrationData->P4) * 34359738368);
    var1 = ((var1 * var1 * (int64_t)calibrationData->P3) / 256) +
           ((var1 * ((int64_t)calibrationData->P2) * 4096));
    var3 = ((int64_t)1) * 140737488355328;
    var1 = (var3 + var1) * ((int64_t)calibrationData->P1) / 8589934592;

    /* To avoid divide by zero exception */
    if (var1 != 0) {
      var4 = 1048576 - uncompensatedPressure;
      var4 = (((var4 * INT64_C(2147483648)) - var2) * 3125) / var1;
      var1 = (((int64_t)calibrationData->P9) * (var4 / 8192) * (var4 / 8192)) /
             33554432;
      var2 = (((int64_t)calibrationData->P8) * var4) / 524288;
      var4 =
          ((var4 + var1 + var2) / 256) + (((int64_t)calibrationData->P7) * 16);
      pressure = (uint32_t)(((var4 / 2) * 100) / 128);

      if (pressure < pressure_min) {
        pressure = pressure_min;
      } else if (pressure > pressure_max) {
        pressure = pressure_max;
      }
    } else {
      pressure = pressure_min;
    }
  }

  return {{.temperature = double(temperature) / 100,
           .humidity = double(humidity) / 1024,
           .pressure = double(pressure) / 100}};
}

}  // namespace BME280
