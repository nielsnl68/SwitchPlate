#pragma once

#include "esphome/components/display/display_buffer.h"
#include "esphome/components/touchscreen/touchscreen.h"

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/automation.h"
#include "esphome/core/log.h"
#include <map>

#ifdef USE_TIME
#include "esphome/components/time/real_time_clock.h"
#endif
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

#define CONST_TOUCH_TO_MOVE 6  // NOLINT

namespace switch_plate {

const static char *TAG = "SwitchPlate";

enum class Align : uint8_t {
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
  GROUP_ALIGN_VERTICAL = 0x84,
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

// struct Style: uint32_t {

const uint32_t STYLE_BACKGROUND = 0x0001;
const uint32_t STYLE_BORDER = 0x0002;
const uint32_t STYLE_TEXT = 0x0003;
const uint32_t STYLE_HEADER = 0x0004;
const uint32_t STYLE_FOOTER = 0x0005;

const uint32_t STYLE_COLOR = 0x0100;
const uint32_t STYLE_GRADIENT_FROM = 0x0200;
const uint32_t STYLE_GRADIENT_TO = 0x0300;
const uint32_t STYLE_GRADIENT_DIRECTION = 0x0400;
const uint32_t STYLE_FONT = 0x0500;
const uint32_t STYLE_ALIGN = 0x0600;
const uint32_t STYLE_MODE = 0x0700;
const uint32_t STYLE_RADIUS = 0X0800;
const uint32_t STYLE_IMAGE = 0x0900;
const uint32_t STYLE_IMAGE_SHIFT_X = 0x0A00;
const uint32_t STYLE_IMAGE_SHIFT_Y = 0x0B00;

const uint32_t STYLE_DEFAULT = 0x00000;
const uint32_t STYLE_PRESSED = 0x10000;
const uint32_t STYLE_SELECTED = 0x20000;
const uint32_t STYLE_DISABLED = 0x40000;

const uint32_t STYLE_BASE_SWITCHPLATE = 0x100000;
const uint32_t STYLE_WIDGET_LABEL = 0x200000;
const uint32_t STYLE_WIDGET_DATETIME = 0x300000;
const uint32_t STYLE_WIDGET_BUTTON = 0x400000;
const uint32_t STYLE_WIDGET_PAGE = 0x500000;
const uint32_t STYLE_WIDGET_PAGETITLE = 0x600000;
const uint32_t STYLE_WIDGET_PANEL = 0x600000;

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
  uint8_t var_type;
  inline TaggedVariable() ALWAYS_INLINE : var_type(0), uint32_(0) {}  // NOLINT
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
      uint8_t draggable : 1;
      uint8_t droppable : 1;
      uint8_t editable : 2;
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

// ============================================================================== SwitchPlateStyle

class SwitchPlateStyle : public Parented<SwitchPlateBase> {
 public:
  SwitchPlateStyle(){};
  void set(uint32_t key, uint32_t var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLIN
    ESP_LOGI("", "- Set uint %x to %d ", key, var);
    this->styles_[key].uint32_ = var;
    this->styles_[key].var_type = 1;
    this->set_redraw();
  }

  void set(uint32_t key, int32_t var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    ESP_LOGI("", "- Set int %x to %d ", key, var);
    this->styles_[key].int32_ = var;
    this->styles_[key].var_type = 2;
    this->set_redraw();
  }

  void set(uint32_t key, float var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    ESP_LOGI("", "- Set float %x to %f", key, var);
    this->styles_[key].fload_ = var;
    this->styles_[key].var_type = 3;
    this->set_redraw();
  }

  void set(uint32_t key, Mode var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    ESP_LOGI("", "- Set mode %x to %d", key, (uint8_t) var);
    this->styles_[key].mode_ = var;
    this->styles_[key].var_type = 4;
    this->set_redraw();
  }

  void set(uint32_t key, Align var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    ESP_LOGI("", "- Set align %x to %d", key, (uint8_t) var);
    this->styles_[key].align_ = var;
    this->styles_[key].var_type = 5;
    this->set_redraw();
  }

  void set(uint32_t key, Direction var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    ESP_LOGI("", "- Set direction %x to %d", key, (uint8_t) var);
    this->styles_[key].direction_ = var;
    this->styles_[key].var_type = 6;
    this->set_redraw();
  }

  void set(uint32_t key, Color var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    ESP_LOGI("", "- Set Color %x to #%08x", key, var.raw_32);
    this->styles_[key].color_ = var;
    this->styles_[key].var_type = 7;
    this->set_redraw();
  }

