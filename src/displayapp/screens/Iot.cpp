#include "Iot.h"

#include "displayapp/screens/Screen.h"
#include "displayapp/screens/Symbols.h"
#include <lvgl/lvgl.h>
#include "projdefs.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include <tuple>

using namespace Pinetime::Applications::Screens;

namespace {
  void eventHandler(lv_obj_t* obj, lv_event_t event) {
    auto* screen = static_cast<Iot*>(obj->user_data);
    screen->OnEvent(obj, event);
  }

  lv_obj_t* createLabel(const char* name, lv_obj_t* reference, lv_align_t align, lv_font_t* font, uint8_t x, uint8_t y) {
    lv_obj_t* label = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, font);
    lv_obj_set_style_local_text_color(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_label_set_text(label, name);
    lv_obj_align(label, reference, align, x, y);

    return label;
  }
}


Iot::Iot(DisplayApp* app) : Screen(app) {
  light1Toggle = lv_btn_create(lv_scr_act(), nullptr);
  light1Toggle->user_data = this;
  lv_obj_set_event_cb(light1Toggle, eventHandler);
  lv_obj_set_size(light1Toggle, 115, 50);
  lv_obj_align(light1Toggle, lv_scr_act(), LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
  lv_obj_set_style_local_value_str(light1Toggle, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, Applications::Screens::Symbols::brightnessMedium);
}

Iot::~Iot() {
  lv_obj_clean(lv_scr_act());
}

void Iot::Refresh() {
}

void Iot::OnEvent(lv_obj_t* obj, lv_event_t event) {
  switch (event) {
    case LV_EVENT_CLICKED: {
      if (obj == light1Toggle) {
        light1Toggle->user_data = this;
        // TODO(aligator) Implement
      }
      break;
    }
    default:
      break;
  }
}

bool Iot::OnButtonPushed() {
  return true;
}
