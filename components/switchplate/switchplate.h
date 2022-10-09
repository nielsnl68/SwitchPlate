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
#ifdef USE_SWITCH
#include "esphome/components/switch/switch.h"
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

#define WORD_TO_BINARY(uint16_t) \
  ((uint16_t) &0x8000 ? '1' : '0'), ((uint16_t) &0x4000 ? '1' : '0'), ((uint16_t) &0x2000 ? '1' : '0'), \
      ((uint16_t) &0x1000 ? '1' : '0'), ((uint16_t) &0x0800 ? '1' : '0'), ((uint16_t) &0x0400 ? '1' : '0'), \
      ((uint16_t) &0x0200 ? '1' : '0'), ((uint16_t) &0x0100 ? '1' : '0'), ((uint16_t) &0x80 ? '1' : '0'), \
      ((uint16_t) &0x40 ? '1' : '0'), ((uint16_t) &0x20 ? '1' : '0'), ((uint16_t) &0x10 ? '1' : '0'), \
      ((uint16_t) &0x08 ? '1' : '0'), ((uint16_t) &0x04 ? '1' : '0'), ((uint16_t) &0x02 ? '1' : '0'), \
      ((uint16_t) &0x01 ? '1' : '0')

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

  RELATIVE = 0x80,
  ABSOLUTE = 0x81,
  HORZONTAL = 0x82,
  VERTICAL = 0x84,
};

enum class Mode : uint8_t {
  EXPAND = 0,
  BREAK = 1,
  DOTS = 2,
  SCROLL = 3,
  LOOP = 4,
  CROP = 5,

  CONSTANT = 10,
  SLOWDOWNUP = 11,
  SLOWSTRATCH = 12,

  ROUND_MODE = 20,
  RECT_MODE = 21
};

enum class Direction : uint8_t {
  DOWN = 0,
  UP = 1,
  LEFT = 2,
  RIGHT = 3,

  CW = 10,
  CCW = 11,

  NONE = 20,
  HORIZONTAL = 21,
  VERTICAL = 22,
  BOTH = 23,
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

struct Style {
  static constexpr uint32_t BACKGROUND = 0x0001;
  static constexpr uint32_t BORDER = 0x0002;
  static constexpr uint32_t TEXT = 0x0003;
  static constexpr uint32_t HEADER = 0x0004;
  static constexpr uint32_t FOOTER = 0x0005;
  static constexpr uint32_t FOREGROUND = 0x0006;
  static constexpr uint32_t SWITCH = 0x0007;
  static constexpr uint32_t CHECKBOX = 0x0008;

  static constexpr uint32_t COLOR = 0x0080;
  static constexpr uint32_t IMAGE = 0x00A0;

  static constexpr uint32_t ID = 0x0100;

  static constexpr uint32_t FROM = 0x0000;
  static constexpr uint32_t TO = 0x0300;
  static constexpr uint32_t DIRECTION = 0x0400;
  static constexpr uint32_t FONT = 0x0500;
  static constexpr uint32_t ALIGN = 0x0600;
  static constexpr uint32_t MODE = 0x0700;
  static constexpr uint32_t RADIUS = 0X0800;
  static constexpr uint32_t OFFSET_X = 0x0A00;
  static constexpr uint32_t OFFSET_Y = 0x0B00;
  static constexpr uint32_t HEIGTH = 0x0C00;
  static constexpr uint32_t WIDTH = 0x0D00;

  static constexpr uint32_t NONE = 0x00000;
  static constexpr uint32_t PRESS = 0x10000;
  static constexpr uint32_t SELECT = 0x20000;
  static constexpr uint32_t DISABLE = 0x40000;

  static constexpr uint32_t BASE_SWITCHPLATE = 0x100000;
  static constexpr uint32_t WIDGET_LABEL = 0x200000;
  static constexpr uint32_t WIDGET_DATETIME = 0x300000;
  static constexpr uint32_t WIDGET_BUTTON = 0x400000;
  static constexpr uint32_t WIDGET_PAGE = 0x500000;
  static constexpr uint32_t WIDGET_PAGETITLE = 0x600000;
  static constexpr uint32_t WIDGET_PANEL = 0x700000;
  static constexpr uint32_t WIDGET_IMAGE = 0x800000;
  static constexpr uint32_t WIDGET_STATIC = 0x900000;
  static constexpr uint32_t WIDGET_CHECKBOX = 0xA00000;

