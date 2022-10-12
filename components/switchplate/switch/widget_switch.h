#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/switchplate/switchplate.h"

namespace esphome {
namespace switch_plate {

enum WidgetSwitchRestoreMode {
  WIDGET_SWITCH_RESTORE_DEFAULT_OFF,
  WIDGET_SWITCH_RESTORE_DEFAULT_ON,
  WIDGET_SWITCH_ALWAYS_OFF,
  WIDGET_SWITCH_ALWAYS_ON,
  WIDGET_SWITCH_RESTORE_INVERTED_DEFAULT_OFF,
  WIDGET_SWITCH_RESTORE_INVERTED_DEFAULT_ON,
  WIDGET_SWITCH_RESTORE_FROM_SERVER,
};

class WidgetSwitch : public WidgetBridge, public switch_::Switch, public Component {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE - 1.0f; }

  void set_restore_mode(WidgetSwitchRestoreMode restore_mode) { restore_mode_ = restore_mode; }
  void set_duration(uint32_t duration) { duration_ = duration; }

 protected:
  void write_state(bool state) override;
  WidgetSwitchRestoreMode restore_mode_{WIDGET_SWITCH_RESTORE_FROM_SERVER};

  uint32_t duration_{0};
};

}  // namespace switch_plate
}  // namespace esphome
