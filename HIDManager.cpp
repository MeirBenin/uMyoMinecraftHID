
#include "HIDManager.h"

HIDManager::HIDManager(void)
{
  board_init();
  tusb_init();
  macOSInit();
  update();
}

void HIDManager::update()
{
  tud_task();
  if (!timeIntervalElapsed())
    return;
  if (tud_suspended())
  {
    tud_remote_wakeup();
    return;
  }
  if (!tud_hid_ready())
    return;

  if (m_activeKeyboard)
  {
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, m_keycodeArray);
    m_activeKeyboard = false;
    m_lastPressed = true;
  }
  else if (m_lastPressed)
  {
    m_keycodeArray[0] = 0;
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, m_keycodeArray);
    m_lastPressed = false;
  }
  if (m_activeMouse){
    tud_hid_mouse_report(REPORT_ID_MOUSE, m_buttons, m_x, m_y, 0, 0);
    m_activeMouse = false;
  }
}
void HIDManager::setKeyboardKey(uint8_t keycode)
{
  m_activeKeyboard = true;
  m_keycodeArray[0] = keycode;
  update();
}
void HIDManager::setMouseState(uint8_t buttons, int8_t x, int8_t y)
{
  m_activeMouse = true;
  m_buttons = buttons;
  m_x = x;
  m_y = y;
  update();
}

bool HIDManager::timeIntervalElapsed()
{
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if (board_millis() - start_ms < interval_ms)
    return false; // not enough time
  start_ms += interval_ms;
  return true;
}

void HIDManager::macOSInit()
{
  setKeyboardKey(KEYBOARD_MODIFIER_LEFTGUI);
  setKeyboardKey(HID_KEY_SHIFT_LEFT);
  setKeyboardKey(HID_KEY_Q);
}

//================================================================================
// some callbacks from TinyUSB @todo: remove relted config
//================================================================================
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len)
{
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
  return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
}