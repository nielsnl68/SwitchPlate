#pragma once

#include "esphome/components/switchplate/switchplate.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/core/defines.h"

namespace esphome {
namespace switch_plate {
/*

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
*/

}  // namespace switch_plate
}  // namespace esphome
