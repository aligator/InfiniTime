#include "Iot.h"

#include "displayapp/screens/Screen.h"
#include "displayapp/screens/Symbols.h"
#include "components/ble/IotService.h"
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
}

Iot::Iot(DisplayApp* app, Pinetime::Controllers::IotService& iot) : Screen(app), iotService(iot) {
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
  if (event == LV_EVENT_CLICKED) {
    if (obj == light1Toggle) {
      iotService.event(Controllers::IotService::EVENT_BUTTON_TOGGLE);
    }
  }
}