#include "switchplate.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "switch/widget_switch.h"
#include "binary_sensor/widget_binary_sensor.h"

namespace esphome {
namespace switch_plate {

void SwitchPlate::setup(){};

void SwitchPlate::call_setup() {
  this->setup();
  for (auto *header : this->header_) {
    header->call_setup();
  }
  for (auto *footer : this->footer_) {
    footer->call_setup();
  }

  if (this->first_page_ != nullptr) {
    this->first_page_->call_setup();
    if (this->first_page_ != this->current_page_) {
      this->current_page_ = this->first_page_;
    }
  }
}

void SwitchPlate::dump_config() {
  // for (auto [key, value] : this->vars_) {
  //     ESP_LOGCONFIG(" ", "  Variable %s(%s): %d", key, value.t, value.u);
  // }
  for (auto *header : this->header_) {
    //  LOG_SWITCH_PLATE_HEADER("  ", "Header", header);
  }
  for (auto *footer : this->footer_) {
    //  LOG_SWITCH_PLATE_FOOTER("  ", "Footer", footer);
  }
  if (this->first_page_ != nullptr) {
    // LOG_SWITCH_PLATE_PAGE("  ", "First page", this->first_page_);
  }
  if (this->current_page_ != nullptr) {
    //   LOG_SWITCH_PLATE_PAGE("  ", "Current page", this->current_page_);
  }
}

void SwitchPlate::show_page(SwitchPlatePage *page) {
  if (page == nullptr)
    return;

  if (this->current_page_ != page) {
    this->previous_page_ = this->current_page_;
    this->current_page_ = page;
    page->call_setup();
  }
}

void SwitchPlate::show() {
  if (this->current_page_ == nullptr) {
    this->current_page_ = this->first_page_;
    this->previous_page_ = nullptr;
  }
  if (this->current_page_ == nullptr) {
    ESP_LOGW(TAG, "no page found");
    return;
  }
  display()->push_clipping(Rect(0, 0, this->screen_width(), this->screen_height()));

  this->current_page_->call_show();

  if (!this->header_.empty()) {
    //ESP_LOGW(TAG, "show headers");
    display()->filled_rectangle(0,0,this->screen_width(),this->header_height_,Color(0x000088));
    for (auto *header : this->header_) {
      header->need_redrawing();
      header->call_show();
    }
  }

  if (!this->header_.empty()) {
    for (auto *footer : this->footer_) {
      footer->need_redrawing();
      footer->call_show();
    }
  }
  display()->pop_clipping();
}

void SwitchPlate::add_page(SwitchPlatePage *page) {
  page->set_parent(this);
  page->set_prev(this->previous_page_);
  if (this->previous_page_ != nullptr) {
    this->previous_page_->set_next(page);
  }
  this->previous_page_ = page;
  if ((this->first_page_ == nullptr) && page->is_selectable()) {
    this->first_page_ = page;
  }
}

void SwitchPlate::add_header_widget(SwitchPlateItem *item) {
  item->set_parent(this);
  item->set_display(this->display_);
  item->set_switchplate(this);
  this->header_.push_back(item);
}

void SwitchPlate::add_footer_widget(SwitchPlateItem *item) {
  item->set_parent(this);
  item->set_display(this->display_);
  item->set_switchplate(this);

  this->footer_.push_back(item);
}

void SwitchPlate::show_prev() { this->show_page(this->get_prev()); }
bool SwitchPlate::can_prev() { return this->get_prev() != nullptr; }

SwitchPlatePage *SwitchPlate::get_prev() {
  SwitchPlatePage *page;
  if (this->current_page_ == nullptr) {
    page = this->first_page_;
  } else {
    page = this->current_page_->get_prev();
    while ((page != nullptr) && !page->is_selectable()) {
      page = page->get_prev();
    }
  }
  return page;
}

void SwitchPlate::show_next() { this->show_page(this->get_next()); }
bool SwitchPlate::can_next() { return this->get_next() != nullptr; }

SwitchPlatePage *SwitchPlate::get_next() {
  SwitchPlatePage *page;
  if (this->current_page_ == nullptr) {
    page = this->first_page_;
  } else {
    page = this->current_page_->get_next();
    while ((page != nullptr) && !page->is_selectable()) {
      page = page->get_next();
    }
  }
  return page;
}


void SwitchPlate::touch(TouchPoint tpoint) {
  ESP_LOGV("SwitchPlate","=====> A (%3d, %3d) %3d", tpoint.x, tpoint.y, (uint8_t)this->touch_info_.state);
  if (tpoint.x == 32666) {
    if ((this->touch_info_.state != TouchState::Released) && (this->touch_info_.origin != nullptr)) {
      if ((this->touch_info_.state != TouchState::Dragging) &&(this->touch_info_.destiny != nullptr)) {
        ESP_LOGD("SwitchPlate","=====> B");
        this->touch_info_.state = TouchState::Dropping;
        this->touch_info_.destiny->check_touch(this->touch_info_, Rect(0, 0, this->screen_width(), this->screen_height()));
        this->touch_info_.destiny = nullptr;
      }
      this->touch_info_.state = TouchState::Released;

      this->touch_info_.origin->check_touch(this->touch_info_, Rect(0, 0, this->screen_width(), this->screen_height()));
    } else {
      this->touch_info_.state = TouchState::Released;
      ESP_LOGV("SwitchPlate","=====> C");
    }
    this->touch_info_.origin = nullptr;
  } else if ((this->touch_info_.state == TouchState::Released)) {
    this->touch_info_.x = tpoint.x;
    this->touch_info_.y = tpoint.y;
    this->touch_info_.origin_x = tpoint.x;
    this->touch_info_.origin_y = tpoint.y;
    this->touch_info_.start_now = millis();
    if (this->current_page_ != nullptr) {
      this->touch_info_.state = TouchState::Pressed;
      ESP_LOGV("SwitchPlate","=====> D");
      this->touch_info_.origin =
          this->current_page_->check_touch(this->touch_info_, Rect(0, 0, this->screen_width(), this->screen_height()));
      if (this->touch_info_.origin == nullptr) {
        this->touch_info_.state = TouchState::Ignore;
        ESP_LOGV("SwitchPlate","=====> E");
      } else {
        ESP_LOGV("SwitchPlate","=====> F");
      }
    } else {
        ESP_LOGW("SwitchPlate","=====> I");
    }
  } else if (this->touch_info_.state != TouchState::Ignore) {
    this->touch_info_.x = tpoint.x;
    this->touch_info_.y = tpoint.y;

    if (((abs(this->touch_info_.origin_x - tpoint.x) >= CONST_TOUCH_TO_MOVE) ||
        (abs(this->touch_info_.origin_y - tpoint.y) >= CONST_TOUCH_TO_MOVE)) &&
        (this->touch_info_.state == TouchState::Ignore)) {
      this->touch_info_.state = TouchState::Moving;
      ESP_LOGV("SwitchPlate","=====> G");
    } else {
      ESP_LOGV("SwitchPlate","=====> H");
    }
    auto * check =
      this->current_page_->check_touch(this->touch_info_, Rect(0, 0, this->screen_width(), this->screen_height()));
  }
};

void SwitchPlatePage::set_prev(SwitchPlatePage *prev) { this->prev_ = prev; }
void SwitchPlatePage::set_next(SwitchPlatePage *next) { this->next_ = next; }

void SwitchPlateItem::show_image(int16_t offset_x, int16_t offset_y, Image *image, Color color_on, Color color_off) {
  Color color;
  int16_t width = image->get_width(), height = image->get_height();   // NOLINT
  Rect r = this->get_boundry();  // NOLINT
  //r.info("widget");
  int16_t x_org = r.x, y_org = r.y;
  Rect clip = display()->get_clipping();
  //clip.info("clip");
  r.substract(clip);
  r.info("Cliped widget");
  //ESP_LOGI(TAG,"x_org:%d , y_org:%d | offset_x:%d, offset_x:%d | width:%d, height:%d ",offset_x, offset_y, x_org, y_org, height, width);
  x_org = x_org - r.x;
  y_org = y_org - r.y;
  //ESP_LOGI(TAG,"new x_org: %d, new y_org: %d ", x_org, y_org);
  offset_x = offset_x - x_org; offset_y = offset_y - y_org;
  if (width- offset_x > r.w) { width = r.w; }
  if (height- offset_y > r.h) { height = r.h; }
  //ESP_LOGI(TAG,"offset_x:%d, offset_y:%d | width:%d, height:%d ", offset_x, offset_y, width, height);

  for (int16_t img_x = 0; img_x < width; img_x++) {
    if ((img_x < 0) || (img_x >= image->get_width())) {
      ESP_LOGE(TAG,"SKIP img_x:%d * %d * %d", offset_x, img_x, image->get_width());
      break;
    }
    for (int16_t img_y = 0; img_y < height; img_y++) {
      if ((img_y < 0) || (img_y >= image->get_height())) {
        ESP_LOGE(TAG,"SKIP img_y: %d * %d * %d", offset_y, img_y, image->get_height());
        break;
      }
      switch (image->get_type()) {
        case display::IMAGE_TYPE_BINARY:
          color = image->get_pixel(offset_x+ img_x, offset_y+ img_y) ? color_on : color_off;
          break;
        case display::IMAGE_TYPE_TRANSPARENT_BINARY:
          if (image->get_pixel(img_x, img_y)){
            color =  color_on;
          } else {
            continue;
          }
          break;
        case display::IMAGE_TYPE_GRAYSCALE:
          color = image->get_grayscale_pixel(offset_x+ img_x,offset_y+  img_y);
          break;
        case display::IMAGE_TYPE_RGB24:
          color = image->get_color_pixel(offset_x+ img_x, offset_y+ img_y);
          break;
        case display::IMAGE_TYPE_RGB565:
          color = image->get_rgb565_pixel(offset_x+ img_x,offset_y+  img_y);
          break;
      }
      //ESP_LOGI(TAG,"x:%d y:%d ",r.x + (img_x -x), r.y + (img_y-y));
      display()->draw_pixel_at(r.x + (img_x), r.y + (img_y), color);
    }
    App.feed_wdt();
  }
}

void SwitchPlateItem::set_disable_style() {
  this->set_style(Style::BORDER_COLOR | Style::DISABLE, Color(0x000000), true);
  this->set_style(Style::IMAGE | Style::COLOR | Style::DISABLE, Color(0x777777), true);
  this->set_style(Style::TEXT_COLOR | Style::DISABLE, Color(0x777777), true);
  this->set_style(Style::BACKGROUND_COLOR | Style::DISABLE, Color(0x000000), true);
 // if ((action_ == DoAction::SHOW_NEXT)) status_.disabled = this.plate()->current_page()->can_next()?0:1;
 //if ((action_ == DoAction::SHOW_PREV)) status_.disabled = this.plate()->current_page()->can_prev()?0:1;
}

void SwitchPlateItem::update_switches(bool state) {

  for (auto *t : bridges_) {
    switch (t->get_bridge() ) {
#ifdef USE_SWITCH
      case BRIDGE_SWITCH:
        ((WidgetSwitch *) t)->publish_state(state);
        break;
#endif
#ifdef USE_BINARY_SENSOR
      case BRIDGE_BINARY:
        ((WidgetBinarySensor *) t)->publish_state(state);
        break;
#endif
    }
  }
}




}  // namespace switch_plate
}  // namespace esphome