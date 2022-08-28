#pragma once

#include "esphome/components/display/display_buffer.h"
#include "esphome/components/touchscreen/touchscreen.h"

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/automation.h"
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



namespace esphome
{
  using display::DisplayBuffer;
  using display::Rect;
  using display::Image;
  using display::Font;
  using display::GradientDirection;
  using display::TextAlign;

  namespace switch_plate
  {
 
    enum Rotation
    {
      DISPLAY_ROTATION_0_DEGREES = 0,
      DISPLAY_ROTATION_90_DEGREES = 90,
      DISPLAY_ROTATION_180_DEGREES = 180,
      DISPLAY_ROTATION_270_DEGREES = 270,
    };
    enum Align
    {
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

      GROUP_ALIGN_RELATIVE  = 0x80,
      GROUP_ALIGN_HORZONTAL = 0x81,
      GROUP_ALIGN_VERTICAL  = 0x82,
    };

    enum Mode
    {
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

    enum Direction
    {
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

    class DisplayOnPageChangeTrigger;

    struct uvar
    {
      union 
      {
          uint32_t u;
          int32_t i;
          bool b;
          Align a;
          float f;
          Mode m;
          Direction d;
          SwitchPlateStyle *sps;
          Font *fnt;
          Image *img;
      };
      char t;
    };

    // ============================================================================== SwitchPlateVars

    class SwitchPlateVars
    {
      public:
        SwitchPlateVars(){};

        uvar get_variable(std::string key, bool is_missing);

        void set_variable(std::string key, uint32_t var, bool is_missing = false  ) {
          if (is_missing && has_variable(key)) {
            return;
          }
          ESP_LOGW("", "- Set uint %s to %d ", key.c_str(), var);
          this->vars_[key].u = var;
          this->vars_[key].t = 'u';
          this->set_redraw();
        }
        uint32_t get_uint(std::string key, bool is_missing = false) {
          return this->get_variable(key, is_missing).u;
        }

        void set_variable(std::string key, int32_t var, bool is_missing = false) {
          if (is_missing && has_variable(key)) {
            return;
          }
          ESP_LOGW("", "- Set int %s to %d", key.c_str(), var);
          this->vars_[key].i = var;
          this->vars_[key].t = 'i';
          this->set_redraw();
        }
        int32_t get_int(std::string key, bool is_missing = false) {
          return this->get_variable(key, is_missing).i;
        }

        void set_variable(std::string key, float var, bool is_missing = false) {
          if (is_missing && has_variable(key)) {
            return;
          }
          ESP_LOGW("", "- Set float %s to %f", key.c_str(), var);
          this->vars_[key].f = var;
          this->vars_[key].t = 'f';
          this->set_redraw();
        }
        float get_float(std::string key, bool is_missing = false) {
          return this->get_variable(key, is_missing).f;
        }


        void set_variable(std::string key, Mode var, bool is_missing = false) {
          if (is_missing && has_variable(key)) {
            return;
          }
          ESP_LOGW("", "- Set mode %s to %d", key.c_str(), var);

          this->vars_[key].m = var;
          this->vars_[key].t = 'm';
          this->set_redraw();
        }
        Mode  get_mode(std::string key, bool is_missing = false) {
          return this->get_variable(key, is_missing).m;
        }

        void set_variable(std::string key, Align var, bool is_missing = false) {
          if (is_missing && has_variable(key)) {
            return;
          }
          ESP_LOGW("", "- Set align %s to %d", key.c_str(), var);

          this->vars_[key].a =  var;
          this->vars_[key].t = 'a';
          this->set_redraw();
        }
        Align get_align(std::string key, bool is_missing = false) {
          return this->get_variable(key, is_missing).a;
        }

        void set_variable(std::string key, Direction var, bool is_missing = false) {
          if (is_missing && has_variable(key)) {
            return;
          }
          ESP_LOGW("", "- Set direction %s to %d", key.c_str(), var);

          this->vars_[key].d=  var;
          this->vars_[key].t = 'd';
          this->set_redraw();
        }
        Direction get_direction(std::string key, bool is_missing = false) {
          return this->get_variable(key, is_missing).d;
        }

