#pragma once

#include "displayapp/screens/Screen.h"
#include "components/datetime/DateTimeController.h"
#include "displayapp/LittleVgl.h"

#include "FreeRTOS.h"
#include "portmacro_cmsis.h"

#include <array>
#include "systemtask/SystemTask.h"

namespace Pinetime::Applications::Screens {

  class Iot : public Screen {
  public:
    Iot(DisplayApp* app);
    ~Iot() override;
    void Refresh() override;
    void OnEvent(lv_obj_t* obj, lv_event_t event);
    bool OnButtonPushed() override;

  private:
    lv_obj_t *light1Toggle;
  };
}
