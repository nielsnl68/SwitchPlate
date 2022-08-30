#include "switchplate.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace switch_plate
    {

        static const char *TAG = "SwitchPlate";

        void SwitchPlate::setup() {};

        void SwitchPlate::call_setup()
        {
            this->setup();
            for (auto *header : this->header_)
            {
                header->call_setup();
            }
            for (auto *footer : this->footer_)
            {
                footer->call_setup();
            }

            if (this->first_page_ != nullptr)
            {
                this->first_page_->call_setup();
                if (this->first_page_ != this->current_page_)
                {
                    this->current_page_ = this->first_page_;
                }
            }
        }

        void SwitchPlate::dump_config()
        {
#ifdef USE_SENSOR
            for (auto *sensor : this->sensors_)
            {
                LOG_SENSOR("  ", "Sensor", sensor);
            }
#endif
#ifdef USE_BINARY_SENSOR
            for (auto *text_sensor : this->text_sensors_)
            {
                LOG_TEXT_SENSOR("  ", "Text sensor", text_sensor);
            }
#endif
#ifdef USE_TEXT_SENSOR
            for (auto *binary_sensor : this->binary_sensors_)
            {
                LOG_BINARY_SENSOR("  ", "Binary sensor", binary_sensor);
            }
#endif
           // for (auto [key, value] : this->vars_) {
           //     ESP_LOGCONFIG(" ", "  Variable %s(%s): %d", key, value.t, value.u);
           // }
            for (auto *header : this->header_)
            {
              //  LOG_SWITCH_PLATE_HEADER("  ", "Header", header);
            }
            for (auto *footer : this->footer_)
            {
              //  LOG_SWITCH_PLATE_FOOTER("  ", "Footer", footer);
            }
            if (this->first_page_ != nullptr)
            {
               // LOG_SWITCH_PLATE_PAGE("  ", "First page", this->first_page_);
            }
            if (this->current_page_ != nullptr)
            {
             //   LOG_SWITCH_PLATE_PAGE("  ", "Current page", this->current_page_);
            }
        }

        /// Get the width of the image in pixels with rotation applied.
        int16_t SwitchPlate::max_width() { return SwitchPlate::display_->get_width(); }

        /// Get the height of the image in pixels with rotation applied.
        int16_t SwitchPlate::max_height() { return SwitchPlate::display_->get_height(); };


        void SwitchPlate::show_page(SwitchPlatePage *page)
        {
            if (page == nullptr)
                return;

            if (this->current_page_ != page)
            {
                this->previous_page_ = this->current_page_;
                this->current_page_ = page;
                page->call_setup();
                    // for (auto *t : this->on_page_change_triggers_)
                    //     t->process(this->previous_page_, this->current_page_);
            }
        }

        void SwitchPlate::show(DisplayBuffer & disp_buf)
        {
            if (this->current_page_ == nullptr)
            {
                this->current_page_ = this->first_page_;
                this->previous_page_ = nullptr;
            }
            if (this->current_page_ == nullptr) {
                ESP_LOGW(TAG, "no page found");
                return;
            }
            disp_buf.set_clipping(Rect(0,0,this->max_width(), this->max_height()));

            if (!this->header_.empty()) {
                for (auto *header : this->header_)
                {
                    header->call_show(disp_buf);
                }
            }

            this->current_page_->call_show(disp_buf);

            if (!this->header_.empty()) {
                for (auto *footer : this->footer_)
                {
                    footer->call_show(disp_buf);
                }
            }
            disp_buf.clear_clipping();
        }

        void SwitchPlate::add_page(SwitchPlatePage *page)
        {
            page->set_parent(this);
            page->set_prev(this->previous_page_);
            if (this->previous_page_ != nullptr)
            {
                this->previous_page_->set_next(page);
            }
            this->previous_page_ = page;
            if ((this->first_page_ == nullptr) && page->get_selectable())
            {
                this->first_page_ = page;
            }
        }

        void SwitchPlate::add_headerItem(SwitchPlateItem *item)
        {
            item->set_parent(this);
            this->header_.push_back(item);
        }

        void SwitchPlate::add_footerItem(SwitchPlateItem *item)
        {
            item->set_parent(this);
            this->footer_.push_back(item);
        }


        SwitchPlatePage *SwitchPlate::get_next()
        {
            SwitchPlatePage *page;
            if (this->current_page_ == nullptr)
            {
                page = this->first_page_;
            }
            else
            {
                page = this->current_page_->get_next();
                while ((page != nullptr) && !page->get_selectable())
                {
                    page = page->get_next();
                }
            }
            return page;
        }

        void SwitchPlate::show_next() { this->show_page(this->get_next()); }
        bool SwitchPlate::can_next() { return this->get_next() != nullptr; }

        SwitchPlatePage *SwitchPlate::get_prev()
        {
            SwitchPlatePage *page;
            if (this->current_page_ == nullptr)
            {
                page = this->first_page_;
            }
            else
            {
                page = this->current_page_->get_prev();
                while ((page != nullptr) && !page->get_selectable())
                {
                    page = page->get_prev();
                }
            }
            return page;
        }

        void SwitchPlate::show_prev() { this->show_page(this->get_prev()); }
        bool SwitchPlate::can_prev() { return this->get_prev() != nullptr; }

        void SwitchPlatePage::set_prev(SwitchPlatePage *prev) { this->prev_ = prev; }
        void SwitchPlatePage::set_next(SwitchPlatePage *next) { this->next_ = next; }

        TaggedVariable SwitchPlateVars::get_variable(std::string key, bool search_parent) {
          TaggedVariable var_;
          var_.uint32_ = 0;

          if (!has_variable(key)) {
            if (search_parent && (this->parent_!= nullptr)){
              var_ = this->parent_->get_variable(key, search_parent);
            }
          } else {
            var_ = this->vars_[key];
          }
          return var_;
        }
    } // namespace switchplate
} // namespace esphome