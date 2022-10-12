#include "widget_switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace switch_plate {

//static const char *const TAG = "Widget.switch";

void WidgetSwitch::dump_config() {
  LOG_SWITCH("", "Widget Switch", this);
  ESP_LOGCONFIG(TAG, "  Duration: %.1fs", this->duration_ / 1e3f);
}
void WidgetSwitch::setup() {
  bool initial_state = false;
  switch (this->restore_mode_) {
    case WIDGET_SWITCH_RESTORE_DEFAULT_OFF:
      initial_state = this->get_initial_state().value_or(false);
      break;
    case WIDGET_SWITCH_RESTORE_DEFAULT_ON:
      initial_state = this->get_initial_state().value_or(true);

      break;
    case WIDGET_SWITCH_RESTORE_INVERTED_DEFAULT_OFF:
      initial_state = !this->get_initial_state().value_or(true);
      break;
    case WIDGET_SWITCH_RESTORE_INVERTED_DEFAULT_ON:
      initial_state = !this->get_initial_state().value_or(false);
      break;
    case WIDGET_SWITCH_ALWAYS_OFF:
      initial_state = false;
      break;
    case WIDGET_SWITCH_ALWAYS_ON:
      initial_state = true;
      break;
    default:
      return;
  }

  if (initial_state) {
    this->turn_on();
  } else {
    this->turn_off();
  }
}
void WidgetSwitch::write_state(bool state) {
  this->widget_->set_status(Status::SELECTED_, state);
  this->publish_state(state);
  if (state && (duration_ > 0)) {
    // Use a named timeout so that it's automatically cancelled if button is pressed again before it's reset
    this->set_timeout("reset", this->duration_, [this]() {
      this->widget_->set_status(Status::SELECTED_, false);
      this->publish_state(false);
    });
  }
}

}  // namespace switch_plate
}  // namespace esphome
