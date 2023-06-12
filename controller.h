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
    float ryoffset = 0;
    float rxoffset = 0;
    float lyoffset = 0;
    float lxoffset = 0;

    int getDelta(float radian);
    void update();
    void rightHeand();
    void leftHeand();
    bool isMenu();
    bool connect();
    bool specialAction();
    float readX(int hand);
    float readY(int hand);

public:
    Controller();
    bool initIMU();
    void run();
};