  static constexpr uint32_t BACKGROUND_COLOR = BACKGROUND | COLOR;
  static constexpr uint32_t BACKGROUND_COLOR_TO = BACKGROUND | COLOR | TO;
  static constexpr uint32_t BACKGROUND_COLOR_DIRECTION = BACKGROUND | COLOR | DIRECTION;
  static constexpr uint32_t BACKGROUND_IMAGE = BACKGROUND | IMAGE | ID;

  static constexpr uint32_t BORDER_COLOR = BORDER | COLOR;
  static constexpr uint32_t BORDER_COLOR_TO = BORDER | COLOR | TO;
  static constexpr uint32_t BORDER_COLOR_DIRECTION = BORDER | COLOR | DIRECTION;
  static constexpr uint32_t BORDER_RADIUS = BORDER | RADIUS;

  static constexpr uint32_t TEXT_COLOR = TEXT | COLOR;
  static constexpr uint32_t TEXT_COLOR_TO = TEXT | COLOR | TO;
  static constexpr uint32_t TEXT_COLOR_DIRECTION = TEXT | COLOR | DIRECTION;
  static constexpr uint32_t TEXT_FONT = TEXT | FONT;
  static constexpr uint32_t TEXT_ALIGN = TEXT | ALIGN;

  static constexpr uint32_t HEADER_HEIGTH = HEADER | HEIGTH;
  static constexpr uint32_t HEADER_COLOR = HEADER | COLOR;
  static constexpr uint32_t FOOTER_HEIGTH = FOOTER | HEIGTH;
  static constexpr uint32_t FOOTER_COLOR = FOOTER | COLOR;

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
  };
  Color color_;

  enum {
    is_none,
    is_uint32,
    is_int32,
    is_fload,
    is_bool,
    is_align,
    is_mode,
    is_direction,
    is_font,
    is_image,
    is_color
  } var_type;

  inline Style() ALWAYS_INLINE : uint32_(0), var_type(is_none) {}                         // NOLINT
  inline Style(uint32_t var) ALWAYS_INLINE : uint32_(var), var_type(is_uint32) {}         // NOLINT
  inline Style(int32_t var) ALWAYS_INLINE : var_type(is_int32), int32_(var) {}            // NOLINT
  inline Style(float var) ALWAYS_INLINE : var_type(is_fload), fload_(var) {}              // NOLINT
  inline Style(Mode var) ALWAYS_INLINE : var_type(is_mode), mode_(var) {}                 // NOLINT
  inline Style(Align var) ALWAYS_INLINE : var_type(is_align), align_(var) {}              // NOLINT
  inline Style(Direction var) ALWAYS_INLINE : var_type(is_direction), direction_(var) {}  // NOLINT
  inline Style(Color var) ALWAYS_INLINE : var_type(is_color), color_(var) {}              // NOLINT
  inline Style(Font *var) ALWAYS_INLINE : var_type(is_font), font_(var) {}                // NOLINT
  inline Style(Image *var) ALWAYS_INLINE : var_type(is_image), image_(var) {}             // NOLINT
  inline Style(bool var) ALWAYS_INLINE : var_type(is_bool), bool_(var) {}                 // NOLINT
};

struct Status {
  static constexpr uint16_t PRESSED_ = 0x0001;
  static constexpr uint16_t SELECTED_ = 0x0002;
  static constexpr uint16_t DISABLED_ = 0x0004;

  static constexpr uint16_t INVALIDATE_ = 0x0080;

  static constexpr uint16_t VISIBLE_ = 0x0100;
  static constexpr uint16_t CLICKABLE_ = 0x0200;
  static constexpr uint16_t SELECTABLE_ = 0x0400;
  static constexpr uint16_t DRAGGABLE_ = 0x0800;
  static constexpr uint16_t DROPPABLR_ = 0x1000;
  static constexpr uint16_t EDIT_NUM_ = 0x2000;
  static constexpr uint16_t EDIT_ALFA_ = 0x4000;
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
  inline Status() ALWAYS_INLINE : raw(1UL << 8) {}  // NOLINT
};

