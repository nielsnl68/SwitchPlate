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
            /*
            for (auto *sensor : this->sensors_)
            {
                LOG_SENSOR("  ", "Sensor", sensor);
            }

            for (auto *text_sensor : this->text_sensors_)
            {
                LOG_TEXT_SENSOR("  ", "Text sensor", text_sensor);
            }

            for (auto *binary_sensor : this->binary_sensors_)
            {
                LOG_BINARY_SENSOR("  ", "Binary sensor", binary_sensor);
            }
            */
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

        void SwitchPlate::set_pages(std::vector<SwitchPlatePage *> pages)
        {
            for (auto *page : pages)
                page->set_parent(this);

            for (uint32_t i = 0; i < pages.size() - 1; i++)
            {
                pages[i]->set_next(pages[i + 1]);
                pages[i + 1]->set_prev(pages[i]);
            }
            pages[0]->set_prev(pages[pages.size() - 1]);
            pages[pages.size() - 1]->set_next(pages[0]);
            this->show_page(pages[0]);
        }

        void SwitchPlate::show_page(SwitchPlatePage *page)
        {
            this->previous_page_ = this->page_;
            this->page_ = page;
            if (this->previous_page_ != this->page_)
            {
                for (auto *t : on_page_change_triggers_)
                    t->process(this->previous_page_, this->page_);
                this->show();
            }

        }
        
        void SwitchPlate::show_next_page() { this->show_page( this->page_->next()); }
        void SwitchPlate::show_prev_page() { this->show_page( this->page_->prev()); }

        void SwitchPlateBase::set_parent(SwitchPlateBase *parent) { this->parent_ = parent; }

        void SwitchPlatePage::show() {((SwitchPlate *) this->parent_)->show_page(this); }
        void SwitchPlatePage::set_prev(SwitchPlatePage *prev) { this->prev_ = prev; }
        void SwitchPlatePage::set_next(SwitchPlatePage *next) { this->next_ = next; }

    } // namespace empty_sensor_hub
} // namespace esphome