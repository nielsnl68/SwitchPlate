#include "switchplate.h"
#include "esphome/core/log.h"

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
#ifdef USE_SENSOR
  for (auto *sensor : this->sensors_) {
    LOG_SENSOR("  ", "Sensor", sensor);
  }
#endif
#ifdef USE_BINARY_SENSOR
  for (auto *text_sensor : this->text_sensors_) {
    LOG_TEXT_SENSOR("  ", "Text sensor", text_sensor);
  }
#endif
#ifdef USE_TEXT_SENSOR
  for (auto *binary_sensor : this->binary_sensors_) {
    LOG_BINARY_SENSOR("  ", "Binary sensor", binary_sensor);
  }
#endif
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
    // for (auto *t : this->on_page_change_triggers_)
    //     t->process(this->previous_page_, this->current_page_);
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
    for (auto *header : this->header_) {
      header->call_show();
    }
  }

  if (!this->header_.empty()) {
    for (auto *footer : this->footer_) {
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
  if ((this->first_page_ == nullptr) && page->get_selectable()) {
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

SwitchPlatePage *SwitchPlate::get_next() {
  SwitchPlatePage *page;
  if (this->current_page_ == nullptr) {
    page = this->first_page_;
  } else {
    page = this->current_page_->get_next();
    while ((page != nullptr) && !page->get_selectable()) {
      page = page->get_next();
    }
  }
  return page;
}

void SwitchPlate::show_next() { this->show_page(this->get_next()); }
bool SwitchPlate::can_next() { return this->get_next() != nullptr; }

void SwitchPlate::touch(TouchPoint tpoint) {
  ESP_LOGD("SwitchPlate","=====> A (%3d, %3d) %3d", tpoint.x, tpoint.y, (uint8_t)this->touch_info_.state);
  if (tpoint.x == 32666) {
    if ((this->touch_info_.state != SwitchPlateTouchState::Released) && (this->touch_info_.origin != nullptr)) {
      if ((this->touch_info_.state != SwitchPlateTouchState::Dragging) &&(this->touch_info_.destiny != nullptr)) {
        this->touch_info_.state = SwitchPlateTouchState::Dropping;
        this->touch_info_.destiny->check_touch(this->touch_info_, Rect(0, 0, this->screen_width(), this->screen_height()));
        this->touch_info_.destiny = nullptr;
      }
      ESP_LOGD("SwitchPlate","=====> B");
      this->touch_info_.state = SwitchPlateTouchState::Released;

      this->touch_info_.origin->check_touch(this->touch_info_, Rect(0, 0, this->screen_width(), this->screen_height()));
    } else {
      this->touch_info_.state = SwitchPlateTouchState::Released;
      ESP_LOGD("SwitchPlate","=====> C");
    }
    this->touch_info_.origin = nullptr;
  } else if ((this->touch_info_.state == SwitchPlateTouchState::Released)) {
    this->touch_info_.x = tpoint.x;
    this->touch_info_.y = tpoint.y;
    this->touch_info_.origin_x = tpoint.x;
    this->touch_info_.origin_y = tpoint.y;
    this->touch_info_.start_now = millis();
    if (this->current_page_ != nullptr) {
      this->touch_info_.state = SwitchPlateTouchState::Pressed;
      ESP_LOGD("SwitchPlate","=====> D");
      this->touch_info_.origin =
          this->current_page_->check_touch(this->touch_info_, Rect(0, 0, this->screen_width(), this->screen_height()));
      if (this->touch_info_.origin == nullptr) {
        this->touch_info_.state = SwitchPlateTouchState::Ignore;
        ESP_LOGE("SwitchPlate","=====> E");
      } else {
        ESP_LOGD("SwitchPlate","=====> F");
      }
    } else {
        ESP_LOGW("SwitchPlate","=====> I");
    }
  } else if (this->touch_info_.state == SwitchPlateTouchState::Ignore) {
    // do noting anymore until wait touch release
  } else {
    this->touch_info_.x = tpoint.x;
    this->touch_info_.y = tpoint.y;

    if (((abs(this->touch_info_.origin_x - tpoint.x) >= CONST_TOUCH_TO_MOVE) ||
        (abs(this->touch_info_.origin_y - tpoint.y) >= CONST_TOUCH_TO_MOVE)) &&
        (this->touch_info_.state == SwitchPlateTouchState::Ignore)) {
      this->touch_info_.state = SwitchPlateTouchState::Moving;
      ESP_LOGD("SwitchPlate","=====> G");
    } else {
      ESP_LOGD("SwitchPlate","=====> H");
    }
    auto * check =
      this->current_page_->check_touch(this->touch_info_, Rect(0, 0, this->screen_width(), this->screen_height()));
  }
};

SwitchPlatePage *SwitchPlate::get_prev() {
  SwitchPlatePage *page;
  if (this->current_page_ == nullptr) {
    page = this->first_page_;
  } else {
    page = this->current_page_->get_prev();
    while ((page != nullptr) && !page->get_selectable()) {
      page = page->get_prev();
    }
  }
  return page;
}

void SwitchPlate::show_prev() { this->show_page(this->get_prev()); }
bool SwitchPlate::can_prev() { return this->get_prev() != nullptr; }

void SwitchPlatePage::set_prev(SwitchPlatePage *prev) { this->prev_ = prev; }
void SwitchPlatePage::set_next(SwitchPlatePage *next) { this->next_ = next; }

void SwitchPlateStyle::set_redraw() {
  if (parent_ != nullptr) {
    parent_->set_redraw();
  }
}

void SwitchPlateItem::show_image(int x, int y, Image *image, Color color_on, Color color_off) {
  /*
  switch (image->get_type()) {
    case IMAGE_TYPE_BINARY:
      for (int img_x = 0; img_x < image->get_width(); img_x++) {
        for (int img_y = 0; img_y < image->get_height(); img_y++) {
          this->draw_pixel_at(x + img_x, y + img_y, image->get_pixel(img_x, img_y) ? color_on : color_off);
        }
      }
      break;
    case IMAGE_TYPE_GRAYSCALE:
      for (int img_x = 0; img_x < image->get_width(); img_x++) {
        for (int img_y = 0; img_y < image->get_height(); img_y++) {
          this->draw_pixel_at(x + img_x, y + img_y, image->get_grayscale_pixel(img_x, img_y));
        }
      }
      break;
    case IMAGE_TYPE_RGB24:
      for (int img_x = 0; img_x < image->get_width(); img_x++) {
        for (int img_y = 0; img_y < image->get_height(); img_y++) {
          this->draw_pixel_at(x + img_x, y + img_y, image->get_color_pixel(img_x, img_y));
        }
      }
      break;
    case IMAGE_TYPE_TRANSPARENT_BINARY:
      for (int img_x = 0; img_x < image->get_width(); img_x++) {
        for (int img_y = 0; img_y < image->get_height(); img_y++) {
          if (image->get_pixel(img_x, img_y))
            this->draw_pixel_at(x + img_x, y + img_y, color_on);
        }
      }
      break;
    case IMAGE_TYPE_RGB565:
      for (int img_x = 0; img_x < image->get_width(); img_x++) {
        for (int img_y = 0; img_y < image->get_height(); img_y++) {
          this->draw_pixel_at(x + img_x, y + img_y, image->get_rgb565_pixel(img_x, img_y));
        }
      }
      break;
  }
  */
}


//SwitchPlateBase::thema_ = new SwitchPlateStyle();

}  // namespace switch_plate
}  // namespace esphome