        void set_variable(std::string key, SwitchPlateStyle * var, bool is_missing = false) {
          if (is_missing && has_variable(key)) {
            return;
          }
          this->vars_[key].sps =  var;
          this->vars_[key].t = 'S';
          this->set_redraw();
        }
        SwitchPlateStyle * get_style(std::string key, bool is_missing = false) {
          return this->get_variable(key, is_missing).sps; 
        }

        void set_variable(std::string key, Color var, bool is_missing = false) {
          if (is_missing && has_variable(key)) {
            return;
          }
          ESP_LOGW("", "- Set Color %s to %d", key.c_str(), var.raw_32);

          this->vars_[key].u =  var.raw_32;
          this->vars_[key].t = 'C';
          this->set_redraw();
        }
        Color get_color(std::string key, bool is_missing = false) {
          return Color(this->get_variable(key, is_missing).u);
        }

        void set_variable(std::string key, Font * var, bool is_missing = false) {
          if (is_missing && has_variable(key)) {
            return;
          }
          ESP_LOGW("", "- Set Font %s to %d", key.c_str(), -1);
          this->vars_[key].fnt =  var;
          this->vars_[key].t = 'F';
          this->set_redraw();
        }
        Font * get_font(std::string key, bool is_missing = false) {
          return this->get_variable(key, is_missing).fnt;
        }

        void set_variable(std::string key, Image * var, bool is_missing = false) {
          if (is_missing && has_variable(key)) {
            return;
          }
          ESP_LOGW("", "- Set Image %s to %d", key.c_str(), -1);
          this->vars_[key].img =  var;
          this->vars_[key].t = 'I';
          this->set_redraw();
        }
        Image * get_image(std::string key, bool is_missing = false) {
          return this->get_variable(key, is_missing).img; 
        }

        void explicit set_variable(std::string key, bool var, bool is_missing = false) {
          if (is_missing && has_variable(key)) {
            return;
          }
          ESP_LOGW("", "- Set bool %s to %s", key.c_str(), var?"true":"false");

          this->vars_[key].b = var;
          this->vars_[key].t = 'b';
          this->set_redraw();
        }
        bool get_bool(std::string key, bool is_missing = false) {
          return this->get_variable(key, is_missing).b;
        }

        bool has_variable(std::string key, char type = '*')
        {
          if ( this->vars_.count(key) != 0 )
          {
            if (type =='*') { return true; }
            uvar var = this->vars_[key];
            return var.t == type;
          }
          return false;
        }

        void parent(SwitchPlateBase *parent) { this->parent_ = parent; }
        SwitchPlateBase *parent() { return this->parent_; }

        virtual void set_redraw() =0 ;

      protected:
        SwitchPlateBase *parent_{nullptr};
        std::map<std::string, uvar> vars_ {  };
    };

    // ============================================================================== SwitchPlateStyle

    class SwitchPlateStyle : public SwitchPlateVars
    {
      public:
        SwitchPlateStyle(){};
        void draw_common(DisplayBuffer & disp, SwitchPlateBase * widget) {};
    };

    // ============================================================================== SwitchPlateBase

    class SwitchPlateBase : public SwitchPlateVars
    {
      public:
        SwitchPlateBase(){};

        virtual int16_t x() { return this->vars_["x"].i; }
        virtual int16_t y() { return this->vars_["y"].i; }
        virtual int16_t height() { return this->vars_["height"].i; }
        virtual int16_t width()  { return this->vars_["width"].i; }

        virtual void setup(){};
        virtual void call_setup(){ this->setup(); };

        virtual void show( DisplayBuffer & disp_buf ){};
        virtual void call_show( DisplayBuffer & disp_buf ){ this->show( disp_buf ); };

        virtual int16_t max_width()
        {
          if ((this->parent_ != nullptr))
          {
            return this->parent_->max_width();
          }
          return 32666;
        }

        virtual int16_t max_height()
        {
          if ((this->parent_ != nullptr))
          {
            return this->parent_->max_height();
          }
          return 32666;
        }

        virtual int16_t calc_x() {
          int x = 0;
          if ((this->parent_ != nullptr))
          {
              x = this->parent_->calc_x();
          }
          return x + this->x();
        }

        virtual int16_t calc_y(){
          int y = 0;
          if ((this->parent_ != nullptr))
          {
              y = this->parent_->calc_y();
          }
          return y + this->y();
        }

