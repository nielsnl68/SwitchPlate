#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/automation.h"
// #include "esphome/components/sensor/sensor.h"
// #include "esphome/components/binary_sensor/binary_sensor.h"
// #include "esphome/components/text_sensor/text_sensor.h"

namespace esphome
{
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
      CONF_ALIGN_LEFT = -1,
      CONF_ALIGN_CENTER = 0,
      CONF_ALIGN_RIGHT = 1
    };
    enum Mode 
    {
      CONF_MODE_EXPAND = 0,
      CONF_MODE_BREAK = 1,
      CONF_MODE_DOTS = 2,
      CONF_MODE_SCROLL = 3,
      CONF_MODE_LOOP = 4,
      CONF_MODE_CROP = 5
    };

    class SwitchPlateBase;
    class SwitchPlatePage;

    class DisplayOnPageChangeTrigger;

    class SwitchPlate : public Component, public SwitchPlateBase
    {
    public:
     // void register_sensor(sensor::Sensor *obj) { this->sensors_.push_back(obj); }
     // void register_text_sensor(text_sensor::TextSensor *obj) { this->text_sensors_.push_back(obj); }
     // void register_binary_sensor(binary_sensor::BinarySensor *obj) { this->binary_sensors_.push_back(obj); }

      void set_rotation(Rotation rotation) { this->rotation_ = rotation; }
      Rotation get_rotation() const { return this->rotation_; }

      void clear();

      /// Get the width of the image in pixels with rotation applied.
      int get_width();
      /// Get the height of the image in pixels with rotation applied.
      int get_height();

      void set_pages(std::vector<SwitchPlatePage *> pages);

      void show_page(SwitchPlatePage *page);
      const SwitchPlatePage *get_active_page() const { return this->page_; }

      void show_next_page();
      void show_prev_page();
      void show_home_page();

      void add_on_page_change_trigger(DisplayOnPageChangeTrigger *t) { this->on_page_change_triggers_.push_back(t); }

      void setup() override;
      void dump_config() override;


      virtual int get_height_internal() {return 0;};
      virtual int get_width_internal() { return 0;}

    protected:
     // std::vector<sensor::Sensor *> sensors_;
     // std::vector<text_sensor::TextSensor *> text_sensors_;
     // std::vector<binary_sensor::BinarySensor *> binary_sensors_;

      Rotation rotation_{DISPLAY_ROTATION_0_DEGREES} ;
      SwitchPlatePage *page_{nullptr};
      SwitchPlatePage *previous_page_{nullptr};
      std::vector<DisplayOnPageChangeTrigger *> on_page_change_triggers_;
    };

    class SwitchPlateBase
    {
    public:
      SwitchPlateBase();
      virtual void show() {};

      void set_parent(SwitchPlateBase *parent) {this->parent_ = parent;}
      SwitchPlateBase *get_parent() {return this->parent_;}
    protected:
      SwitchPlateBase *parent_{nullptr};
    };


    class SwitchPlateItem: public SwitchPlateBase
    {
    public:
      SwitchPlateItem(){};
      void set_top(int top) {this->top_ = top;}
      void set_left(int left) {this->left_ = left;}
      void set_height(int height) {this->height_ = height;}
      void set_width(int width) {this->width_ = width;}

      template<typename V> void set_text(V val) { this->text_ = val; }
      std::string get_text() const { return const_cast<SwitchPlateItem *>(this)->text_.value(this); }

    protected:
      int top_ = 0;
      int left_ = 0;
      int height_ = 0;
      int width_ = 0;
      TemplatableValue<std::string, const SwitchPlateItem *> text_;

    };

    class SwitchPlateGroup: public SwitchPlateBase
    {
    public:
      void set_objects(std::vector<SwitchPlateBase *> childs);

      void show() {
        for (auto *childs : this->childs_)
            {
              childs->show();
            }
      }

    protected:
      std::vector<SwitchPlateBase *> childs_;
    };


    class SwitchPlatePage: public SwitchPlateGroup
    {
    public:
      SwitchPlatePage(){};
      void show() override;
      SwitchPlatePage *next() {return this->next_;}
      SwitchPlatePage *prev() {return this->prev_;} 
      void set_prev(SwitchPlatePage *prev);
      void set_next(SwitchPlatePage *next);


    protected:
      SwitchPlatePage *prev_{nullptr};
      SwitchPlatePage *next_{nullptr};
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
          page->show();
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
} // namespace esphome