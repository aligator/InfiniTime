/*  Copyright (C) 2022 Johannes Hörmann

    This file is part of InfiniTime.

    InfiniTime is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    InfiniTime is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "components/ble/IotService.h"
#include "systemtask/SystemTask.h"

namespace {
  // 1000yyxx-78fc-48fe-8e23-433b3a1942d0
  constexpr ble_uuid128_t CharUuid(uint8_t x, uint8_t y) {
    return ble_uuid128_t{
      .u = {.type = BLE_UUID_TYPE_128},
      .value =  { 0xd0, 0x42, 0x19, 0x3a, 0x3b, 0x43, 0x23, 0x8e, 0xfe, 0x48, 0xfc, 0x78, x, y, 0x00, 0x10 }
    };
  }

  // 10000000-78fc-48fe-8e23-433b3a1942d0
  constexpr ble_uuid128_t BaseUuid() {
    return CharUuid(0x00, 0x00);
  }

  constexpr ble_uuid128_t iotsUuid {BaseUuid()};

  // 10000001-78fc-48fe-8e23-433b3a1942d0
  constexpr ble_uuid128_t iotsEventCharUuid {CharUuid(0x01, 0x00)};

  int IotCallback(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt* ctxt, void* arg) {
    return static_cast<Pinetime::Controllers::IotService*>(arg)->OnCommand(conn_handle, attr_handle, ctxt);
  }
}

Pinetime::Controllers::IotService::IotService(Pinetime::System::SystemTask& system) : m_system(system) {
  characteristicDefinition[0] = {.uuid = &iotsEventCharUuid.u,
                               .access_cb = IotCallback,
                               .arg = this,
                               .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
                               .val_handle = &eventHandle};
  characteristicDefinition[1] = {0};

  serviceDefinition[0] = {
    .type = BLE_GATT_SVC_TYPE_PRIMARY, .uuid = &iotsUuid.u, .characteristics = characteristicDefinition};
  serviceDefinition[1] = {0};
}

void Pinetime::Controllers::IotService::Init() {
  uint8_t res = 0;
  res = ble_gatts_count_cfg(serviceDefinition);
  ASSERT(res == 0);

  res = ble_gatts_add_svcs(serviceDefinition);
  ASSERT(res == 0);
}

int Pinetime::Controllers::IotService::OnCommand(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt* ctxt) {
  if (ctxt->op == BLE_GATT_ACCESS_OP_WRITE_CHR) {
    size_t notifSize = OS_MBUF_PKTLEN(ctxt->om);
    char data[notifSize + 1];
    data[notifSize] = '\0';
    os_mbuf_copydata(ctxt->om, 0, notifSize, data);
    char* s = &data[0];
    //if (ble_uuid_cmp(ctxt->chr->uuid, &iotsEventCharUuid.u) == 0) {
      // TODO(aligator)
    //}
  }
  return 0;
}

// TODO(aligator)
bool Pinetime::Controllers::IotService::getButtonState() const {
  return false;
}

void Pinetime::Controllers::IotService::event(char event) {
  uint8_t buffer[2] = {event, 1}; // [0] = event type, [1] = payload value (e.g. button index)
  auto* om = ble_hs_mbuf_from_flat(buffer, 2);

  uint16_t connectionHandle = m_system.nimble().connHandle();

  if (connectionHandle == 0 || connectionHandle == BLE_HS_CONN_HANDLE_NONE) {
    return;
  }

  ble_gattc_notify_custom(connectionHandle, eventHandle, om);
}
