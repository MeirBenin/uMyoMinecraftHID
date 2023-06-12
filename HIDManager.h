#pragma once
#include "pico/stdlib.h"
#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

class HIDManager
{
private:
    bool timeIntervalElapsed();
    void macOSInit();

    bool m_lastPressed = false;
    bool m_activeKeyboard = false;
    bool m_activeMouse = false;
    uint8_t m_keycodeArray[6] = {0};
    uint8_t m_buttons = 0;
    int8_t m_x = 0;
    int8_t m_y = 0;

public:
    HIDManager(void);
    void update();
    void setKeyboardKey(uint8_t keycode);
    void setMouseState(uint8_t buttons, int8_t x, int8_t y);
};
