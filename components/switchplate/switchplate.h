#pragma once

#include "esphome/components/display/display_buffer.h"
#include "esphome/components/touchscreen/touchscreen.h"

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/automation.h"
#include "esphome/core/log.h"
#include <map>

#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif
#ifdef USE_BINARY_SENSOR
#include "esphome/components/binary_sensor/binary_sensor.h"
#endif
#ifdef USE_TEXT_SENSOR
#include "esphome/components/text_sensor/text_sensor.h"
#endif

namespace esphome {
using display::DisplayBuffer;
using display::Rect;
using display::Image;
using display::Font;
using display::GradientDirection;
using display::TextAlign;
using touchscreen::TouchPoint;
using touchscreen::TouchListener;

#define CONF_WIDGET_STATUS_PRESSED "_pressed"
#define CONF_WIDGET_STATUS_SELECTED "_selected"
#define CONF_WIDGET_STATUS_DISABLED "_disabled"
#define CONST_TOUCH_TO_MOVE 6  // NOLINT

namespace switch_plate {

const static char *TAG = "SwitchPlate";

enum Align {
  TOP = 0x00,
  CENTER_VERTICAL = 0x01,
  BASELINE = 0x02,
  BOTTOM = 0x04,

  LEFT = 0x00,
  CENTER_HORIZONTAL = 0x08,
  RIGHT = 0x10,

  TOP_LEFT = TOP | LEFT,
  TOP_CENTER = TOP | CENTER_HORIZONTAL,
  TOP_RIGHT = TOP | RIGHT,

  CENTER_LEFT = CENTER_VERTICAL | LEFT,
  CENTER = CENTER_VERTICAL | CENTER_HORIZONTAL,
  CENTER_RIGHT = CENTER_VERTICAL | RIGHT,

  BASELINE_LEFT = BASELINE | LEFT,
  BASELINE_CENTER = BASELINE | CENTER_HORIZONTAL,
  BASELINE_RIGHT = BASELINE | RIGHT,

  BOTTOM_LEFT = BOTTOM | LEFT,
  BOTTOM_CENTER = BOTTOM | CENTER_HORIZONTAL,
  BOTTOM_RIGHT = BOTTOM | RIGHT,

  GROUP_ALIGN_RELATIVE = 0x80,
  GROUP_ALIGN_ABSOLUTE = 0x81,
  GROUP_ALIGN_HORZONTAL = 0x82,
  GROUP_ALIGN_VERTICAL = 0x83,
};

enum Mode {
  TEXT_MODE_EXPAND = 0,
  TEXT_MODE_BREAK = 1,
  TEXT_MODE_DOTS = 2,
  TEXT_MODE_SCROLL = 3,
  TEXT_MODE_LOOP = 4,
  TEXT_MODE_CROP = 5,
  SPINNER_MODE_CONSTANT = 10,
  SPINNER_MODE_SLOWDOWNUP = 11,
  SPINNER_MODE_SLOWSTRATCH = 12,
};

enum Direction {
  DIRECTION_DOWN = 0,
  DIRECTION_UP = 1,
  DIRECTION_LEFT = 2,
  DIRECTION_RIGHT = 3,

  DIRECTION_CW = 10,
  DIRECTION_CCW = 11,