        virtual Rect get_boundry() {
          Rect r = Rect(this->calc_x(), this->calc_y(), this->calc_x()+ this->width(), this->calc_y()+this->height());
          ESP_LOGW("", "Boundry (%d,%d,%d,%d)",r.x,r.y,r.w,r.h);
          return r;
        }


        void set_visible(bool visible) { this->visible_ = visible; }
        virtual bool visible()
        {
          if ((this->parent_ != nullptr) && !this->parent_->visible())
          {
            return false;
          }
          return this->visible_;
        }

        virtual void set_redraw() { this->must_redraw_ = true;}
        virtual void clear_redraw() { this->must_redraw_ = false;}
        virtual bool need_redrawing() { return this->must_redraw_; }
        virtual Rect calc_child_clipping() { return Rect(); }

        virtual Rect calc_clipping() {
          Rect result = Rect();
          if (this->visible()) {
            if (this->need_redrawing()) {
              result = this->get_boundry();
            } else {
              result = calc_child_clipping();
              if (result.is_set()) {
                result.intersect(this->get_boundry());
              }
            }
          }
          return result;
        }

      protected:
        bool visible_ = true;
        bool must_redraw_{false};
      };

      // ============================================================================== SwitchPlate

      class SwitchPlate : public Component, public SwitchPlateBase
      {
      public:
#ifdef USE_SENSOR
        void register_sensor(sensor::Sensor *obj)
        {
          this->sensors_.push_back(obj);
        }
#endif
#ifdef USE_BINARY_SENSOR
        void register_text_sensor(text_sensor::TextSensor *obj)
        {
          this->text_sensors_.push_back(obj);
        }
#endif
#ifdef USE_TEXT_SENSOR
        void register_binary_sensor(binary_sensor::BinarySensor *obj)
        {
          this->binary_sensors_.push_back(obj);
        }
#endif
        void set_display(display::DisplayBuffer *display) {
          this->display_ = display;
          display->set_writer([this](display::DisplayBuffer &disp_buf) {
            ESP_LOGW("", "----------------------------------------------------------------------");
            this->show(disp_buf);
          });
        }
        void set_touchscreen(touchscreen::Touchscreen *touchscreen) {
          this->touchscreen_ = touchscreen;
        }

        void add_page(SwitchPlatePage *page);
        void add_headerItem(SwitchPlateItem *item);
        void add_footerItem(SwitchPlateItem *item);

        /// Get the width of the image in pixels with rotation applied.
        int max_Width();

        /// Get the height of the image in pixels with rotation applied.
        int max_Height();

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
        void show(DisplayBuffer & disp_buf) override;

        float get_setup_priority() const override { return esphome::setup_priority::DATA; }

        void set_background_color(Color color) { this->background_color_ = color; }
        Color get_background_color() { return this->background_color_; }

        void set_background_image(Image * image) { this->background_image_ = image; }
        Image * get_background_image() { return this->background_image_; }



      protected:
        /// High Frequency loop() requester used during sampling phase.
        // HighFrequencyLoopRequester high_freq_;

        display::DisplayBuffer *display_ { nullptr };


        touchscreen::Touchscreen *touchscreen_{nullptr};

        std::vector<SwitchPlateItem *> header_;
        std::vector<SwitchPlateItem *> footer_;

        bool tabview_ = false;
        Rotation rotation_{DISPLAY_ROTATION_0_DEGREES};
        SwitchPlatePage *first_page_{nullptr};
        SwitchPlatePage *current_page_{nullptr};
        SwitchPlatePage *previous_page_{nullptr};

        Color background_color_;
        Image * background_image_{nullptr};



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

      class SwitchPlateItem : public SwitchPlateBase
      {
      public:
        SwitchPlateItem() {  };

        int minValue() { return this->vars_["min_value"].i; }
        int maxValue() { return this->vars_["max_value"].i; }

        bool enabled() { return this->vars_["enabled"].b; }

        bool need_redrawing() {
          std::string txt = this->get_text_();
          int state = this->get_state_();

          if (state != this->old_state_) {
            this->old_state_ = state;
            this->must_redraw_ = true;
          }

          if (txt != this->old_text_) {
            this->old_text_ = txt;
            this->must_redraw_ = true;
          }
          return this->must_redraw_;
        }
        template <typename V> void set_text(V val) { this->text_ = val; }
        std::string text() const { return this->old_text_; }

        template <typename V> void set_state(V val) { this->state_ = val; }
        int state() const { return const_cast<SwitchPlateItem *>(this)->state_.value(this); }

