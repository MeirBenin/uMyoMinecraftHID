#pragma once
#include "pico/stdlib.h"



class Led{
private:
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
public:
    Led();
    void on();
    void off();
    void toggle();
};