  void set(uint32_t key, Font *var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    ESP_LOGI("", "- Set Font %x to %p", key, var);
    this->styles_[key].font_ = var;
    this->styles_[key].var_type = 8;
    this->set_redraw();
  }

  void set(uint32_t key, Image *var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    ESP_LOGI("", "- Set Image %x to %p", key, var);
    this->styles_[key].image_ = var;
    this->styles_[key].var_type = 9;
    this->set_redraw();
  }

  void set(uint32_t key, bool var, bool as_missing = false) {
    if (as_missing && has(key))
      return;  // NOLINT
    this->styles_[key].bool_ = var;
    ESP_LOGI("", "- Set bool %x to %s", key, var ? "true" : "false");
    this->styles_[key].var_type = 10;
    this->set_redraw();
  }

  TaggedVariable get(uint32_t key) { return this->styles_[key]; }
  bool has(uint32_t key) { return this->styles_.count(key) != 0; }

  void set_redraw();

 protected:
  std::map<uint32_t, TaggedVariable> styles_{};
};

// ============================================================================== SwitchPlateBase

class SwitchPlateBase : public Parented<SwitchPlateBase> {
 public:
  SwitchPlateBase() { style_.set_parent(this); };
  virtual uint32_t get_style_class_id() { return STYLE_DEFAULT; }

  virtual void setup(){};
  virtual void call_setup() { this->setup(); };

  virtual void show(){};
  virtual void call_show() { this->show(); };

  virtual int16_t screen_width() { return 32666; }
  virtual int16_t screen_height() { return 32666; }

  virtual int16_t x() { return 0; }
  virtual int16_t y() { return 0; }
  virtual int16_t width() { return this->screen_width(); }
  virtual int16_t height() { return this->screen_height(); }

  virtual int16_t absolute_x() { return x(); }
  virtual int16_t absolute_y() { return y(); }

  virtual Rect get_boundry() {
    return Rect(this->absolute_x(), this->absolute_y(), this->absolute_x() + this->width(),
                this->absolute_y() + this->height());
  }

  virtual bool visible() { return true; }
  virtual bool enabled() { return true; }

  void set_display(display::DisplayBuffer *display) { display_ = display; }
  void set_touchscreen(touchscreen::Touchscreen *touchscreen) { touchscreen_ = touchscreen; };
  void set_switchplate(SwitchPlate *plate) { this->switchplate_ = plate; }

  virtual DisplayBuffer *display() {
    if ((this->display_ == nullptr) && (parent_ != nullptr)) {
      this->display_ = parent_->display();
    }
    return (this->display_);
  }
  virtual SwitchPlate *plate() {
    if ((this->switchplate_ == nullptr) && (parent_ != nullptr)) {
      this->switchplate_ = parent_->plate();
    }
    if (this->switchplate_ == nullptr) {
      ESP_LOGE("SwitchPlateBase", "  >>-  SwitchPlate has not been set.");
    }
    return (this->switchplate_);
  }

  virtual SwitchPlateBase *check_touch(SwitchPlateTouch tp, Rect parent) { return nullptr; }
  virtual void handle_touch(SwitchPlateTouch tp) {}

  uint32_t find_style_with_status(uint32_t key, WidgetStatus status) {
    uint32_t result = 0;
    uint32_t themed = get_style_class_id();

    if (((status.raw & 6) == 6) && style_.has(result = (key | STYLE_DISABLED | STYLE_SELECTED))) {
      return result;
    }
    if (((status.raw & 6) == 6) && thema()->has(result = (themed | key | STYLE_DISABLED | STYLE_SELECTED))) {
      return result;
    }

    if ((status.disabled) && style_.has(result = (key | STYLE_DISABLED))) {
      return result;
    }
    if ((status.disabled) && thema()->has(result = (themed | key | STYLE_DISABLED))) {
      return result;
    }

    if (((status.raw & 3) == 3) && style_.has(result = (key | STYLE_PRESSED | STYLE_SELECTED))) {
      return result;
    }
    if (((status.raw & 3) == 3) && thema()->has(result = (themed | key | STYLE_PRESSED | STYLE_SELECTED))) {
      return result;
    }

    if ((status.pressed) && style_.has(result = (key | STYLE_PRESSED))) {
      return result;
    }
    if ((status.pressed) && thema()->has(result = (themed | key | STYLE_PRESSED))) {
      return result;
    }

    if ((status.selected) && style_.has(result = (key | STYLE_SELECTED))) {
      return result;
    }
    if ((status.selected) && thema()->has(result = (themed | key | STYLE_SELECTED))) {
      return result;
    }

    if (style_.has(result = key)) {
      return result;
    }
    if (thema()->has(result = (themed | key))) {
      return result;
    }
    return STYLE_DEFAULT;
  }

