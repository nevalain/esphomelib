//
//  max7219.h
//  esphomelib
//
//  Created by Otto Winter on 14.08.18.
//  Copyright © 2018 Otto Winter. All rights reserved.
//

#ifndef ESPHOMELIB_DISPLAY_MAX7219_H
#define ESPHOMELIB_DISPLAY_MAX7219_H

#include "esphomelib/defines.h"
#include "esphomelib/spi_component.h"
#include "esphomelib/time/rtc_component.h"
#include "esphomelib/sensor/sensor.h"

#ifdef USE_MAX7219

ESPHOMELIB_NAMESPACE_BEGIN

namespace display {

extern const uint8_t MAX7219_ASCII_TO_RAW[94] PROGMEM;

class MAX7219Component;

using max7219_writer_t = std::function<void(MAX7219Component &)>;

class MAX7219Component : public PollingComponent, public SPIDevice {
 public:
  MAX7219Component(SPIComponent *parent, GPIOPin *cs, uint32_t update_interval = 1000);

  void set_writer(max7219_writer_t &&writer);

  void setup() override;

  void update() override;

  float get_setup_priority() const override;

  void display();

  void set_intensity(uint8_t intensity);
  void set_num_chips(uint8_t num_chips);

  uint8_t printf(uint8_t pos, const char *format, ...);
  uint8_t printf(const char *format, ...);

  uint8_t print(uint8_t pos, const char *str);
  uint8_t print(uint8_t pos, const std::string &str);
  uint8_t print(const char *str);
  uint8_t print(const std::string &str);

#ifdef USE_TIME
  uint8_t strftime(uint8_t pos, const char *format, time::EsphomelibTime time);

  uint8_t strftime(const char *format, time::EsphomelibTime time);
#endif

 protected:
  void send_byte_(uint8_t register_, uint8_t data);
  void send_to_all_(uint8_t register_, uint8_t data);
  bool msb_first() override;

  uint8_t intensity_{15}; /// Intensity of the display from 0 to 15 (most)
  uint8_t num_chips_{1};
  uint8_t *buffer_;
  optional<max7219_writer_t> writer_{};
};

} // namespace display

ESPHOMELIB_NAMESPACE_END

#endif //USE_MAX7219

#endif //ESPHOMELIB_DISPLAY_MAX7219_H
