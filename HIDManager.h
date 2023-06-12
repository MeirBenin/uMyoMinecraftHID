#pragma once
#include "pico/stdlib.h"
#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

class HIDManager
{
private:
    bool isReady(void);
public:
    HIDManager(void);
    void update(void);
    bool setKeyboardKey(uint8_t keycode);
    bool setMouseState(uint8_t buttons, int8_t x, int8_t y);
};
