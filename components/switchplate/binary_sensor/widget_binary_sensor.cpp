#include "widget_binary_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace switch_plate {

//static const char *const TAG = "gpio.binary_sensor";

void WidgetBinarySensor::setup() {
}

void WidgetBinarySensor::dump_config() {
  LOG_BINARY_SENSOR("", "Widget Binary Sensor", this);
}


float WidgetBinarySensor::get_setup_priority() const { return setup_priority::HARDWARE; }

}  // namespace switch_plate
}  // namespace esphome
