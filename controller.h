#pragma once
#include "pico/stdlib.h"
#include "umyo.h"
#include "HIDManager.h"
#include "led.h"

#define _0DEG 0
#define _30DEG M_PI / 6
#define _45DEG M_PI / 4
#define _60DEG M_PI / 3
#define _90DEG M_PI / 2
#define _120DEG M_PI * 2 / 3
#define _135DEG M_PI * 3 / 4
#define _150DEG M_PI * 5 / 6
#define _180DEG M_PI
#define _210DEG M_PI * 7 / 6
#define _225DEG M_PI * 5 / 4
#define _240DEG M_PI * 4 / 3
#define _270DEG M_PI * 3 / 2
#define _300DEG M_PI * 5 / 3
#define _315DEG M_PI * 7 / 4
#define _330DEG M_PI * 11 / 6
#define _360DEG M_PI * 2

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