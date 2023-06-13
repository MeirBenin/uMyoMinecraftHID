#pragma once
#include "pico/stdlib.h"
#include "umyo.h"
#include "HIDManager.h"
#include "led.h"
#include "degrees.h"


#define R 0
#define L 1

class Controller
{
private:
    uMyo umyo = uMyo();
    HIDManager hid = HIDManager();
    Led led = Led();
    float rxoffset = 0;
    float ryoffset = 0;
    float rzoffset = 0;
    float lxoffset = 0;
    float lyoffset = 0;
    float lzoffset = 0;
    uint8_t keys[6] = {0, 0, 0, 0, 0, 0};
    uint8_t mouseButtons = 0;
    int8_t mouseX = 0;
    int8_t mouseY = 0;
    int8_t num = 1;
    int getDelta(float radian);
    void rightHeand();
    void leftHeand();
    bool isMenu();
    bool connect();
    bool specialAction();
    float readX(int hand);
    float readY(int hand);
    float readZ(int hand);
    void pushKey(uint8_t keycode);
    void incNum();
    void decNum();
    void setNum();
    void sendHID();

public:
    Controller();
    bool initIMU();
    void run();
};