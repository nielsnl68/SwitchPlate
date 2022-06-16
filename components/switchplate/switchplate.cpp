#include "switchplate.h"
#include "esphome/core/log.h"
#include "lvgl.h"

namespace esphome
{
    namespace switch_plate
    {

        static const char *TAG = "SwitchPlate.component";

        void SwitchPlate::setup()
        {

            lv_init();

#if USE_LV_LOG != 0
            lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

            lv_disp_draw_buf_init(&disp_buf, buf, NULL, buf_pix_count);

            /*Initialize the display*/
            static lv_disp_drv_t disp_drv;
            lv_disp_drv_init(&disp_drv);
            disp_drv.hor_res = this->display_->get_width();
            disp_drv.ver_res = this->display_->get_height();
            disp_drv.flush_cb = &switchplate_flush_cb;
            disp_drv.draw_buf = &disp_buf;
            disp_drv.user_data = this;
            lv_disp_drv_register(&disp_drv);

            /*Initialize the input device driver* /
            static lv_indev_drv_t indev_drv;
            lv_indev_drv_init(&indev_drv);
            indev_drv.type = LV_INDEV_TYPE_POINTER;
            indev_drv.read_cb = my_touchpad_read;
            lv_indev_drv_register(&indev_drv);
            */
           
            this->high_freq_.start(); // avoid 16 ms delay
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

        void SwitchPlate::add_page(SwitchPlatePage *page)
        {
            page->parent(this);
            page->prev(this->previous_page_);
            if (this->previous_page_ != nullptr)
            {
                this->previous_page_->next(page);
            }
            this->previous_page_ = page;
            if ((this->first_page_ == nullptr) && page->get_selectable())
            {
                this->first_page_ = page;
            }
        }

        void SwitchPlate::add_headerItem(SwitchPlateBase *item)
        {
            item->parent(this);
            this->header_.push_back(item);
        }

        void SwitchPlate::add_footerItem(SwitchPlateBase *item)
        {
            item->parent(this);
            this->footer_.push_back(item);
        }

        void SwitchPlate::select_page(SwitchPlatePage *page)
        {
            if (page == nullptr)
                return;

            if (this->current_page_ != page)
            {
                this->previous_page_ = this->current_page_;
                this->current_page_ = page;
                if (this->previous_page_ != this->current_page_)
                {
                    // for (auto *t : this->on_page_change_triggers_)
                    //     t->process(this->previous_page_, this->current_page_);
                    this->show();
                }
            }
        }

        void SwitchPlate::show()
        {
            if (this->current_page_ == nullptr)
            {
                this->current_page_ = this->first_page_;
                this->previous_page_ = nullptr;
            }
            for (auto *header : this->header_)
            {
                header->show();
            }
            this->current_page_->show();
        }

        SwitchPlatePage *SwitchPlate::next()
        {
            SwitchPlatePage *page;
            if (this->current_page_ == nullptr)
            {
                page = this->first_page_;
            }
            else
            {
                page = this->current_page_->next();
                while ((page != nullptr) && !page->get_selectable())
                {
                    page = page->next();
                }
            }
            return page;
        }

        void SwitchPlate::select_next() { this->select_page(this->next()); }
        bool SwitchPlate::can_next() { return this->next() != nullptr; }

        SwitchPlatePage *SwitchPlate::prev()
        {
            SwitchPlatePage *page;
            if (this->current_page_ == nullptr)
            {
                page = this->first_page_;
            }
            else
            {
                page = this->current_page_->prev();
                while ((page != nullptr) && !page->get_selectable())
                {
                    page = page->prev();
                }
            }
            return page;
        }

        void SwitchPlate::select_prev() { this->select_page(this->prev()); }
        bool SwitchPlate::can_prev() { return this->prev() != nullptr; }

        void SwitchPlatePage::prev(SwitchPlatePage *prev) { this->prev_ = prev; }
        void SwitchPlatePage::next(SwitchPlatePage *next) { this->next_ = next; }

    } // namespace empty_sensor_hub
} // namespace esphome