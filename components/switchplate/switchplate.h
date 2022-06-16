#pragma once
//#include "./hasp/lv_conf.h"

#include "esphome/components/display/display_buffer.h"
#include "esphome/components/touchscreen/touchscreen.h"

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/automation.h"
#include <map>

//#include "lv_conf.h"

#include "bootlogo.h"
#include "lvgl.h"

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
  namespace switch_plate
  {

    const size_t buf_pix_count = 480 * 320 / 5; ///TODO: NEED to find a better way to do this

    enum Rotation
    {
      DISPLAY_ROTATION_0_DEGREES = 0,
      DISPLAY_ROTATION_90_DEGREES = 90,
      DISPLAY_ROTATION_180_DEGREES = 180,
      DISPLAY_ROTATION_270_DEGREES = 270,
    };
    enum Align
    {
      TEXT_ALIGN_LEFT = -1,
      TEXT_ALIGN_CENTER = 0,
      TEXT_ALIGN_RIGHT = 1
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

    };
    class SwitchPlateBase;
    class SwitchPlateGroup;
    class SwitchPlatePage;

    class DisplayOnPageChangeTrigger;

    union uvar
    {
        uint32_t u;
        int32_t i;
        bool b;
        Align a;
        Mode m;
        Direction d;
    };

      // ==============================================================================

      class SwitchPlateBase
      {
      public:
        SwitchPlateBase(){};
        virtual void show(){};

        void parent(SwitchPlateBase *parent) { this->parent_ = parent; }
        SwitchPlateBase *parent() { return this->parent_; }

        virtual int maxWidth()
        {
          if ((this->parent_ != nullptr))
          {
            return this->parent_->maxWidth();
          }
          return 0;
        }
        /// Get the height of the image in pixels with rotation applied.
        virtual int maxHeight()
        {
          if ((this->parent_ != nullptr))
          {
            return this->parent_->maxHeight();
          }
          return 0;
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

      protected:
        SwitchPlateBase *parent_{nullptr};
        bool visible_ = true;
      };

      // ==============================================================================

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
        }        
        void set_touchscreen(touchscreen::Touchscreen *touchscreen) {
          this->touchscreen_ = touchscreen;
        }        
        void add_page(SwitchPlatePage *page);

        void add_headerItem(SwitchPlateBase *header);
        size_t items_size() const { return this->header_.size(); }
        SwitchPlateBase *headerItem(size_t i) { return this->header_[i]; }

        void add_footerItem(SwitchPlateBase *footer);
        size_t footer_size() const { return this->footer_.size(); }
        SwitchPlateBase *footerItem(size_t i) { return this->footer_[i]; }

        void set_tabview(bool tabview) { this->tabview_ = tabview; }
        bool set_tabview() const { return this - tabview_; }

        /// Get the width of the image in pixels with rotation applied.
        int maxWidth() { return this->display_->get_width(); }

        /// Get the height of the image in pixels with rotation applied.
        int maxHeight() { return this->display_->get_height(); };

        void select_page(SwitchPlatePage *page);

        const SwitchPlatePage *current_page() const { return this->current_page_; }

        void select_next();
        SwitchPlatePage *next();
        bool can_next();

        void select_prev();
        SwitchPlatePage *prev();
        bool can_prev();

        void show_home() { this->select_page(this->first_page_); }

        void add_on_page_change_trigger(DisplayOnPageChangeTrigger *t) { this->on_page_change_triggers_.push_back(t); }

        void setup() override;
        void dump_config() override;
        void show() override;
        
        void fill(int x1, int y1, int x2, int y2, uint16_t * pixels) {
          this->display_->fill(x1, y1, x2, y2, pixels);    
        }

        void IRAM_ATTR loop() override
        {
          lv_timer_handler(); // called by dispatch_loop
        }
        float get_setup_priority() const override { return esphome::setup_priority::DATA; }

      private:
        /// High Frequency loop() requester used during sampling phase.
        HighFrequencyLoopRequester high_freq_;

        display::DisplayBuffer *display_{nullptr};
        touchscreen::Touchscreen *touchscreen_{nullptr};

        lv_disp_draw_buf_t disp_buf;
        lv_color_t buf[buf_pix_count] ;

        std::vector<SwitchPlateBase *> header_;
        std::vector<SwitchPlateBase *> footer_;

        bool tabview_ = false;
        Rotation rotation_{DISPLAY_ROTATION_0_DEGREES};
        SwitchPlatePage *first_page_{nullptr};
        SwitchPlatePage *current_page_{nullptr};
        SwitchPlatePage *previous_page_{nullptr};

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


        void tft_splashscreen()
        {
          uint8_t fg[] = logoFgColor;
          uint8_t bg[] = logoBgColor;
          Color fgColor = Color(fg[0], fg[1], fg[2]);
          Color bgColor = Color(bg[0], bg[1], bg[2]);

          this->display_->fill(bgColor);

          int x = (maxWidth() - logoWidth) / 2;
          int y = (maxHeight() - logoHeight) / 2;
         // this->display_->image(x, y, logoImage, fgColor, bgColor);
        }

      };


