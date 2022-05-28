#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/automation.h"

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

    class SwitchPlateGroup;
    class SwitchPlatePage;

    class DisplayOnPageChangeTrigger;


    class SwitchPlateBase
    {
    public:
      SwitchPlateBase(){};
      virtual void show(){};

      void set_parent(SwitchPlateBase *parent) { this->parent_ = parent; }
      SwitchPlateBase *get_parent() { return this->parent_; }

    protected:
      SwitchPlateBase *parent_{nullptr};
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
      void add_page(SwitchPlatePage * page);

      void add_headerItem(SwitchPlateBase * header);
      size_t items_size() const { return this->header_.size(); }
      SwitchPlateBase *get_headerItem(size_t i) { return this->header_[i]; }
      
      void add_footerItem(SwitchPlateBase * footer);
      size_t footer_size() const { return this->footer_.size(); }
      SwitchPlateBase *get_footerItem(size_t i) { return this->footer_[i]; }
      //
      //

      void set_rotation(Rotation rotation) { this->rotation_ = rotation; }
      Rotation get_rotation() const { return this->rotation_; }

      void set_tabview(bool tabview) {this->tabview_ = tabview; }
      bool is_tabview() const { return this-tabview_; }


      void clear();

      /// Get the width of the image in pixels with rotation applied.
      int get_width();
      /// Get the height of the image in pixels with rotation applied.
      int get_height();

      void show_page(SwitchPlatePage *page);

      const SwitchPlatePage *current_page() const { return this->current_page_; }
      
      void show_next();
      SwitchPlatePage * get_next();
      bool can_next();

      void show_prev();
      SwitchPlatePage * get_prev();
      bool can_prev();

      void show_home() { this->show_page(this->first_page_);}

      void add_on_page_change_trigger(DisplayOnPageChangeTrigger *t) { this->on_page_change_triggers_.push_back(t); }

      void setup() override;
      void dump_config() override;
      void show() override;

      virtual int get_height_internal() { return 0; };
      virtual int get_width_internal() { return 0; }

    protected:
      std::vector<SwitchPlateBase *> header_;
      std::vector<SwitchPlateBase *> footer_;

      bool tabview_ = false;
      Rotation rotation_{DISPLAY_ROTATION_0_DEGREES};
      SwitchPlatePage * first_page_ {nullptr};
      SwitchPlatePage * current_page_ {nullptr};
      SwitchPlatePage * previous_page_{nullptr};

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

    // ==============================================================================

    class SwitchPlateItem : public SwitchPlateBase
    {
    public:
      SwitchPlateItem(){};
      void set_x(int x) { this->top_ = x; }
      void set_y(int y) { this->left_ = y; }
      void set_height(int height) { this->height_ = height; }
      void set_width(int width) { this->width_ = width; }


      void set_visible(bool VISIBLE) { this->VISIBLE_ =  VISIBLE;}
      void set_enabled(bool ENABLED)  { this->ENABLED_ =  ENABLED;}

      void set_min_value(int MIN_VALUE) { this->MIN_VALUE_ =  MIN_VALUE;}
      void set_max_value(int MAX_VALUE) { this->MAX_VALUE_ =  MAX_VALUE;}
      
      void set_value(int32_t val) { this->VALUE_ = val; }

      //template<typename V> void set_value(V val) { this->VALUE_ = val; }
      // int32_t get_value() const { return const_cast<SwitchPlateItem *>(this)->VALUE_.value(this); }

      void set_mode(Mode MODE) { this->MODE_ =  MODE;}
      void set_align(Align ALIGN) { this->ALIGN_ =  ALIGN;}

      void set_toggle(bool TOGGLE) { this->TOGGLE_ =  TOGGLE;}

      template <typename V> void set_text(V val) { this->text_ = val; }
      //void set_text(std::string val) { this->text_ = val; }
      // std::string get_text() const { return const_cast<SwitchPlateItem *>(this)->text_.value(this); }

    protected:
      int top_ = 0;
      int left_ = 0;
      int height_ = 0;
      int width_ = 0;

      bool VISIBLE_ = true;
      bool ENABLED_ = true;

      int MIN_VALUE_ = 0;
      int MAX_VALUE_ = 100;
      //TemplatableValue<int32_t, const SwitchPlateItem *> VALUE_;
      int32_t VALUE_ = 0;      

      Mode MODE_ = CONF_MODE_CROP;
      Align ALIGN_ = CONF_ALIGN_LEFT;
      TemplatableValue<std::string, const SwitchPlateItem *> text_;      
      //std::string  text_;
      bool TOGGLE_ = false;
    };

    // ==============================================================================

    class SwitchPlateGroup : public SwitchPlateBase
    {
    public:
      void add_object(SwitchPlateBase * object)
      {
          object->set_parent(this);
          this->childs_.push_back (object);
      }

      void show()
      {
        for (auto *childs : this->childs_)
        {
          childs->show();
        }
      }
      
      size_t items_size() const { return this->childs_.size(); }
      SwitchPlateBase *get_item(size_t i) { return this->childs_[i]; }

    protected:
      std::vector<SwitchPlateBase *> childs_;
    };

    // ==============================================================================

    class SwitchPlatePage : public SwitchPlateGroup
    {
    public:
      SwitchPlatePage(){};
      void show() override;

      void set_next(SwitchPlatePage *next);
      SwitchPlatePage *next() { return this->next_; }

      void set_prev(SwitchPlatePage *prev);
      SwitchPlatePage *prev() { return this->prev_; }

      void set_selectable(bool selectable) { this->selectable_ = selectable; }
      bool is_Selectable () {return this->selectable_ ;}
      void set_name(std::string name) { this->name_ = name; }
      std::string get_name() {return this->name_; }

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