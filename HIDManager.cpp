
#include "HIDManager.h"


HIDManager::HIDManager(void)
{
  board_init();
  tusb_init();
  tud_task(); // tinyusb device task
}

void HIDManager::update(void)
{
  tud_task(); // tinyusb device task
}

bool HIDManager::isReady(void)
{
  tud_task(); // tinyusb device task

  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if (board_millis() - start_ms < interval_ms)
    return false; // not enough time
  start_ms += interval_ms;

  // Remote wakeup
  if (tud_suspended())
  {
    // Wake up host if we are in suspend mode
    // and REMOTE_WAKEUP feature is enabled by host
    tud_remote_wakeup();
    return false;
  }
  return tud_hid_ready();
}

bool HIDManager::setKeyboardState(uint8_t keycode[6])
{
  if (!isReady())
    return false;
    static bool has_keyboard_key = false;

    if (keycode[0] != HID_KEY_NONE)
    {
      tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
      has_keyboard_key = true;
    }
    else
    {
      // send empty key report if previously has key pressed
      if (has_keyboard_key)
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
      has_keyboard_key = false;
    }
   
  return true;
}

bool HIDManager::setMouseState(uint8_t buttons, int8_t x, int8_t y)
{
  if (!isReady())
    return false;
  tud_hid_mouse_report(REPORT_ID_MOUSE, buttons, x, y, 0, 0);
  return true;
}




//================================================================================
// tinyusb callbacks
//================================================================================

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len)
{
  // (void)instance;
  // (void)len;

  // uint8_t next_report_id = report[0] + 1;

  // if (next_report_id < REPORT_ID_COUNT)
  // {
  //   send_hid_report(next_report_id);
  // }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void)instance;
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
}