/* Update the TFT - Needs to be accessible from C library */
void switchplate_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color16_t *color_p)
{
  size_t len = lv_area_get_size(area);
  SwitchPlate * switchplate = (SwitchPlate *) disp->user_data;
  // tft.setWindow(area->x1, area->y1, area->x2, area->y2);

  switchplate->fill(area->x1, area->x2, area->y1, area->y2, (uint16_t *)color_p); /* Write words at once */


  /* Tell lvgl that flushing is done */
  lv_disp_flush_ready(disp);
}

/*Read the touchpad - Needs to be accessible from C library * /
void IRAM_ATTR my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    data->state = LV_INDEV_STATE_REL;

 
  uint16_t touchX, touchY;

  bool touched = tft.getTouch(&touchX, &touchY, 600);

  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    / * Set the coordinates* /
    data->point.x = touchX;
    data->point.y = touchY;

    // Serial.print("Data x");
    // Serial.print(touchX);

    // Serial.print(" - y");
    // Serial.println(touchY);
  }

}
  */

      // ==============================================================================

      class SwitchPlateItem : public SwitchPlateBase
      {
      public:
        SwitchPlateItem(std::string type) { this->type_ = type; };

        void set_variable(std::string key, uint32_t var) {this->vars_[key].u = var;} 
        void set_variable(std::string key, int32_t var) {this->vars_[key].i = var;} 
        void set_variable(std::string key, bool var) {this->vars_[key].b = var;} 

        uint32_t varu(std::string key) {return this->vars_[key].u;}
        int32_t  vari(std::string key) {return this->vars_[key].i;}
        bool     varb(std::string key) {return this->vars_[key].b;}

        int x() { return this->vars_["x"].i; }
        int y() { return this->vars_["y"].i; }
        int height() { return this->vars_["height"].i; }
        int width()  { return this->vars_["width"].i; }

        int minValue() { return this->vars_["min_value"].i; }
        int maxValue() { return this->vars_["max_value"].i; }

        bool enabled() { return this->vars_["enabled"].b; }

        Mode mode() { return this->vars_["mode"].m; }

        Align align() { return this->vars_["align"].a; }
        Direction direction() { return this->vars_["direction"].d; }

        template <typename V> void set_text(V val) { this->text_ = val; }
        std::string text() const { return const_cast<SwitchPlateItem *>(this)->text_.value(this); }
        
        template <typename V> void set_value(V val) { this->VALUE_ = val; }
        int value() const { return const_cast<SwitchPlateItem *>(this)->VALUE_.value(this); }

      protected:
        std::string type_ = "none";
        std::map<std::string, uvar> vars_ {  }; 
        bool ENABLED_ = true;

        TemplatableValue<int, const SwitchPlateItem *> VALUE_;
        // int32_t VALUE_ = 0;

        Mode MODE_ = TEXT_MODE_CROP;
        Align ALIGN_ = TEXT_ALIGN_LEFT;

        TemplatableValue<std::string, const SwitchPlateItem *> text_;
        // std::string  text_;
        bool TOGGLE_ = false;
      };

      // ==============================================================================

      class SwitchPlateGroup : public SwitchPlateBase
      {
      public:
        void add_object(SwitchPlateBase *object)
        {
          object->parent(this);
          this->childs_.push_back(object);
        }

        void show()
        {
          if (this->visible())
          {
            for (auto *child : this->childs_)
            {
              child->show();
            }
          }
        }

        size_t items_size() const { return this->childs_.size(); }
        SwitchPlateBase *item(size_t i) { return this->childs_[i]; }

      protected:
        std::vector<SwitchPlateBase *> childs_;
      };

      // ==============================================================================

      class SwitchPlatePage : public SwitchPlateGroup
      {
      public:
        SwitchPlatePage(){};

        void next(SwitchPlatePage *next);
        SwitchPlatePage *next() { return this->next_; }

        void prev(SwitchPlatePage *prev);
        SwitchPlatePage *prev() { return this->prev_; }

        void select()
        {
          ((SwitchPlate *)this->parent())->select_page(this);
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

      // ==============================================================================

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

        void play(Ts... x) override { this->display_->show_next_page(); }

        SwitchPlate *display_;
      };

      template <typename... Ts>
      class SwitchPlateShowHomeAction : public Action<Ts...>
      {
      public:
        SwitchPlateShowHomeAction(SwitchPlate *display) : display_(display) {}

        void play(Ts... x) override { this->display_->show_home_page(); }

        SwitchPlate *display_;
      };

      template <typename... Ts>
      class SwitchPlateShowPrevAction : public Action<Ts...>
      {
      public:
        SwitchPlateShowPrevAction(SwitchPlate *display) : display_(display) {}

        void play(Ts... x) override { this->display_->show_prev_page(); }

        SwitchPlate *display_;
      };

      template <typename... Ts>
      class SwitchPlateIsPageCondition : public Condition<Ts...>
      {
      public:
        SwitchPlateIsPageCondition(SwitchPlate *display) : display_(display) {}

        void set_page(SwitchPlatePage *page) { this->page_ = page; }
        bool check(Ts... x) override { return this->display_->get_active_page() == this->page_; }

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