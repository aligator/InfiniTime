#pragma once

#include <FreeRTOS.h>
#include <lvgl/src/lv_core/lv_obj.h>
#include <string>
#include "displayapp/screens/Screen.h"


namespace Pinetime {
  namespace Controllers {
    class IotService;
  }

  namespace Applications {
    namespace Screens {
      class Iot : public Screen {
      public:
        Iot(DisplayApp* app, Pinetime::Controllers::IotService& iot);
        ~Iot() override;
        void Refresh() override;
        void OnEvent(lv_obj_t* obj, lv_event_t event);

      private:
        lv_obj_t *light1Toggle;

        Pinetime::Controllers::IotService& iotService;
      };
    }
  }
}