enum class TouchState : uint8_t {
  Released = 0,
  Pressed = 1,
  Holding = 2,
  Moving = 3,
  Dragging = 4,
  Dropping = 5,
  Ignore = 128
};
struct TouchInfo {
  uint16_t x;
  uint16_t y;
  uint16_t origin_x;
  uint16_t origin_y;
  TouchState state;
  SwitchPlateBase *origin;
  SwitchPlateBase *destiny;
  uint32_t start_now;
  inline TouchInfo() ALWAYS_INLINE : x(0),
                                     y(0),
                                     origin_x(0),
                                     origin_y(0),
                                     state(TouchState::Released),
                                     origin(nullptr),
                                     destiny(nullptr),
                                     start_now(0) {}  // NOLINT
};

enum class DoAction : uint8_t { DO_NOTTING, SHOW_HOME, SHOW_PREV, SHOW_NEXT };

// ============================================================================== SwitchPlateStyle

class SwitchPlateStyle {
 public:
  SwitchPlateStyle(){};
  void set(uint32_t key, Style var, bool as_missing = false) {
    if (!(as_missing && has(key))) {
      this->styles_[key] = var;
    }
  }
  Style get(uint32_t key) { return this->styles_[key]; }
  bool has(uint32_t key) { return this->styles_.count(key) != 0; }

 protected:
  std::map<uint32_t, Style> styles_{};
};

// ============================================================================== SwitchPlateBase

class SwitchPlateBase : public Parented<SwitchPlateBase> {
 public:
  SwitchPlateBase() {}

  virtual uint32_t get_style_class_id() { return Style::NONE; }

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

  virtual Rect get_boundry() { return Rect(this->absolute_x(), this->absolute_y(), this->width(), this->height()); }

  virtual bool is_visible() { return true; }
  virtual bool is_enabled() { return true; }

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

  virtual SwitchPlateBase *check_touch(TouchInfo tp, Rect parent) { return nullptr; }
  virtual void handle_touch(TouchInfo tp) {}

  uint32_t find_style_with_status(uint32_t key, Status status) {
    uint32_t result = 0;
    uint32_t themed = get_style_class_id();

    if (((status.raw & 6) == 6) && style_.has(result = (key | Style::DISABLE | Style::SELECT))) {
      return result;
    }
    if (((status.raw & 6) == 6) && thema_()->has(result = (themed | key | Style::DISABLE | Style::SELECT))) {
      return result;
    }

    if ((status.disabled) && style_.has(result = (key | Style::DISABLE))) {
      return result;
    }
    if ((status.disabled) && thema_()->has(result = (themed | key | Style::DISABLE))) {
      return result;
    }

    if (((status.raw & 3) == 3) && style_.has(result = (key | Style::PRESS | Style::SELECT))) {
      return result;
    }
    if (((status.raw & 3) == 3) && thema_()->has(result = (themed | key | Style::PRESS | Style::SELECT))) {
      return result;
    }

    if ((status.pressed) && style_.has(result = (key | Style::PRESS))) {
      return result;
    }
    if ((status.pressed) && thema_()->has(result = (themed | key | Style::PRESS))) {
      return result;
    }

    if ((status.selected) && style_.has(result = (key | Style::SELECT))) {
      return result;
    }
    if ((status.selected) && thema_()->has(result = (themed | key | Style::SELECT))) {
      return result;
    }

    if (style_.has(result = key)) {
      return result;
    }
    if (thema_()->has(result = (themed | key))) {
      return result;
    }
    return Style::NONE;
  }

  void set_style(uint32_t key, Style var, bool as_missing = false) {
    if (key < Style::BASE_SWITCHPLATE) {
      style_.set(key, var, as_missing);
      this->set_redraw();
    } else {
      thema_()->set(key, var, as_missing);
      // plate()->set_redraw();
    }
  }

  Style get_style(uint32_t key, Status status = Status()) {
    uint32_t key_ = find_style_with_status(key, status);
    if (key_ == Style::NONE) {
      ESP_LOGV("", " -- Style %x NOT found ! ! !  (%x)", key, get_style_class_id());
      return Style();
    } else if (key_ < Style::BASE_SWITCHPLATE) {
      return style_.get(key_);
    } else {
      return thema_()->get(key_);
    }
  };