        virtual void show(DisplayBuffer & disp_buf){
          ESP_LOGW("", "paint border and backround");
          return;
          uint8_t radius = get_int("border_radius");
          Rect r = this->get_boundry();
          if (this->has_variable("background_color")) {
            disp_buf.filled_rectangle(r.x, r.y, r.w, r.h, radius, this->get_color("background_color"));
          } else if (this->has_variable("background_gradient_from")) {
            Color from_color = this->get_color("background_gradient_from");
            Color to_color = this->get_color("background_gradient_from");
            GradientDirection dir = (GradientDirection) this->get_direction("background_gradient_direction");
            disp_buf.filled_rectangle(r.x, r.y, r.w, r.h, radius, from_color, to_color, dir);
          }
          if (this->has_variable("border_color")) {
            disp_buf.rectangle(r.x, r.y, r.w, r.h, radius, this->get_color("border_color"));
          } else if (this->has_variable("border_gradient_from")) {
            Color from_color = this->get_color("border_gradient_from");
            Color to_color = this->get_color("border_gradient_from");
            GradientDirection dir = (GradientDirection) this->get_direction("border_gradient_direction");
            disp_buf.filled_rectangle(r.x, r.y, r.w, r.h, radius, from_color, to_color, dir);
          }
        }

      protected:
        std::string get_text_() const { return const_cast<SwitchPlateItem *>(this)->text_.value(this); }
        int get_state_() const { return const_cast<SwitchPlateItem *>(this)->state_.value(this); }
        bool ENABLED_ = true;

        TemplatableValue<int, const SwitchPlateItem *> state_;
        int old_state_{0};

        Mode MODE_ = TEXT_MODE_CROP;
        Align ALIGN_ = LEFT;

        TemplatableValue<std::string, const SwitchPlateItem *> text_;
        std::string old_text_ {""};
      };

      // ============================================================================== SwitchPlateGroup

      class SwitchPlateGroup : public SwitchPlateBase
      {
      public:
        void add_widget(SwitchPlateBase *widget)
        {
          widget->parent(this);
          this->widgets_.push_back(widget);
        }

        virtual void call_setup()
        {
          this->setup();
          for (auto *widget : this->widgets_)
            {
              widget->call_setup();
            }
        }

        bool need_child_redrawing() {
          for (auto *widget : this->widgets_)
          {
            if (widget->need_redrawing()) { return true; };
          }
          return false;
        }

        virtual void call_show(DisplayBuffer & disp_buf) {
          if (this->visible() && !disp_buf.is_clipped(this->get_boundry())) {
            Rect r = this->calc_clipping();
            ESP_LOGW("", "Group clipping (%d,%d,%d,%d)",r.x,r.y,r.w,r.h);
            if (r.is_set()) {
              disp_buf.set_clipping(r);
              this->show(disp_buf);
              for (auto *widget : this->widgets_)
              {
                widget->call_show(disp_buf);
              }
              disp_buf.clear_clipping();
            }
          }
        }

        size_t items_size() const { return this->widgets_.size(); }
        SwitchPlateBase *item(size_t i) { return this->widgets_[i]; }

        virtual Rect calc_child_clipping() {

          Rect r = Rect();
          if (this->visible()) {
            for (auto *widget : this->widgets_)
            {
              if (widget->need_redrawing()) {
                r.join(widget->get_boundry());
              }
            }
          }
          return r;
        }
      protected:
        std::vector<SwitchPlateBase *> widgets_;
      };

      // ============================================================================== SwitchPlatePage

      class SwitchPlatePage : public SwitchPlateGroup
      {
      public:
        virtual int16_t x() { return 0; }
        virtual int16_t y() { return 0; }
        virtual int16_t height() { return this->max_height(); }
        virtual int16_t width()  { return this->max_width(); }


        void set_next(SwitchPlatePage *next);
        SwitchPlatePage *get_next() { return this->next_; }

        void set_prev(SwitchPlatePage *prev);
        SwitchPlatePage *get_prev() { return this->prev_; }

        void select()
        {
          ((SwitchPlate *)this->parent())->show_page(this);
        }

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