  GRADIENT_NONE = 20,
  GRADIENT_HORIZONTAL = 21,
  GRADIENT_VERTICAL = 22,
  GRADIENT_BOTH = 23,
};

class SwitchPlateBase;
class SwitchPlateGroup;
class SwitchPlatePage;
class SwitchPlate;
class SwitchPlateStyle;
class SwitchPlateItem;
class SwitchPlateStyle;

class DisplayOnPageChangeTrigger;

//struct Style: uint32_t {
const uint32_t STYLE_DEFAULT = 0x0000;

const uint32_t STYLE_BACKGROUND = 0x0001;
const uint32_t STYLE_BORDER = 0x0002;
const uint32_t STYLE_TEXT = 0x0003;
const uint32_t STYLE_HEADER = 0x0004;
const uint32_t STYLE_FOOTER = 0x0005;

const uint32_t STYLE_COLOR = 0x0100;
const uint32_t STYLE_GRADIENT_TO = 0x0200;
const uint32_t STYLE_GRADIENT_FROM = 0x0300;
const uint32_t STYLE_GRADIENT_DIRECTION = 0x0400;
const uint32_t STYLE_FONT = 0x0500;
const uint32_t STYLE_ALIGN = 0x0600;
const uint32_t STYLE_MODE = 0x0700;
const uint32_t STYLE_IMAGE = 0x0800;
const uint32_t STYLE_RADIUS = 0X0900;

const uint32_t STYLE_PRESSED = 0x10000;
const uint32_t STYLE_SELECTED = 0x20000;
const uint32_t STYLE_DISABLED = 0x40000;

const uint32_t STYLE_BASE_SWITCHPLATE = 0x100000;
const uint32_t STYLE_WIDGET_LABEL    = 0x200000;
const uint32_t STYLE_WIDGET_CLOCK    = 0x300000;
const uint32_t STYLE_WIDGET_BUTTON   = 0x400000;
const uint32_t STYLE_WIDGET_PAGE     = 0x500000;
const uint32_t STYLE_WIDGET_PANEL    = 0x600000;

const uint32_t STYLE_BACKGROUND_COLOR = STYLE_BACKGROUND | STYLE_COLOR;
const uint32_t STYLE_BACKGROUND_GRADIENT_TO = STYLE_BACKGROUND | STYLE_GRADIENT_TO;
const uint32_t STYLE_BACKGROUND_GRADIENT_FROM = STYLE_BACKGROUND | STYLE_GRADIENT_FROM;
const uint32_t STYLE_BACKGROUND_GRADIENT_DIRECTION = STYLE_BACKGROUND | STYLE_COLOR;
const uint32_t STYLE_BACKGROUND_IMAGE = STYLE_BACKGROUND | STYLE_IMAGE;

const uint32_t STYLE_BORDER_COLOR = STYLE_BORDER | STYLE_COLOR;
const uint32_t STYLE_BORDER_GRADIENT_TO = STYLE_BORDER | STYLE_GRADIENT_TO;
const uint32_t STYLE_BORDER_GRADIENT_FROM = STYLE_BORDER | STYLE_GRADIENT_FROM;
const uint32_t STYLE_BORDER_GRADIENT_DIRECTION = STYLE_BORDER | STYLE_GRADIENT_DIRECTION;
const uint32_t STYLE_BORDER_RADIUS = STYLE_BORDER | STYLE_RADIUS;

const uint32_t STYLE_TEXT_COLOR = STYLE_TEXT | STYLE_COLOR;
const uint32_t STYLE_TEXT_GRADIENT_TO = STYLE_TEXT | STYLE_GRADIENT_TO;
const uint32_t STYLE_TEXT_GRADIENT_FROM = STYLE_TEXT | STYLE_GRADIENT_FROM;
const uint32_t STYLE_TEXT_GRADIENT_DIRECTION = STYLE_TEXT | STYLE_GRADIENT_DIRECTION;
const uint32_t STYLE_TEXT_FONT = STYLE_TEXT | STYLE_FONT;
const uint32_t STYLE_TEXT_ALIGN = STYLE_TEXT | STYLE_ALIGN;



struct TaggedVariable {
  union {
    uint32_t uint32_;
    int32_t int32_;
    float fload_;
    bool bool_;

    Align align_;
    Mode mode_;
    Direction direction_;
    Font *font_;
    Image *image_;
    Component *component_;
  };
  Color color_;
};
struct WidgetStatus {
  union {
    struct {
      uint8_t pressed : 1;
      uint8_t selected : 1;
      uint8_t disabled : 1;
      uint8_t dummystatus : 4;
      uint8_t invalidate : 1;

      uint8_t visible : 1;
      uint8_t clickable : 1;
      uint8_t selectable : 1;
      uint8_t editable : 2;
      uint8_t draggable : 1;
      uint8_t droppable : 1;
      uint8_t dummyoptions : 1;
    };
    uint16_t raw;
  };
  inline WidgetStatus() ALWAYS_INLINE : raw(1UL << 8) {}  // NOLINT
};

enum class SwitchPlateTouchState : uint8_t {
  Released = 0,
  Pressed = 1,
  Holding = 2,
  Moving = 3,
  Dragging = 4,
  Dropping = 5,
  Ignore = 128
};
struct SwitchPlateTouch {
  uint16_t x;
  uint16_t y;
  uint16_t origin_x;
  uint16_t origin_y;
  SwitchPlateTouchState state;
  SwitchPlateBase *origin;
  SwitchPlateBase *destiny;
  uint32_t start_now;
  inline SwitchPlateTouch() ALWAYS_INLINE : x(0),
                                            y(0),
                                            origin_x(0),
                                            origin_y(0),
                                            state(SwitchPlateTouchState::Released),
                                            origin(nullptr),
                                            destiny(nullptr),
                                            start_now(0) {}  // NOLINT
};

// ============================================================================== SwitchPlateVars

class SwitchPlateVars : public Parented<SwitchPlateBase> {
 public:
  SwitchPlateVars(){};

  void set_variable(std::string key, uint32_t var, bool as_missing = false) {
    if (as_missing && has_variable(key))
      return;  // NOLINT
    ESP_LOGW("", "- Set uint %s to %d ", key.c_str(), var);
    this->vars_[key].uint32_ = var;
    this->set_redraw();
  }
  uint32_t get_uint(std::string key, bool search_parent = false) {
    return this->get_variable(key, search_parent).uint32_;
  }

  void set_variable(std::string key, int32_t var, bool as_missing = false) {
    if (as_missing && has_variable(key))
      return;  // NOLINT
    ESP_LOGW("", "- Set int %s to %d", key.c_str(), var);
    this->vars_[key].int32_ = var;
    this->set_redraw();
  }
  int32_t get_int(std::string key, bool search_parent = false) { return this->get_variable(key, search_parent).int32_; }

