//
//  sht3xd_component.h
//  esphomelib
//
//  Created by Otto Winter on 11.05.18.
//  Copyright © 2018 Otto Winter. All rights reserved.
//

#ifndef ESPHOMELIB_SENSOR_SHT3XD_COMPONENT_H
#define ESPHOMELIB_SENSOR_SHT3XD_COMPONENT_H

#include "esphomelib/sensor/sensor.h"
#include "esphomelib/i2c_component.h"
#include "esphomelib/defines.h"

#ifdef USE_SHT3XD

ESPHOMELIB_NAMESPACE_BEGIN

namespace sensor {

class SHT3XDTemperatureSensor;
class SHT3XDHumiditySensor;

/// This class implements support for the SHT3x-DIS family of temperature+humidity i2c sensors.
class SHT3XDComponent : public PollingComponent, public I2CDevice {
 public:
  SHT3XDComponent(I2CComponent *parent,
                  const std::string &temperature_name, const std::string &humidity_name,
                  uint8_t address = 0x44, uint32_t update_interval = 15000);

  // ========== INTERNAL METHODS ==========
  // (In most use cases you won't need these)
  SHT3XDTemperatureSensor *get_temperature_sensor() const;
  SHT3XDHumiditySensor *get_humidity_sensor() const;

  void setup() override;
  float get_setup_priority() const override;
  void update() override;

 protected:
  bool write_command(uint16_t command);
  bool read_data(uint16_t *data, uint8_t len);

  SHT3XDTemperatureSensor *temperature_sensor_;
  SHT3XDHumiditySensor *humidity_sensor_;
};

/// Helper class exposing an SHT3xD temperature sensor with a unique id.
class SHT3XDTemperatureSensor : public EmptyPollingParentSensor<1, ICON_EMPTY, UNIT_C, SHT3XDComponent> {
 public:
  SHT3XDTemperatureSensor(const std::string &name, SHT3XDComponent *parent);

  std::string unique_id() override;

 protected:
  friend SHT3XDComponent;

  std::string unique_id_;
};

/// Helper class exposing an SHT3xD humidity sensor with a unique id.
class SHT3XDHumiditySensor : public EmptyPollingParentSensor<1, ICON_WATER_PERCENT, UNIT_PERCENT, SHT3XDComponent> {
 public:
  SHT3XDHumiditySensor(const std::string &name, SHT3XDComponent *parent);

  std::string unique_id() override;

 protected:
  friend SHT3XDComponent;

  std::string unique_id_;
};

} // namespace sensor

ESPHOMELIB_NAMESPACE_END

#endif //USE_SHT3XD

#endif //ESPHOMELIB_SENSOR_SHT3XD_COMPONENT_H