      class SwitchPlateLabel : public SwitchPlateItem
      {
        public:
          void setup() {
            this->set_variable("text_color", new Color(0xFFFFFF), true);
            this->set_variable("text_font", this->get_font("default_font", true), true);
            this->set_variable("border_color", new Color( 0xFF0000), true);
            this->set_variable("background_color", new Color( 0x00FF00), true);

          };
          virtual void show(DisplayBuffer & disp_buf){
            SwitchPlateItem::show(disp_buf);
            Color color = this->get_color("text_color");
            Font * font = this->get_font("text_font");
            TextAlign align = (TextAlign) this->get_align("text_align");
            //int x, int y, Font *font, Color color, TextAlign align, const char *text
            //  disp_buf.print(this->calc_x(), this->calc_y(), font, color, align, this->text().c_str());
          };
      };

/// ============================================================================== SwitchPlateFrame


      class SwitchPlateFrame : public SwitchPlateGroup
      {
        public:
          virtual void show(DisplayBuffer & disp_buf);

      };

// ============================================================================== SwitchPlateLabel

      class SwitchPlateButton : public SwitchPlateItem
      {
        public:
          void setup() {
            this->set_variable("text_color", Color((uint32_t) 0xFFFFFF), true);
          };
          virtual void show(DisplayBuffer & disp_buf){
    //       disp_buf.print(this->calc_x, this->calc_y, this->text());
          };
      };

// ============================================================================== SwitchPlateSpinner

      class SwitchPlateSpinner : public SwitchPlateItem
      {
        public:
          void setup() {
            this->set_variable("text_color", Color((uint32_t) 0xFFFFFF), true);
          };
          virtual void show(DisplayBuffer & disp_buf){
    //       disp_buf.print(this->calc_x, this->calc_y, this->text());
          };
      };

// ============================================================================== SwitchPlateSlider

      class SwitchPlateSlider : public SwitchPlateItem
      {
        public:
          void setup() {
             this->set_variable("text_color", 0xFFFFFF, true);
          };
        virtual void show(DisplayBuffer & disp_buf){
    //       disp_buf.print(this->calc_x, this->calc_y, this->text());
        };

      };



/// ==============================================================================

      template <typename... Ts>
      class SwitchPlateShowAction : public Action<Ts...>
      {
      public:
        TEMPLATABLE_VALUE(SwitchPlatePage *, page)

        void play(Ts... x) override
        {
          auto *page = this->page_.value(x...);
          if (page != nullptr)
          {
            page->select();
          }
        }
      };

      template <typename... Ts>
      class SwitchPlateShowNextAction : public Action<Ts...>
      {
      public:
        SwitchPlateShowNextAction(SwitchPlate *display) : display_(display) {}

        void play(Ts... x) override { this->display_->show_next(); }

        SwitchPlate *display_;
      };

      template <typename... Ts>
      class SwitchPlateShowHomeAction : public Action<Ts...>
      {
      public:
        SwitchPlateShowHomeAction(SwitchPlate *display) : display_(display) {}

        void play(Ts... x) override { this->display_->show_home(); }

        SwitchPlate *display_;
      };

      template <typename... Ts>
      class SwitchPlateShowPrevAction : public Action<Ts...>
      {
      public:
        SwitchPlateShowPrevAction(SwitchPlate *display) : display_(display) {}

        void play(Ts... x) override { this->display_->show_prev(); }

        SwitchPlate *display_;
      };

      template <typename... Ts>
      class SwitchPlateIsPageCondition : public Condition<Ts...>
      {
      public:
        SwitchPlateIsPageCondition(SwitchPlate *display) : display_(display) {}

        void set_page(SwitchPlatePage *page) { this->page_ = page; }
        bool check(Ts... x) override { return this->display_->current_page() == this->page_; }

      protected:
        SwitchPlate *display_;
        SwitchPlatePage *page_;
      };

      class DisplayOnPageChangeTrigger : public Trigger<SwitchPlatePage *, SwitchPlatePage *>
      {
      public:
        explicit DisplayOnPageChangeTrigger(SwitchPlate *display) { display->add_on_page_change_trigger(this); }
        void process(SwitchPlatePage *from, SwitchPlatePage *to);
        void set_from(SwitchPlatePage *p) { this->from_ = p; }
        void set_to(SwitchPlatePage *p) { this->to_ = p; }

      protected:
        SwitchPlatePage *from_{nullptr};
        SwitchPlatePage *to_{nullptr};
      };

    } // namespace empty_sensor_hub
  }   // namespace esphome