  virtual TaggedVariable get_style(uint32_t key, WidgetStatus status = WidgetStatus()) {
    uint32_t key_ = find_style_with_status(key, status);
    if (key_ == STYLE_DEFAULT) {
      ESP_LOGV("", " -- Style %x NOT found ! ! !  (%x)", key, get_style_class_id());
      return TaggedVariable();
    } else if (key_ < STYLE_BASE_SWITCHPLATE) {
      return style_.get(key_);
    } else {
      return thema()->get(key_);
    }
  };

  virtual bool has_style(uint32_t key, WidgetStatus status = WidgetStatus()) {
    return find_style_with_status(key, status) != STYLE_DEFAULT;
  };

  SwitchPlateStyle *thema() {
    static SwitchPlateStyle *theme_{nullptr};
    if (theme_ == nullptr) {
      theme_ = new SwitchPlateStyle();
      theme_->set_parent(this);
    }
    if (theme_ == nullptr) {
      ESP_LOGE("SwitchPlateBase", "  >>-  Theme-style has not been set.");
    }
    return (theme_);
  }
  virtual void set_redraw() = 0;

 protected:
  display::DisplayBuffer *display_{nullptr};
  touchscreen::Touchscreen *touchscreen_{nullptr};

  SwitchPlate *switchplate_{nullptr};
  SwitchPlateStyle style_{};
};

// ============================================================================== SwitchPlate

class SwitchPlate : public Component, public SwitchPlateBase, public TouchListener {
 public:
  uint32_t get_style_class_id() override { return STYLE_BASE_SWITCHPLATE; }
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
      ESP_LOGD("SP_display", "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
      this->show();
      ESP_LOGD("SP_display", "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
    });
  }

  void set_touchscreen(touchscreen::Touchscreen *touchscreen) {
    this->touchscreen_ = touchscreen;
    this->set_display(touchscreen->get_display());
    touchscreen->register_listener(this);
  }
  void set_default_font(Font *font) {
    this->style_.set(STYLE_FONT, font);
    this->set_redraw();
  }
  void set_tabview(bool tabview) {
    this->tabview_ = tabview;
    this->set_redraw();
  }
  void set_header_height(uint16_t header_height) {
    this->header_height_ = header_height;
    this->set_redraw();
  }
  void set_footer_height(uint16_t footer_height) {
    this->footer_height_ = footer_height;
    this->set_redraw();
  }

  void add_page(SwitchPlatePage *page);
  void add_header_widget(SwitchPlateItem *item);
  void add_footer_widget(SwitchPlateItem *item);

  /// Get the width of the image in pixels with rotation applied.
  int16_t screen_width() override { return this->display()->get_width(); }

  /// Get the height of the image in pixels with rotation applied.
  int16_t screen_height() override { return this->display()->get_height(); };

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
  void call_setup() override;

  void dump_config() override;
  void show() override;

  float get_setup_priority() const override { return esphome::setup_priority::HARDWARE; }

  void set_redraw() {}  // TODO: check of we need to do display()->call_update() here.

  void touch(TouchPoint tp);

  void release() {
    TouchPoint tp;
    tp.x = 32666;
    tp.y = 32666;
    this->touch(tp);
  }

  SwitchPlate *plate() { return this; }

 protected:
  /// High Frequency loop() requester used during sampling phase.
  // HighFrequencyLoopRequester high_freq_;

  std::vector<SwitchPlateItem *> header_;
  std::vector<SwitchPlateItem *> footer_;

