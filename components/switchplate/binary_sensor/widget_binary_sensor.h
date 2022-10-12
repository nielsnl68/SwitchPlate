#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/switchplate/switchplate.h"

namespace esphome {
namespace switch_plate {

class WidgetBinarySensor : public WidgetBridge, public binary_sensor::BinarySensor, public Component {
 public:
  // ========== INTERNAL METHODS ==========
  // (In most use cases you won't need these)
  /// Setup pin
  void setup() override;
  void dump_config() override;
  /// Hardware priority
  float get_setup_priority() const override;
  /// Check sensor
};

}  // namespace switch_plate
}  // namespace esphome