  bool has_style(uint32_t key, Status status = Status()) { return find_style_with_status(key, status) != Style::NONE; };

  virtual void set_redraw() = 0;

 protected:
  display::DisplayBuffer *display_{nullptr};
  touchscreen::Touchscreen *touchscreen_{nullptr};

  SwitchPlate *switchplate_{nullptr};
  SwitchPlateStyle style_{};

  SwitchPlateStyle *thema_() {
    static SwitchPlateStyle *theme_{nullptr};
    if (theme_ == nullptr) {
      theme_ = new SwitchPlateStyle();
    }
    if (theme_ == nullptr) {
      ESP_LOGE("SwitchPlateBase", "  >>-  Theme-style has not been set.");
    }
    return (theme_);
  }
};

// =====================================================================================================================
// SwitchPlate

class SwitchPlate : public Component, public SwitchPlateBase, public TouchListener {
 public:
  uint32_t get_style_class_id() override { return Style::BASE_SWITCHPLATE; }

  void set_display(display::DisplayBuffer *display) {
    this->display_ = display;
    display->set_writer([this](display::DisplayBuffer &disp_buf) {
      ESP_LOGV(TAG, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
      this->call_show();
      ESP_LOGV(TAG, "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
    });
  }

  void set_touchscreen(touchscreen::Touchscreen *touchscreen) {
    this->touchscreen_ = touchscreen;
    this->set_display(touchscreen->get_display());
    touchscreen->register_listener(this);
  }
  void set_default_font(Font *font) {
    this->set_style(Style::FONT, font);
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

  void redraw() {
    defer([this]() { this->display_->call_update(); });
  }

 protected:
  /// High Frequency loop() requester used during sampling phase.
  // HighFrequencyLoopRequester high_freq_;

  std::vector<SwitchPlateItem *> header_;
  std::vector<SwitchPlateItem *> footer_;

  bool tabview_ = false;
  uint16_t header_height_{24};
  uint16_t footer_height_{24};

  SwitchPlatePage *first_page_{nullptr};
  SwitchPlatePage *current_page_{nullptr};
  SwitchPlatePage *previous_page_{nullptr};

  TouchInfo touch_info_ = TouchInfo();
};

// =================================================================================================================
// SwitchPlateItem

class SwitchPlateItem : public SwitchPlateBase {
 public:
  SwitchPlateItem(){};
#ifdef USE_SWITCH
  void register_switch(switch_::Switch *obj) { this->switches_.push_back(obj); }
#endif
  void update_switches(bool state) {
#ifdef USE_SWITCH
    for (auto *t : switches_)
      t->publish_state(state);
#endif
  }

  void set_status(uint32_t bit_no, bool state) {
    int old_status = this->status_.raw;
    if (state) {
      this->status_.raw |= bit_no;
    } else {
      this->status_.raw &= ~(bit_no);
    }
    ESP_LOGVV(TAG,
              "    =====> old: " BYTE_TO_BINARY_PATTERN BYTE_TO_BINARY_PATTERN
              ", new: " BYTE_TO_BINARY_PATTERN BYTE_TO_BINARY_PATTERN
              ", set: " BYTE_TO_BINARY_PATTERN BYTE_TO_BINARY_PATTERN " %s",
              WORD_TO_BINARY(old_status), WORD_TO_BINARY(this->status_.raw), WORD_TO_BINARY(bit_no), YESNO(state));
    if (old_status != this->status_.raw) {
      set_redraw();
    }
  }
  void set_pressed(bool value) { set_status(Status::PRESSED_, value); }
  void set_selected(bool value) { set_status(Status::SELECTED_, value); }
  void set_enabled(bool value) { set_status(Status::DISABLED_, !value); }
  bool is_enabled() override {
    if ((this->parent_ != nullptr) && !this->parent_->is_enabled()) {
      return false;
    }
    return !this->status_.disabled;
  }
  void set_visible(bool value) { set_status(Status::VISIBLE_, value); }
  bool is_visible() override {
    if ((this->parent_ != nullptr) && !this->parent_->is_visible()) {
      return false;
    }
    return this->status_.visible == 1;
  }

  void set_clickable(bool value) { set_status(Status::CLICKABLE_, value); }
  bool is_clickable() { return this->status_.clickable; }
  void set_selectable(bool value) { set_status(Status::SELECTABLE_, value); }
  bool is_selectable() { return this->status_.selectable; }

  template<typename V> void set_text(V val) { this->text_ = val; }
  std::string text() const { return this->old_text_; }

  template<typename V> void set_state(V val) { this->state_ = val; }
  int state() const { return const_cast<SwitchPlateItem *>(this)->state_.value(this); }

  void set_redraw() {
    bool old = this->status_.invalidate;
    this->status_.invalidate = 1;
    if ((not old) && (display() != nullptr) && (plate() != nullptr)) {
      plate()->redraw();
    }
  }

  void clear_redraw() { this->status_.invalidate = 0; }
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

  virtual Rect calc_child_clipping() { return Rect(); }

  virtual Rect calc_clipping() {
    Rect result = Rect();
    if (this->is_visible()) {
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

  SwitchPlateBase *check_touch(TouchInfo tp, Rect parent) {
    if (this->status_.visible && !this->status_.disabled) {
      Rect r = this->get_boundry();
      r.substract(parent);
      bool touched = r.inside(tp.x, tp.y);
      ESP_LOGV("SwitchPlate", "    =====> L touched: %s, clickable: %s", YESNO(touched),
               YESNO(this->status_.clickable));
      if ((tp.state == TouchState::Released)) {
        if (this->status_.pressed) {
          set_pressed(false);
          if (this->status_.droppable && (tp.origin != this) && touched) {
            ESP_LOGW("SwitchPlate", "    =====> N");
            return this;
          }
          if ((this->status_.selectable == 0) && (this->action_ != DoAction::DO_NOTTING)) {
            switch (this->action_) {
              case DoAction::SHOW_HOME:
                plate()->show_home();
                /* code */
                break;
              case DoAction::SHOW_PREV:
                plate()->show_prev();
                /* code */
                break;
              case DoAction::SHOW_NEXT:
                plate()->show_next();
                /* code */
                break;
              default:
                break;
            }
          } else {
            if (this->status_.selectable == 0) {
              update_switches(false);
            }
            handle_touch(tp);
          }
        }

      } else if (this->status_.clickable && (tp.state == TouchState::Pressed) && touched) {
        if (this->status_.pressed == 0) {
          set_pressed(true);
          if (this->status_.selectable) {
            set_selected(this->status_.selected == 0);
            update_switches(this->status_.selected);
          } else {
            update_switches(true);
          }
          ESP_LOGV("SwitchPlate", "    =====> O");
        } else {
          ESP_LOGV("SwitchPlate", "    =====> Q");
        }
        handle_touch(tp);
        return this;
      } else if (tp.state == TouchState::Moving) {
        if (!touched && this->status_.draggable) {
          ESP_LOGD("SwitchPlate", "    =====> W");
          return nullptr;
        } else {
          ESP_LOGV("SwitchPlate", "    =====> R");
        }
        handle_touch(tp);
        return this;
      } else if (this->status_.droppable && (tp.state == TouchState::Dragging) && touched) {
        ESP_LOGW("SwitchPlate", "    =====> P");
        return this;
      }
    }
    ESP_LOGV("SwitchPlate", "    =====> M");
    return nullptr;
  };
  virtual void handle_touch(TouchInfo tp) {}

  void set_disable_style();

  bool get_color_definition(uint32_t style, Color &from, Color &to, GradientDirection &dir) {
    if (this->has_style(style | Style::COLOR, this->status_)) {
      from = this->get_style(style | Style::COLOR, this->status_).color_;
      to = from;
      dir = GradientDirection::GRADIENT_NONE;
      if (this->has_style(style | Style::COLOR | Style::TO, this->status_)) {
        to = this->get_style(style | Style::COLOR | Style::TO, this->status_).color_;
        dir = (GradientDirection) this->get_style(style | Style::COLOR | Style::DIRECTION, this->status_).direction_;
      }
      return true;
    }
    return false;
  }

  void show_background() {
    uint8_t radius = get_style(Style::BORDER_RADIUS, this->status_).uint32_;
    Rect r = this->get_boundry();
    show_background(r.x, r.y, r.w, r.h, radius);
  }

  void show_background(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t radius) {
    Color from, to;
    GradientDirection dir = GradientDirection::GRADIENT_NONE;

    if (this->get_color_definition(Style::BACKGROUND, from, to, dir)) {
      display()->filled_rectangle(x, y, width, height, radius, from, to, dir);
    }

    if (this->get_color_definition(Style::BORDER_COLOR, from, to, dir)) {
      display()->rectangle(x, y, width, height, radius, from, to, dir);
    }
  }

  void show_image(int16_t offset_x, int16_t offset_y, Image *image, Color color_on, Color color_off);

  bool has_border() { return (this->has_style(Style::BORDER_COLOR, this->status_)); }

  void calc_text_alignment(Font *font, Align align, int16_t &x, int16_t &y) {
    Rect r = get_boundry();
    if (this->has_border()) {
      r.expand(-1, -1);
    }

    auto x_align = TextAlign(int(align) & 0x18);
    auto y_align = TextAlign(int(align) & 0x07);

    switch (x_align) {
      case TextAlign::RIGHT:
        x = r.x2();
        break;
      case TextAlign::CENTER_HORIZONTAL:
        x = r.x + (r.w / 2);
        break;
      case TextAlign::LEFT:
      default:
        // LEFT
        x = r.x;
        break;
    }
    switch (y_align) {
      case TextAlign::BOTTOM:
        y = r.y2();
        break;
      case TextAlign::BASELINE:
        y = r.y + font->get_baseline();
        break;
      case TextAlign::CENTER_VERTICAL:
        y = r.y + (r.h / 2);
        break;
      case TextAlign::TOP:
      default:
        y = r.y;
        break;
    }
  }

  void call_show() override {
    Rect r = this->get_boundry();

    if (this->is_visible() && display()->get_clipping().inside(r)) {
      display()->push_clipping(r);
      this->show();
      display()->pop_clipping();
    }
    clear_redraw();
  }

  void set_action(DoAction action) { this->action_ = action; }

 protected:
  std::string get_text_() const { return const_cast<SwitchPlateItem *>(this)->text_.value(this); }
  int get_state_() const { return const_cast<SwitchPlateItem *>(this)->state_.value(this); }
  bool ENABLED_ = true;

  TemplatableValue<int, const SwitchPlateItem *> state_;
  int old_state_{0};

  TemplatableValue<std::string, const SwitchPlateItem *> text_;
  std::string old_text_{""};

  Rect dimension_;
  Status status_{};

  DoAction action_{DoAction::DO_NOTTING};

#ifdef USE_SWITCH
  std::vector<switch_::Switch *> switches_;
#endif
};

//  ===============================================================================================================
//  SwitchPlateGroup

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
  SwitchPlateBase *check_touch(TouchInfo tp, Rect parent) override {
    Rect r = this->get_boundry();
    SwitchPlateBase *check;
    r.substract(parent);
    if (r.is_set() && (tp.state == TouchState::Pressed)) {
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
    if (this->is_visible() && display()->get_clipping().inside(this->get_boundry())) {
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
    if (this->is_visible()) {
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

// =================================================================================================================
// SwitchPlatePage

class SwitchPlatePage : public SwitchPlateGroup {
 public:
  uint32_t get_widget_id() { return Style::WIDGET_PAGE; }

  void setup() override {
    set_disable_style();
    this->set_style(Style::BACKGROUND_COLOR, Color(0x001111), true);
  };

  void show() override { show_background(); }

  virtual int16_t x() { return 0; }
  virtual int16_t y() { return 0; }
  virtual int16_t height() { return this->screen_height(); }
  virtual int16_t width() { return this->screen_width(); }

  void set_next(SwitchPlatePage *next);
  SwitchPlatePage *get_next() { return this->next_; }
  bool can_next() { return this->next_ != nullptr; }

  void set_prev(SwitchPlatePage *prev);
  SwitchPlatePage *get_prev() { return this->prev_; }
  bool can_prev() { return this->prev_ != nullptr; }

  void select() { ((SwitchPlate *) this->parent_)->show_page(this); }

  void set_title(std::string title) { this->title_ = title; }
  std::string get_title() const { return this->title_; }

 protected:
  SwitchPlatePage *prev_{nullptr};
  SwitchPlatePage *next_{nullptr};
  std::string title_{""};
};

/// ===============================================================================================================
/// SwitchPlateLabel

class SwitchPlateLabel : public SwitchPlateItem {
 public:
  uint32_t get_style_class_id() override { return Style::WIDGET_LABEL; }

  void setup() override {
    set_disable_style();

    if (this->plate() == nullptr) {
      ESP_LOGE("SwitchPlateLabel", "  >>-  SwitchPlate has not been set.");
    } else {
      Font *font = this->plate()->get_style(Style::FONT).font_;
      this->set_style(Style::TEXT_FONT, font, true);
    }
    this->set_style(Style::TEXT_COLOR, Color(0xFFFFFF), true);
    this->set_style(Style::TEXT_ALIGN, Align::LEFT, true);
  };

  void show() override {
    int16_t x, y;
    Align align = this->get_style(Style::TEXT_ALIGN).align_;
    Font *font = this->get_style(Style::TEXT_FONT).font_;
    Color from, to;
    GradientDirection dir = GradientDirection::GRADIENT_NONE;

    this->get_color_definition(Style::TEXT, from, to, dir);
    show_background();
    calc_text_alignment(font, align, x, y);
    display()->print(x, y, font, from, (TextAlign) align, text().c_str());
  };

  void set_mode(Mode mode) {
    this->mode_ = mode;
    this->set_redraw();
  }

 protected:
  Mode mode_{Mode::CROP};
};

/// ============================================================================================================
/// SwitchPlateDateTime

class SwitchPlateDateTime : public SwitchPlateLabel {
 public:
  uint32_t get_style_class_id() override { return Style::WIDGET_DATETIME; }
  void setup() override {
    SwitchPlateLabel::setup();
#ifdef USE_TIME
    auto *time = *this->time_;
    auto format = this->format_;
    time->add_on_time_sync_callback([this] { this->time_synced(); });
    set_text([=](const switch_plate::SwitchPlateItem *it) -> std::string {
      if (this->time_synced_) {
        return (*time).now().strftime(format);
      } else {
        return "Waiting";
      }
    });
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
  void time_synced() { time_synced_ = true; }

 protected:
  std::string format_ = "%X";
  optional<time::RealTimeClock *> time_{};
  bool time_synced_{false};
};

// ============================================================================================================
// SwitchPlatePageTitle

class SwitchPlatePageTitle : public SwitchPlateLabel {
 public:
  uint32_t get_style_class_id() override { return Style::WIDGET_PAGETITLE; }
  void setup() override {
    SwitchPlateLabel::setup();
    SwitchPlate *myplate = plate();
    set_text(
        [=](const switch_plate::SwitchPlateItem *it) -> std::string { return myplate->current_page()->get_title(); });
  }
};

// ===============================================================================================================
// SwitchPlateButton

class SwitchPlateButton : public SwitchPlateLabel {
 public:
  uint32_t get_style_class_id() override { return Style::WIDGET_BUTTON; }

  void setup() override {
    this->set_style(Style::TEXT_ALIGN, Align::CENTER, true);
    SwitchPlateLabel::setup();
    this->set_style(Style::BORDER_RADIUS, 10, true);

    this->set_style(Style::BORDER_COLOR, Color(0xDDDDDD), true);
    this->set_style(Style::BACKGROUND_COLOR, Color(0x999999), true);

    this->set_style(Style::BORDER_COLOR | Style::PRESS, Color(0xEEEEEE), true);
    this->set_style(Style::BACKGROUND_COLOR | Style::PRESS, Color(0xAAAAAA), true);

    this->status_.clickable = 1;
  };
};

// ================================================================================================================
// SwitchPlateImage

class SwitchPlateImage : public SwitchPlateItem {
 public:
  void setup() override {
    set_disable_style();

    this->set_style(Style::IMAGE | Style::COLOR, Color(0xFFFFFF), true);
    this->set_style(Style::BACKGROUND_COLOR, Color(0x000000), true);
    auto *image = this->get_style(Style::IMAGE | Style::ID, this->status_).image_;
    set_state([=](const switch_plate::SwitchPlateItem *it) -> int { return image->get_current_frame(); });
  };
  void show() override {
    show_background();
    auto *image = this->get_style(Style::IMAGE | Style::ID, this->status_).image_;
    Color bg_color = this->get_style(Style::BACKGROUND_COLOR, this->status_).color_;
    Color fg_color = this->get_style(Style::IMAGE | Style::COLOR, this->status_).color_;
    int16_t offset_x = this->get_style(Style::IMAGE | Style::OFFSET_X, this->status_).uint32_;
    int16_t offset_y = this->get_style(Style::IMAGE | Style::OFFSET_Y, this->status_).uint32_;
    show_image(offset_x, offset_y, image, fg_color, bg_color);
  };
};

// ================================================================================================================
// SwitchPlatePanel

class SwitchPlatePanel : public SwitchPlateGroup {
 public:
  void setup() override {
    set_disable_style();
    this->set_style(Style::BACKGROUND_COLOR, Color((uint32_t) 0x003300), true);
  };
  void show() override { show_background(); };
};

// ================================================================================================================
// SwitchPlateSwitch

class SwitchPlateSwitch : public SwitchPlateButton {
 public:
  void setup() override {
    this->set_style(Style::TEXT_ALIGN, Align::CENTER, true);

    this->set_style(Style::BORDER_RADIUS, 10, true);

    this->set_style(Style::BORDER_COLOR, Color(0x999999), true);
    this->set_style(Style::BACKGROUND_COLOR, Color(0x660000), true);
    this->set_style(Style::FOREGROUND | Style::COLOR, Color(0xDD0000), true);

    this->set_style(Style::BORDER_COLOR | Style::SELECT, Color(0x999999), true);
    this->set_style(Style::BACKGROUND_COLOR | Style::SELECT, Color(0x006600), true);
    this->set_style(Style::FOREGROUND | Style::COLOR | Style::SELECT, Color(0x00DD00), true);

    this->set_style(Style::BORDER_COLOR | Style::PRESS, Color(0xEEEEEE), true);
    this->set_style(Style::BACKGROUND_COLOR | Style::PRESS, Color(0x770000), true);
    this->set_style(Style::FOREGROUND | Style::COLOR | Style::PRESS, Color(0xEE0000), true);

    this->set_style(Style::BORDER_COLOR | Style::PRESS | Style::SELECT, Color(0xEEEEEE), true);
    this->set_style(Style::BACKGROUND_COLOR | Style::PRESS | Style::SELECT, Color(0x007700), true);
    this->set_style(Style::FOREGROUND | Style::COLOR | Style::PRESS | Style::SELECT, Color(0x00EE00), true);

    SwitchPlateButton::setup();
    this->status_.selectable = 1;

  };

  void show() override {
    uint16_t width = this->width(), x_off = x() + 2;
    uint16_t height = this->height(), y_off = y() + 2;
    uint16_t radius = 3;
    uint16_t ribben = this->width() - 5;
    Mode mode = get_style(Style::SWITCH | Style::MODE).mode_;
    if (width == height) {
      if (mode != Mode::RECT_MODE) {
        radius = width / 2;
      }

    } else if (width < height) {
      // horizontal switch
      if (width * 2 < height) {
        height = width * 2;
      } else {
        width = height / 2;
      }
      ribben = width - 5;
      y_off = (y() + height) - (width - 2);
      if (mode != Mode::RECT_MODE) {
        radius = width / 2;
      }
    } else {
      // vertical switch
      if (height * 2 < width) {
        width = height * 2;
      } else {
        height = width / 2;
      }
      ribben = height - 5;
      x_off = (x() + width) - (height - 2);
      if (mode != Mode::RECT_MODE) {
        radius = height / 2;
      }
    }
    show_background(this->x(), this->y(), width, height, radius);
    Color from, to;
    GradientDirection dir = GradientDirection::GRADIENT_NONE;

    this->get_color_definition(Style::FOREGROUND, from, to, dir);
    if (status_.selected == 0) {
      x_off = x() + 2;
      y_off = y() + 2;
    }
    if (mode != Mode::RECT_MODE) {
      radius = radius - 2;
    }

    display()->filled_rectangle(x_off, y_off, ribben, ribben, radius, from, to, dir);
  };
};

}  // namespace switch_plate
}  // namespace esphome