  void set_variable(std::string key, float var, bool as_missing = false) {
    if (as_missing && has_variable(key))
      return;  // NOLINT
    ESP_LOGW("", "- Set float %s to %f", key.c_str(), var);
    this->vars_[key].fload_ = var;
    this->set_redraw();
  }
  float get_float(std::string key, bool search_parent = false) { return this->get_variable(key, search_parent).fload_; }

  void set_variable(std::string key, Mode var, bool as_missing = false) {
    if (as_missing && has_variable(key))
      return;  // NOLINT
    ESP_LOGW("", "- Set mode %s to %d", key.c_str(), (uint8_t) var);

    this->vars_[key].mode_ = var;
    this->set_redraw();
  }
  Mode get_mode(std::string key, bool search_parent = false) { return this->get_variable(key, search_parent).mode_; }

  void set_variable(std::string key, Align var, bool as_missing = false) {
    if (as_missing && has_variable(key))
      return;  // NOLINT
    ESP_LOGW("", "- Set align %s to %d", key.c_str(), (uint8_t) var);

    this->vars_[key].align_ = var;
    this->set_redraw();
  }
  Align get_align(std::string key, bool search_parent = false) { return this->get_variable(key, search_parent).align_; }

  void set_variable(std::string key, Direction var, bool as_missing = false) {
    if (as_missing && has_variable(key))
      return;  // NOLINT
    ESP_LOGW("", "- Set direction %s to %d", key.c_str(), (uint8_t) var);

    this->vars_[key].direction_ = var;
    this->set_redraw();
  }
  Direction get_direction(std::string key, bool search_parent = false) {
    return this->get_variable(key, search_parent).direction_;
  }

  void set_variable(std::string key, Color var, bool as_missing = false) {
    if (as_missing && has_variable(key))
      return;  // NOLINT
    ESP_LOGW("", "- Set Color %s to #%08x", key.c_str(), var.raw_32);

    this->vars_[key].uint32_ = var.raw_32;
    this->set_redraw();
  }
  Color get_color(std::string key, bool search_parent = false) {
    return this->get_variable(key, search_parent).color_;
  }

  void set_variable(std::string key, Font *var, bool as_missing = false) {
    if (as_missing && has_variable(key))
      return;  // NOLINT
    ESP_LOGW("", "- Set Font %s to %p", key.c_str(), var);
    this->vars_[key].font_ = var;
    this->set_redraw();
  }
  Font *get_font(std::string key, bool search_parent = false) { return this->get_variable(key, search_parent).font_; }

  void set_variable(std::string key, Image *var, bool as_missing = false) {
    if (as_missing && has_variable(key))
      return;  // NOLINT
    ESP_LOGW("", "- Set Image %s to %p", key.c_str(), var);
    this->vars_[key].image_ = var;
    this->set_redraw();
  }
  Image *get_image(std::string key, bool search_parent = false) {
    return this->get_variable(key, search_parent).image_;
  }

  void set_variable(std::string key, Component *var, bool as_missing = false) {
    if (as_missing && has_variable(key))
      return;  // NOLINT
    ESP_LOGW("", "- Set Image %s to %p", key.c_str(), var);
    this->vars_[key].component_ = var;
    this->set_redraw();
  }
  void set_variable(std::string key, bool var, bool as_missing = false) {
    if (as_missing && has_variable(key))
      return;  // NOLINT
    ESP_LOGW("", "- Set bool %s to %s", key.c_str(), var ? "true" : "false");

    this->vars_[key].bool_ = var;
    this->set_redraw();
  }

  TaggedVariable get_variable(std::string key, bool search_parent = false);

  bool has_variable(std::string key, bool search_parent = false);

  virtual void set_redraw() = 0;

 protected:
  std::map<std::string, TaggedVariable> vars_{};
};

// ============================================================================== SwitchPlateStyle

class SwitchPlateStyle: public Parented<SwitchPlateBase> {
 public:
  SwitchPlateStyle(){};
  void set(uint32_t key, uint32_t var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    this->set_redraw();
  }

  void set(uint32_t key, int32_t var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    this->styles_[key].int32_ = var;
    this->set_redraw();
  }

  void set(uint32_t key, float var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    this->styles_[key].fload_ = var;
    this->set_redraw();
  }

  void set(uint32_t key, Mode var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    this->styles_[key].mode_ = var;
    this->set_redraw();
  }

  void set(uint32_t key, Align var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    this->styles_[key].align_ = var;
    this->set_redraw();
  }

  void set(uint32_t key, Direction var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    this->styles_[key].direction_ = var;
    this->set_redraw();
  }

  void set(uint32_t key, Color var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    this->styles_[key].color_ = var;
    this->set_redraw();
  }

  void set(uint32_t key, Font *var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    this->styles_[key].font_ = var;
    this->set_redraw();
  }

