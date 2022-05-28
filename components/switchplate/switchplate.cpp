#include "switchplate.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace switch_plate
    {

        static const char *TAG = "SwitchPlate.component";

        void SwitchPlate::setup()
        {
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
        }

        int SwitchPlate::get_width()
        {
            switch (this->rotation_)
            {
            case DISPLAY_ROTATION_90_DEGREES:
            case DISPLAY_ROTATION_270_DEGREES:
                return this->get_height_internal();
            case DISPLAY_ROTATION_0_DEGREES:
            case DISPLAY_ROTATION_180_DEGREES:
            default:
                return this->get_width_internal();
            }
        }
        int SwitchPlate::get_height()
        {
            switch (this->rotation_)
            {
            case DISPLAY_ROTATION_0_DEGREES:
            case DISPLAY_ROTATION_180_DEGREES:
                return this->get_height_internal();
            case DISPLAY_ROTATION_90_DEGREES:
            case DISPLAY_ROTATION_270_DEGREES:
            default:
                return this->get_width_internal();
            }
        }

        void SwitchPlate::add_page(SwitchPlatePage * page)
        {
            page->set_parent(this);
            page->set_prev(this->previous_page_);
            if (this->previous_page_ != nullptr) {
                this->previous_page_->set_next(page);
            }  
            this->previous_page_ = page;
            if ((this->first_page_ == nullptr ) && page->is_Selectable()) {
                this->first_page_ = page;
            }
        }

        void SwitchPlate::add_headerItem(SwitchPlateBase * item)
        {
                item->set_parent(this);
                this->header_.push_back(item);
        }

        void SwitchPlate::add_footerItem(SwitchPlateBase * item)
        {
                item->set_parent(this);
                this->footer_.push_back(item);
        }

        void SwitchPlate::show_page(SwitchPlatePage *page)
        {
            if (page == nullptr) return;

            if (this->current_page_ != page)
            {
                this->previous_page_ = this->current_page_;
                this->current_page_ = page;
                if (this->previous_page_ != this->current_page_)
                { 
                    //for (auto *t : this->on_page_change_triggers_)
                    //    t->process(this->previous_page_, this->current_page_);
                    this->show();
                }
            }
        }

        void SwitchPlate::show() {
            if (this->current_page_ == nullptr) {
                this->current_page_ = this->first_page_;
                this->previous_page_ = nullptr;
            }
            this->current_page_->show();
        }

        SwitchPlatePage * SwitchPlate::get_next() {
            SwitchPlatePage * page;
            if (this->current_page_ == nullptr) {
                page = this->first_page_;
            } else {
                page = this->current_page_->next(); 
                while ((page != nullptr) && !page->is_Selectable()) {
                    page = page->next();
                }
            }
            return page;
        }

        void SwitchPlate::show_next() { this->show_page( this->get_next()); }
        bool SwitchPlate::can_next() { return this-> get_next() != nullptr; }


        SwitchPlatePage * SwitchPlate::get_prev() {
            SwitchPlatePage * page;
            if (this->current_page_ == nullptr) {
                page = this->first_page_;
            } else {
                page = this->current_page_->prev(); 
                while ((page != nullptr) && !page->is_Selectable()) {
                    page = page->prev();
                }
            }
            return page;
        }


        void SwitchPlate::show_prev() { this->show_page( this->get_prev()); }
        bool SwitchPlate::can_prev() { return this-> get_prev() != nullptr; }

        void SwitchPlatePage::show() {((SwitchPlate *) this->parent_)->show_page(this); }
        void SwitchPlatePage::set_prev(SwitchPlatePage *prev) { this->prev_ = prev; }
        void SwitchPlatePage::set_next(SwitchPlatePage *next) { this->next_ = next; }

    } // namespace empty_sensor_hub
} // namespace esphome