  bool tabview_ = false;
  uint16_t header_height_{23};
  uint16_t footer_height_{23};

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
          display()->call_update();
        }
        if (this->status_.droppable && (tp.origin != this) && touched) {
          ESP_LOGW("SwitchPlate", "    =====> N");
          return this;
        }
      } else if (this->status_.clickable && (tp.state == SwitchPlateTouchState::Pressed) && touched) {
        if (this->status_.pressed == 0) {
          this->status_.pressed = 1;
          this->set_redraw();
          display()->call_update();
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

  void set_widget_status(uint8_t bit_no, bool state) {
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
  int16_t x() override { return this->dimension_.x; }
  int16_t y() override { return this->dimension_.y; }
  int16_t width() override { return this->dimension_.w; }
  int16_t height() override { return this->dimension_.h; }

  int16_t screen_width() override {
    if ((this->parent_ != nullptr)) {
      return this->parent_->screen_width();
    }
    return 32666;
  }

  int16_t screen_height() override {
    if ((this->parent_ != nullptr)) {
      return this->parent_->screen_height();
    }
    return 32666;
  }

  int16_t absolute_x() override {
    int x = 0;
    if ((this->parent_ != nullptr)) {
      x = this->parent_->absolute_x();
    }
    return x + this->x();
  }

  int16_t absolute_y() override {
    int y = 0;
    if ((this->parent_ != nullptr)) {
      y = this->parent_->absolute_y();
    }
    return y + this->y();
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
    ESP_LOGD("SwitchPlate", "Need '%s' to redraw? %S", this->old_text_.c_str(),(this->status_.invalidate?"Yes":"No"));
    return this->status_.invalidate == 1;
  }

  template<typename V> void set_text(V val) { this->text_ = val; }
  std::string text() const { return this->old_text_; }

  template<typename V> void set_state(V val) { this->state_ = val; }
  int state() const { return const_cast<SwitchPlateItem *>(this)->state_.value(this); }

  void show_background() {
    uint8_t radius = get_style(STYLE_BORDER_RADIUS, this->status_).uint32_;
    Rect r = this->get_boundry();

    if (this->has_style(STYLE_BACKGROUND_COLOR, this->status_)) {
      display()->filled_rectangle(r.x, r.y, r.w - r.x, r.h - r.y, radius,
                                  this->get_style(STYLE_BACKGROUND_COLOR, this->status_).color_);
    } else if (this->has_style(STYLE_BACKGROUND_GRADIENT_FROM, this->status_)) {
      Color from_color = this->get_style(STYLE_BACKGROUND_GRADIENT_FROM, this->status_).color_;
      Color to_color = this->get_style(STYLE_BACKGROUND_GRADIENT_TO, this->status_).color_;
      GradientDirection dir =
          (GradientDirection) this->get_style(STYLE_BACKGROUND_GRADIENT_DIRECTION, this->status_).direction_;
      display()->filled_rectangle(r.x, r.y, r.w - r.x, r.h - r.y, radius, from_color, to_color, dir);
    }
    if (this->has_style(STYLE_BORDER_COLOR, this->status_)) {
      display()->rectangle(r.x, r.y, r.w - r.x, r.h - r.y, radius,
                           this->get_style(STYLE_BORDER_COLOR, this->status_).color_);
    } else if (this->has_style(STYLE_BORDER_GRADIENT_FROM, this->status_)) {
      Color from_color = this->get_style(STYLE_BORDER_GRADIENT_FROM, this->status_).color_;
      Color to_color = this->get_style(STYLE_BORDER_GRADIENT_TO, this->status_).color_;
      GradientDirection dir =
          (GradientDirection) this->get_style(STYLE_BORDER_GRADIENT_DIRECTION, this->status_).direction_;
      display()->rectangle(r.x, r.y, r.w - r.x, r.h - r.y, radius, from_color, to_color, dir);
    }
  }

  void show_image(int x, int y, Image *image, Color color_on, Color color_off);

  bool has_border() {
    return (this->has_style(STYLE_BORDER_COLOR, this->status_) ||
            this->has_style(STYLE_BORDER_GRADIENT_FROM, this->status_));
  }

  void calc_text_alignment(Font *font, Align align, int16_t &x, int16_t &y) {
    Rect r = get_boundry();
    if (this->has_border()) {
      r.expand(-1, -1);
    }

    auto x_align = TextAlign(int(align) & 0x18);
    auto y_align = TextAlign(int(align) & 0x07);

    switch (x_align) {
      case TextAlign::RIGHT:
        x = r.w;
        break;
      case TextAlign::CENTER_HORIZONTAL:
        x = r.x + ((r.w - r.x + 1) / 2);
        break;
      case TextAlign::LEFT:
      default:
        // LEFT
        x = r.x;
        break;
    }
    switch (y_align) {
      case TextAlign::BOTTOM:
        y = r.h;
        break;
      case TextAlign::BASELINE:
        y = r.y + font->get_baseline();
        break;
      case TextAlign::CENTER_VERTICAL:
        y = r.y + ((r.h - r.y + 1) / 2);
        break;
      case TextAlign::TOP:
      default:
        y = r.y;
        break;
    }
  }

  virtual void call_show() {
    Rect r = this->get_boundry(), cr= display()->get_clipping();
    if (this->visible() && display()->get_clipping().inside(r)) {
      r.substract(cr);
      if (r.is_set()) {
        display()->push_clipping(r);
        this->show();
        display()->pop_clipping();
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
  WidgetStatus status_{};
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
  SwitchPlateBase *check_touch(SwitchPlateTouch tp, Rect parent) override {
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

  void call_show() override {
    if (this->visible() && display()->get_clipping().inside(this->get_boundry())) {
      Rect r = this->calc_clipping();
      if (r.is_set()) {
        display()->push_clipping(r);
        this->show();
        for (auto *widget : this->widgets_) {
          widget->call_show();
        }
        display()->pop_clipping();
      }
    }
    clear_redraw();
  }

  size_t items_size() const { return this->widgets_.size(); }
  SwitchPlateBase *item(size_t i) { return this->widgets_[i]; }

  Rect calc_child_clipping() override {
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

  void set_title(std::string title) { this->title_ = title; }
  std::string get_title() { return this->title_; }

 protected:
  SwitchPlatePage *prev_{nullptr};
  SwitchPlatePage *next_{nullptr};
  bool selectable_ = true;
  std::string title_ = "";
};

/// ============================================================================== SwitchPlateLabel

class SwitchPlateLabel : public SwitchPlateItem {
 public:
  uint32_t get_style_class_id() override { return STYLE_WIDGET_LABEL; }

  void setup() {
    if (this->plate() == nullptr) {
      ESP_LOGE("SwitchPlateLabel", "  >>-  SwitchPlate has not been set.");
    } else {
      Font *font = this->plate()->get_style(STYLE_FONT).font_;
      this->style_.set(STYLE_TEXT_FONT, font, true);
    }
    this->style_.set(STYLE_TEXT_COLOR, Color(0xFFFFFF), true);
    this->style_.set(STYLE_TEXT_ALIGN, Align::LEFT, true);
  };

  virtual void show() {
    int16_t x, y;
    Align align = this->get_style(STYLE_TEXT_ALIGN).align_;
    Color color = this->get_style(STYLE_TEXT_COLOR).color_;
    Font *font = this->get_style(STYLE_TEXT_FONT).font_;
    // int x, int y, Font *font, Color color, TextAlign align, const char *text

    show_background();
    calc_text_alignment(font, align, x, y);
    display()->print(x, y, font, color, (TextAlign) align, text().c_str());
  };

  void set_mode(Mode mode) {
    this->mode_ = mode;
    this->set_redraw();
  }

 protected:
  Mode mode_{TEXT_MODE_CROP};
};

/// ============================================================================== SwitchPlateDateTime

class SwitchPlateDateTime : public SwitchPlateLabel {
 public:
  uint32_t get_style_class_id() override { return STYLE_WIDGET_DATETIME; }
  void setup() {
    SwitchPlateLabel::setup();
#ifdef USE_TIME
    auto *time = *this->time_;
    auto format = this->format_;
    set_text([=](const switch_plate::SwitchPlateItem *it) -> std::string { return (*time).now().strftime(format); });
#endif
  }
  void set_time_format(std::string format) {
    this->format_ = format;
    this->set_redraw();
  }
  void set_time(time::RealTimeClock *time) {
    this->time_ = time;
    this->set_redraw();
  }

 protected:
  std::string format_ = "%X";
  optional<time::RealTimeClock *> time_{};
};

/// ============================================================================== SwitchPlatePageTitle

class SwitchPlatePageTitle : public SwitchPlateLabel {
 public:
  uint32_t get_style_class_id() override { return STYLE_WIDGET_PAGETITLE; }
  void setup() {
    SwitchPlateLabel::setup();
    auto plate = this->plate();
    set_text([=](const switch_plate::SwitchPlateItem *it) -> std::string { return return plate.current_page()->get_title(); });
  }

 protected:
};

// ============================================================================== SwitchPlateLabel

class SwitchPlateButton : public SwitchPlateLabel {
 public:
  uint32_t get_style_class_id() override { return STYLE_WIDGET_BUTTON; }

  void setup() {
    this->style_.set(STYLE_TEXT_ALIGN, Align::CENTER, true);
    SwitchPlateLabel::setup();
    this->style_.set(STYLE_BORDER_RADIUS, 10, true);

    this->style_.set(STYLE_BORDER_COLOR, Color(0xDD0000), true);
    this->style_.set(STYLE_BACKGROUND_COLOR, Color(0x660000), true);

    this->style_.set(STYLE_BORDER_COLOR | STYLE_PRESSED, Color(0x00DD00), true);
    this->style_.set(STYLE_BACKGROUND_COLOR | STYLE_PRESSED, Color(0x00600), true);

    this->status_.clickable = 1;
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
      //       display()->print(this->absolute_x, this->absolute_y, this->text());
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