  void set(uint32_t key, Image *var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    this->styles_[key].image_ = var;
    this->set_redraw();
  }

  void set(uint32_t key, Component *var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    this->styles_[key].component_ = var;
    this->set_redraw();
  }

  void set(uint32_t key, bool var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    this->styles_[key].bool_ = var;
    this->set_redraw();
  }
  TaggedVariable get(uint32_t key) {
    return this->styles_[key];
  }
  bool has(uint32_t key) {
    return this->styles_.count(key) != 0;
  }

  void set_redraw();

 protected:
  std::map<uint32_t, TaggedVariable> styles_{};
};

// ============================================================================== SwitchPlateBase

class SwitchPlateBase : public SwitchPlateVars {
 public:
  SwitchPlateBase() { style_.set_parent(this); };
  virtual uint32_t get_style_class_id() { return STYLE_DEFAULT; }

  virtual void setup(){};
  virtual void call_setup() { this->setup(); };

  virtual void show(){};
  virtual void call_show() { this->show(); };

  virtual int16_t x() { return 0; }
  virtual int16_t y() { return 0; }
  virtual int16_t width() { return this->screen_width(); }
  virtual int16_t height() { return this->screen_height(); }

  virtual int16_t screen_width() { return 32666; }

  virtual int16_t screen_height() { return 32666; }

  virtual int16_t calc_x() { return x(); }

  virtual int16_t calc_y() { return y(); }
  virtual bool visible() { return true; }
  virtual bool enabled() { return true; }

  virtual DisplayBuffer &display() {
    if ((display_ == nullptr) && (parent_ != nullptr)) {
      display_ = parent_->display_;
    }
    return (*display_);
  };
  virtual SwitchPlateBase *check_touch(SwitchPlateTouch tp, Rect parent) { return nullptr; }

  uint32_t find_key(uint32_t key) {
    uint32_t result = 0;
    uint32_t themed = get_style_class_id();
    uint32_t pressed = STYLE_PRESSED;
    uint32_t selected = STYLE_SELECTED;
    uint32_t disabled = STYLE_DISABLED;

    if (((status_.raw & 6) == 6) && style_.has(result = (key | disabled | selected))) { return result; }
    if (((status_.raw & 6) == 6) && thema().has(result = (key | themed | disabled | selected))) { return result; }

    if ((status_.disabled) && style_.has(result = (key | disabled))) { return result; }
    if ((status_.disabled) && thema().has(result = (themed | key | disabled))) { return result; }

    if (((status_.raw & 3) == 3) && style_.has(result = (key | pressed | selected))) { return result; }
    if (((status_.raw & 3) == 3) && thema().has(result = (themed | key | pressed | selected))) { return result; }

    if ((status_.pressed) && style_.has(result = (key | pressed))) { return result; }
    if ((status_.pressed) && thema().has(result = (themed | key | pressed))) { return result; }

    if ((status_.selected) && style_.has(result = (key | selected))) { return result; }
    if ((status_.selected) && thema().has(result = (themed | key | selected))) { return result; }

    if (style_.has(result = key)) { return result; }
    if (thema().has(result = (themed | key))) { return result; }
    return STYLE_DEFAULT;
  }


  virtual TaggedVariable get_style(uint32_t key) {
    key = find_key(key);
    if ((uint32_t)key < (uint32_t) STYLE_BASE_SWITCHPLATE) {
      return style_.get(key);
    } else {
      return thema().get(key);
    }
  };

  virtual bool has_style(uint32_t key) {
    return find_key(key) !=  STYLE_DEFAULT;
  };

  static SwitchPlateStyle thema() {
    static SwitchPlateStyle theme_ ;
    return theme_;
  }

 protected:
  display::DisplayBuffer *display_{nullptr};
  SwitchPlateStyle style_{};

  WidgetStatus status_{};
};

// ============================================================================== SwitchPlate

class SwitchPlate : public Component, public SwitchPlateBase, public TouchListener {

