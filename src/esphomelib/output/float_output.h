//
// Created by Otto Winter on 26.11.17.
//

#ifndef ESPHOMELIB_OUTPUT_FLOAT_OUTPUT_H
#define ESPHOMELIB_OUTPUT_FLOAT_OUTPUT_H

#include "esphomelib/output/binary_output.h"

#include "esphomelib/component.h"
#include "esphomelib/defines.h"

#ifdef USE_OUTPUT

ESPHOMELIB_NAMESPACE_BEGIN

namespace output {

template<typename T>
class SetLevelAction;

/** Base class for all output components that can output a variable level, like PWM.
 *
 * Floating Point Outputs always use output values in the range from 0.0 to 1.0 (inclusive), where 0.0 means off
 * and 1.0 means fully on. While using floating point numbers might make computation slower, it
 * makes using maths much easier and (in theory) supports all possible bit depths.
 *
 * If you want to create a FloatOutput yourself, you essentially just have to override write_state(float).
 * That method will be called for you with inversion and max power already applied. It is
 *
 * This interface is compatible with BinaryOutput (and will automatically convert the binary states to floating
 * point states for you). Additionally, this class provides a way for users to set a maximum power
 * output
 */
class FloatOutput : public BinaryOutput {
 public:
  // ===== OVERRIDE THIS =====
  /// Write a floating point state to hardware, inversion and max_power is already applied.
  virtual void write_state(float state) = 0;
  // ===== OVERRIDE THIS =====

  /** Set the maximum power output of this component.
   *
   * All values are multiplied by this float to get the adjusted value.
   *
   * @param max_power Automatically clamped from 0 to 1.
   */
  void set_max_power(float max_power);

  // ========== INTERNAL METHODS ==========
  // (In most use cases you won't need these)

  /// This is the method that is called internally by the MQTT component.
  void set_state_(float state);

  /// Override BinaryOutput's enable() and disable() so that we can convert it to float.
  void enable() override;
  /// Override BinaryOutput's enable() and disable() so that we can convert it to float.
  void disable() override;

  /// Get the maximum power output.
  float get_max_power() const;

  /// Implement BinarySensor's write_enabled; this should never be called.
  void write_enabled(bool value) override;

  template<typename T>
  SetLevelAction<T> *make_set_level_action();

 protected:
  float max_power_{1.0f};
};

template<typename T>
class SetLevelAction : public Action<T> {
 public:
  SetLevelAction(FloatOutput *output);

  void set_level(std::function<float(T)> &&level);
  void set_level(float level);
  void play(T x) override;

 protected:
  FloatOutput *output_;
  TemplatableValue<float, T> level_;
};

template<typename T>
SetLevelAction<T>::SetLevelAction(FloatOutput *output) : output_(output) {}

template<typename T>
void SetLevelAction<T>::set_level(std::function<float(T)> &&level) {
  this->level_ = std::move(level);
}
template<typename T>
void SetLevelAction<T>::set_level(float level) {
  this->level_ = level;
}
template<typename T>
void SetLevelAction<T>::play(T x) {
  this->output_->set_state_(this->level_.value(x));
  this->play_next(x);
}

template<typename T>
SetLevelAction<T> *FloatOutput::make_set_level_action() {
  return new SetLevelAction<T>(this);
}

} // namespace output

ESPHOMELIB_NAMESPACE_END

#endif //USE_OUTPUT

#endif //ESPHOMELIB_OUTPUT_FLOAT_OUTPUT_H