 public:
  uint32_t get_style_class_id() { return STYLE_BASE_SWITCHPLATE; }
#ifdef USE_SENSOR
  void register_sensor(sensor::Sensor *obj) { this->sensors_.push_back(obj); }
#endif
#ifdef USE_BINARY_SENSOR
  void register_text_sensor(text_sensor::TextSensor *obj) { this->text_sensors_.push_back(obj); }
#endif
#ifdef USE_TEXT_SENSOR
  void register_binary_sensor(binary_sensor::BinarySensor *obj) { this->binary_sensors_.push_back(obj); }
#endif
  void set_display(display::DisplayBuffer *display) {
    this->display_ = display;
    display->set_writer([this](display::DisplayBuffer &disp_buf) {
      ESP_LOGVV("SP_display", "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
      this->show();
      ESP_LOGVV("SP_display", "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
    });
  }

  void set_touchscreen(touchscreen::Touchscreen *touchscreen) {
    this->touchscreen_ = touchscreen;
    this->set_display(touchscreen->get_display());
    touchscreen->register_listener(this);
  }

  void add_page(SwitchPlatePage *page);
  void add_headerItem(SwitchPlateItem *item);
  void add_footerItem(SwitchPlateItem *item);

  /// Get the width of the image in pixels with rotation applied.
  int16_t screen_width() { return this->display().get_width(); }

  /// Get the height of the image in pixels with rotation applied.
  int16_t screen_height() { return this->display().get_height(); };

  void show_page(SwitchPlatePage *page);

  const SwitchPlatePage *current_page() const { return this->current_page_; }

  void show_next();
  SwitchPlatePage *get_next();
  bool can_next();

  void show_prev();
  SwitchPlatePage *get_prev();
  bool can_prev();

  void show_home() { this->show_page(this->first_page_); }

  void add_on_page_change_trigger(DisplayOnPageChangeTrigger *t) { this->on_page_change_triggers_.push_back(t); }

  void setup() override;
  virtual void call_setup();

  void dump_config() override;
  void show() override;

  float get_setup_priority() const override { return esphome::setup_priority::HARDWARE; }

  void set_redraw() {} // TODO: check of we need to do display()->call_update() here.

  void touch(TouchPoint tp);

  void release() {
    TouchPoint tp;
    tp.x = 32666;
    tp.y = 32666;
    this->touch(tp);
  }

  void register_listener(TouchListener *listener) {
    if (this->touchscreen_ != nullptr) {
      this->touchscreen_->register_listener(listener);
    }
  }

 protected:
  /// High Frequency loop() requester used during sampling phase.
  // HighFrequencyLoopRequester high_freq_;

  touchscreen::Touchscreen *touchscreen_{nullptr};

  std::vector<SwitchPlateItem *> header_;
  std::vector<SwitchPlateItem *> footer_;

  bool tabview_ = false;

  SwitchPlatePage *first_page_{nullptr};
  SwitchPlatePage *current_page_{nullptr};
  SwitchPlatePage *previous_page_{nullptr};

  SwitchPlateTouch touch_info_ = SwitchPlateTouch();

#ifdef USE_SENSOR
  std::vector<sensor::Sensor *> sensors_;
#endif
#ifdef USE_BINARY_SENSOR
  std::vector<text_sensor::TextSensor *> text_sensors_;
#endif
#ifdef USE_TEXT_SENSOR
  std::vector<binary_sensor::BinarySensor *> binary_sensors_;
#endif

  std::vector<DisplayOnPageChangeTrigger *> on_page_change_triggers_;
};

// ============================================================================== SwitchPlateItem

class SwitchPlateItem : public SwitchPlateBase {
 public:
  SwitchPlateItem(){};

  SwitchPlateBase *check_touch(SwitchPlateTouch tp, Rect parent) {
    if (this->status_.visible && !this->status_.disabled) {
      Rect r = this->get_boundry();
      r.substract(parent);
      bool touched = r.inside(tp.x, tp.y);
      if (touched) {
        ESP_LOGD("SwitchPlate", "    =====> L touched: %s, clickable: %s", YESNO(touched),
                 YESNO(this->status_.clickable));
      } else {
        ESP_LOGE("SwitchPlate", "    =====> L touched: %s", YESNO(touched));
      }
      if ((tp.state == SwitchPlateTouchState::Released)) {
        if (this->status_.pressed) {
          this->status_.pressed = 0;
          this->set_redraw();
          display().call_update();
        }
        if (this->status_.droppable && (tp.origin != this) && touched) {
          ESP_LOGW("SwitchPlate", "    =====> N");
          return this;
        }
      } else if (this->status_.clickable && (tp.state == SwitchPlateTouchState::Pressed) && touched) {
        if (this->status_.pressed == 0) {
          this->status_.pressed = 1;
          this->set_redraw();
          display().call_update();
          ESP_LOGD("SwitchPlate", "    =====> O");
        } else {
          ESP_LOGI("SwitchPlate", "    =====> Q");
        }
        return this;
      } else if (tp.state == SwitchPlateTouchState::Moving) {
        if (!touched && this->status_.draggable) {
          ESP_LOGD("SwitchPlate", "    =====> W");
          return nullptr;
        } else {
          ESP_LOGI("SwitchPlate", "    =====> R");
        }
        return this;
      } else if (this->status_.droppable && (tp.state == SwitchPlateTouchState::Dragging) && touched) {
        ESP_LOGW("SwitchPlate", "    =====> P");
        return this;
      }
    }
    ESP_LOGW("SwitchPlate", "    =====> M");
    return nullptr;
  };
  virtual void handle_touch(SwitchPlateTouch tp) {}

  void set_widget_options(uint8_t bit_no, bool state) {
    if (bit_no >= 16) {
      if (state) {
        this->status_.raw |= 1UL << bit_no;
      } else {
        this->status_.raw &= ~(1UL << bit_no);
      }
    }
  }

  void set_x(int16_t value) { this->dimension_.x = value; }
  void set_y(int16_t value) { this->dimension_.y = value; }
  void set_width(int16_t value) { this->dimension_.w = value; }
  void set_height(int16_t value) { this->dimension_.h = value; }
  void set_dimension(int16_t x, int16_t y, int16_t width, int16_t height) {
    this->dimension_ = Rect(x, y, width, height);
  }
  int16_t x() { return this->dimension_.x; }
  int16_t y() { return this->dimension_.y; }
  int16_t width() { return this->dimension_.w; }
  int16_t height() { return this->dimension_.h; }

  virtual int16_t screen_width() {
    if ((this->parent_ != nullptr)) {
      return this->parent_->screen_width();
    }
    return 32666;
  }

  virtual int16_t screen_height() {
    if ((this->parent_ != nullptr)) {
      return this->parent_->screen_height();
    }
    return 32666;
  }

  virtual int16_t calc_x() {
    int x = 0;
    if ((this->parent_ != nullptr)) {
      x = this->parent_->calc_x();
    }
    return x + this->x();
  }

  virtual int16_t calc_y() {
    int y = 0;
    if ((this->parent_ != nullptr)) {
      y = this->parent_->calc_y();
    }
    return y + this->y();
  }

  virtual Rect get_boundry() {
    Rect r = Rect(this->calc_x(), this->calc_y(), this->calc_x() + this->width(), this->calc_y() + this->height());
    return r;
  }

  void set_visible(bool visible) { this->status_.visible = visible ? 1 : 0; }
  bool visible() {
    if ((this->parent_ != nullptr) && !this->parent_->visible()) {
      return false;
    }
    return this->status_.visible == 1;
  }
  void set_enabled(bool enabled) { this->status_.disabled = !enabled ? 1 : 0; }
  bool enabled() {
    if ((this->parent_ != nullptr) && !this->parent_->enabled()) {
      return false;
    }
    return !this->status_.disabled;
  }

  void set_redraw() { this->status_.invalidate = 1; }
  void clear_redraw() { this->status_.invalidate = 0; }

  virtual Rect calc_child_clipping() { return Rect(); }

  virtual Rect calc_clipping() {
    Rect result = Rect();
    if (this->visible()) {
      if (this->need_redrawing()) {
        result = this->get_boundry();
      } else {
        result = calc_child_clipping();
        if (result.is_set()) {
          result.substract(this->get_boundry());
        }
      }
    }
    return result;
  }

  virtual bool need_redrawing() {
    std::string txt = this->get_text_();
    int state = this->get_state_();

    if (state != this->old_state_) {
      this->old_state_ = state;
      this->set_redraw();
    }

    if (txt != this->old_text_) {
      this->old_text_ = txt;
      this->set_redraw();
    }
    return this->status_.invalidate == 1;
  }

  template<typename V> void set_text(V val) { this->text_ = val; }
  std::string text() const { return this->old_text_; }

  template<typename V> void set_state(V val) { this->state_ = val; }
  int state() const { return const_cast<SwitchPlateItem *>(this)->state_.value(this); }

  void show_background() {
    uint8_t radius = get_style(STYLE_BORDER_RADIUS).uint32_;
    Rect r = this->get_boundry();

    if (this->has_style(STYLE_BACKGROUND_COLOR)) {
      display().filled_rectangle(r.x, r.y, r.w - r.x, r.h - r.y, radius,
                                 this->get_style(STYLE_BACKGROUND_COLOR).color_);
    } else if (this->has_style(STYLE_BACKGROUND_GRADIENT_FROM)) {
      Color from_color = this->get_style(STYLE_BACKGROUND_GRADIENT_FROM).color_;
      Color to_color = this->get_style(STYLE_BACKGROUND_GRADIENT_TO).color_;
      GradientDirection dir = (GradientDirection) this->get_style(STYLE_BACKGROUND_GRADIENT_DIRECTION).direction_;
      display().filled_rectangle(r.x, r.y, r.w - r.x, r.h - r.y, radius, from_color, to_color, dir);
    }
    if (this->has_style(STYLE_BORDER_COLOR)) {
      display().rectangle(r.x, r.y, r.w - r.x, r.h - r.y, radius, this->get_style(STYLE_BORDER_COLOR).color_);
    } else if (this->has_style(STYLE_BORDER_GRADIENT_FROM)) {
      Color from_color = this->get_style(STYLE_BORDER_GRADIENT_FROM).color_;
      Color to_color = this->get_style(STYLE_BORDER_GRADIENT_TO).color_;
      GradientDirection dir = (GradientDirection) this->get_style(STYLE_BORDER_GRADIENT_DIRECTION).direction_;
      display().rectangle(r.x, r.y, r.w - r.x, r.h - r.y, radius, from_color, to_color, dir);
    }
  }

  virtual void show_text() {
    show_background();
    Color color = this->get_style(STYLE_TEXT_COLOR).color_;
    Font *font = this->get_style(STYLE_TEXT_FONT).font_;
    Align align = this->get_style(STYLE_TEXT_ALIGN).align_;
    // int x, int y, Font *font, Color color, TextAlign align, const char *text
    display().print(this->calc_x(), this->calc_y(), font, color, this->text().c_str());
  };

  virtual void call_show() {
    Rect r = this->get_boundry();
    if (this->visible() && !display().is_clipped(r)) {
      r.substract(display().get_clipping());
      if (r.is_set()) {
        display().set_clipping(r);
        this->show();
        display().clear_clipping();
      }
    }
    clear_redraw();
  }

 protected:
  std::string get_text_() const { return const_cast<SwitchPlateItem *>(this)->text_.value(this); }
  int get_state_() const { return const_cast<SwitchPlateItem *>(this)->state_.value(this); }
  bool ENABLED_ = true;

  TemplatableValue<int, const SwitchPlateItem *> state_;
  int old_state_{0};

  TemplatableValue<std::string, const SwitchPlateItem *> text_;
  std::string old_text_{""};

  Rect dimension_;
};

// ============================================================================== SwitchPlateGroup

class SwitchPlateGroup : public SwitchPlateItem {
 public:
  void add_widget(SwitchPlateItem *widget) {
    widget->set_parent(this);
    this->widgets_.push_back(widget);
  }

  void call_setup() override {
    SwitchPlateItem::call_setup();

    this->setup();
    for (auto *widget : this->widgets_) {
      widget->call_setup();
    }
  }
  SwitchPlateBase *check_touch(SwitchPlateTouch tp, Rect parent) {
    Rect r = this->get_boundry();
    SwitchPlateBase *check;
    r.substract(parent);
    if (r.is_set() && (tp.state == SwitchPlateTouchState::Pressed)) {
      for (auto it = this->widgets_.rbegin(); it != this->widgets_.rend(); ++it) {
        check = (*it)->check_touch(tp, r);
        if (check != nullptr)
          return check;  // NOLINT
      }
    }
    ESP_LOGD("SwitchPlate", "  =====> K");

    return SwitchPlateItem::check_touch(tp, parent);
  }

  bool need_child_redrawing() {
    for (auto *widget : this->widgets_) {
      if (widget->need_redrawing()) {
        return true;
      }
    }
    return false;
  }

  virtual void call_show() {
    if (this->visible() && !display().is_clipped(this->get_boundry())) {
      Rect r = this->calc_clipping();
      if (r.is_set()) {
        display().set_clipping(r);

        this->show();
        for (auto *widget : this->widgets_) {
          widget->call_show();
        }
        display().clear_clipping();
      }
    }
    clear_redraw();
  }

  size_t items_size() const { return this->widgets_.size(); }
  SwitchPlateBase *item(size_t i) { return this->widgets_[i]; }

  virtual Rect calc_child_clipping() {
    Rect r = Rect();
    if (this->visible()) {
      for (auto *widget : this->widgets_) {
        if (widget->need_redrawing()) {
          r.join(widget->get_boundry());
        }
      }
    }
    return r;
  }

 protected:
  std::vector<SwitchPlateItem *> widgets_;
};

// ============================================================================== SwitchPlatePage

class SwitchPlatePage : public SwitchPlateGroup {
 public:
  uint32_t get_widget_id() { return STYLE_WIDGET_PAGE; }

  void setup() { this->style_.set(STYLE_BACKGROUND_COLOR, Color(0x111111), true); };

  void show() { show_background(); }

  virtual int16_t x() { return 0; }
  virtual int16_t y() { return 0; }
  virtual int16_t height() { return this->screen_height(); }
  virtual int16_t width() { return this->screen_width(); }

  void set_next(SwitchPlatePage *next);
  SwitchPlatePage *get_next() { return this->next_; }

  void set_prev(SwitchPlatePage *prev);
  SwitchPlatePage *get_prev() { return this->prev_; }

  void select() { ((SwitchPlate *) this->parent_)->show_page(this); }

  void set_selectable(bool selectable) { this->selectable_ = selectable; }
  bool get_selectable() { return this->selectable_; }

  void set_name(std::string name) { this->name_ = name; }
  std::string get_name() { return this->name_; }

 protected:
  SwitchPlatePage *prev_{nullptr};
  SwitchPlatePage *next_{nullptr};
  bool selectable_ = true;
  std::string name_ = "";
};

/// ============================================================================== SwitchPlateLabel

class SwitchPlateLabel : public SwitchPlateItem {
 public:
  uint32_t get_style_class_id() { return STYLE_WIDGET_LABEL; }

  void setup() {
    this->style_.set(STYLE_TEXT_COLOR, Color(0xFFFFFF), true);
    this->style_.set(STYLE_TEXT_FONT, this->get_font("default_font", true), true);
    this->style_.set(STYLE_TEXT_ALIGN, LEFT);
    ESP_LOGW("SwitchPlate", "    Style test  %s 0x%06X %s  0x%06X",
             YESNO(this->style_.has(STYLE_TEXT_COLOR)), this->style_.get(STYLE_TEXT_COLOR).color_.raw_32,
             YESNO(this->has_style(STYLE_TEXT_COLOR)),  this->get_style(STYLE_TEXT_COLOR).color_.raw_32);
  };
  virtual void show() {
    show_background();
    show_text();
  };
};

/// ============================================================================== SwitchPlateLabel

class SwitchPlateClock : public SwitchPlateLabel {
 public:
  uint32_t get_style_class_id() { return STYLE_WIDGET_CLOCK; }
  void setup() {
    this->style_.set(STYLE_TEXT_COLOR, Color(0xFFFFFF), true);
    this->style_.set(STYLE_TEXT_FONT, this->get_font("default_font", true), true);
    this->style_.set(STYLE_TEXT_ALIGN, LEFT);
  };
  virtual void show() {
    show_background();
    show_text();
  };
};

// ============================================================================== SwitchPlateLabel

class SwitchPlateButton : public SwitchPlateItem {
 public:
   uint32_t get_style_class_id() { return STYLE_WIDGET_BUTTON; }

  void setup() {
    this->style_.set(STYLE_TEXT_COLOR, Color((uint32_t) 0xffffff), true);
    this->style_.set(STYLE_TEXT_FONT, this->get_font("default_font", true), true);
    this->style_.set(STYLE_TEXT_ALIGN, CENTER, true);
    this->style_.set(STYLE_BORDER_RADIUS, 10,true);

    this->style_.set(STYLE_BORDER_COLOR, Color(0xDD0000), true);
    this->style_.set(STYLE_BACKGROUND_COLOR, Color(0x660000), true);

    this->style_.set(STYLE_BORDER_COLOR | STYLE_PRESSED, Color(0x00DD00), true);
    this->style_.set(STYLE_BACKGROUND_COLOR | STYLE_PRESSED, Color(0x00600), true);

    this->status_.clickable = 1;
  };
  virtual void show() {
    show_background();
    show_text();
  };
};

// ============================================================================== SwitchPlateSpinner

class SwitchPlateSpinner : public SwitchPlateItem {
 public:
  void setup() { this->style_.set(STYLE_TEXT_COLOR, Color((uint32_t) 0xFFFFFF), true); };
  void show() override { show_background(); };
};

// ============================================================================== SwitchPlateSlider

class SwitchPlateSlider : public SwitchPlateItem {
 public:
  void setup() { this->style_.set(STYLE_TEXT_COLOR, 0xFFFFFF, true); };
  virtual void show(){
      //       display().print(this->calc_x, this->calc_y, this->text());
  };
};

/// ==============================================================================

template<typename... Ts> class SwitchPlateShowAction : public Action<Ts...> {
 public:
  TEMPLATABLE_VALUE(SwitchPlatePage *, page)

  void play(Ts... x) override {
    auto *page = this->page_.value(x...);
    if (page != nullptr) {
      page->select();
    }
  }
};

template<typename... Ts> class SwitchPlateShowNextAction : public Action<Ts...> {
 public:
  SwitchPlateShowNextAction(SwitchPlate *plate) : plate_(plate) {}

  void play(Ts... x) override { this->plate_->show_next(); }

  SwitchPlate *plate_;
};

template<typename... Ts> class SwitchPlateShowHomeAction : public Action<Ts...> {
 public:
  SwitchPlateShowHomeAction(SwitchPlate *plate) : plate_(plate) {}

  void play(Ts... x) override { this->plate_->show_home(); }

  SwitchPlate *plate_;
};

template<typename... Ts> class SwitchPlateShowPrevAction : public Action<Ts...> {
 public:
  SwitchPlateShowPrevAction(SwitchPlate *plate) : plate_(plate) {}

  void play(Ts... x) override { this->plate_->show_prev(); }

  SwitchPlate *plate_;
};

template<typename... Ts> class SwitchPlateIsPageCondition : public Condition<Ts...> {
 public:
  SwitchPlateIsPageCondition(SwitchPlate *plate) : plate_(plate) {}

  void set_page(SwitchPlatePage *page) { this->page_ = page; }
  bool check(Ts... x) override { return this->plate_->current_page() == this->page_; }

 protected:
  SwitchPlate *plate_;
  SwitchPlatePage *page_;
};

class DisplayOnPageChangeTrigger : public Trigger<SwitchPlatePage *, SwitchPlatePage *> {
 public:
  explicit DisplayOnPageChangeTrigger(SwitchPlate *plate) { plate->add_on_page_change_trigger(this); }
  void process(SwitchPlatePage *from, SwitchPlatePage *to);
  void set_from(SwitchPlatePage *p) { this->from_ = p; }
  void set_to(SwitchPlatePage *p) { this->to_ = p; }

 protected:
  SwitchPlatePage *from_{nullptr};
  SwitchPlatePage *to_{nullptr};
};



}  // namespace switch_plate
}  // namespace esphome