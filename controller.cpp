#include "controller.h"

Controller::Controller()
{
    stdio_init_all();
}

void Controller::update()
{
    umyo.update();
}

void Controller::run()
{
    while (true)
    {
        update();
        if (!connect())
            continue;
        // if (isMenu())
        //     continue;
        led.off();
        // rightHeand();
        // leftHeand();
        printf("10 -10 %d %d %d %d\n", -getDelta(readX(R)), -getDelta(readY(R)), getDelta(readX(L)), getDelta(readY(L)));
    }
}

void Controller::rightHeand()
{
    // int butuon = 0;
    // if (umyo.getMuscleLevel(R) > 1000)
    //     butuon = MOUSE_BUTTON_LEFT;
    // hid.setMouseState(butuon, -getDelta(readX(R)), -getDelta(readY(R)));
}

void Controller::leftHeand()
{
//     int deltax = getDelta(readX(L));
//     int deltay = getDelta(readY(L));
//     if (deltax < 0)
//         hid.setKeyboardKey(HID_KEY_D);
//     if (deltax > 0)
//         hid.setKeyboardKey(HID_KEY_A);
//     if (deltay < 0)
//         hid.setKeyboardKey(HID_KEY_W);
//     if (deltay > 0)
//         hid.setKeyboardKey(HID_KEY_S);
//     if (umyo.getMuscleLevel(L) > 1000)
//         hid.setMouseState(MOUSE_BUTTON_RIGHT, 0, 0);
}

int Controller::getDelta(float radian)
{
    if (radian > _0DEG && radian <= _30DEG)
        return 10;
    if (radian > _30DEG && radian <= _60DEG)
        return 5;
    if (radian > _60DEG && radian <= _90DEG)
        return 0;
    if (radian > _90DEG && radian <= _120DEG)
        return 0;
    if (radian > _120DEG && radian <= _150DEG)
        return -5;
    if (radian > _150DEG && radian <= _180DEG)
        return -10;
    if (radian > _180DEG && radian <= _210DEG)
        return -10;
    if (radian > _210DEG && radian <= _240DEG)
        return -10;
    if (radian > _240DEG && radian <= _270DEG)
        return 0;
    if (radian > _270DEG && radian <= _300DEG)
        return 0;
    if (radian > _300DEG && radian <= _330DEG)
        return 10;
    if (radian > _330DEG && radian <= _360DEG)
        return 10;

    return 0;
}

float Controller::readX(int hand)
{
    float x = umyo.getYaw(hand) + rxoffset;
    if (x > _360DEG)
        x -= _360DEG;
    if (x < 0)
        x += _360DEG;
    return x;
}

float Controller::readY(int hand)
{
    float y = umyo.getPitch(hand) + ryoffset;
    if (y > _180DEG)
        y -= _180DEG;
    if (y < 0)
        y += _180DEG;
    return y;
}

bool Controller::specialAction()
{
    return umyo.getMuscleLevel(R) > 1000 && umyo.getMuscleLevel(L) > 1000;
}

bool Controller::isMenu()
{
    return false;
}

bool Controller::connect()
{
    if (umyo.getDeviceCount() >= 2)
        return true;
    led.toggle();
    sleep_ms(100);
    return false;
}

bool Controller::initIMU()
{
    led.on();
    update();
    if (!connect())
        return false;
    if (specialAction())
    {
        ryoffset = _90DEG - umyo.getPitch(R);
        rxoffset = _90DEG - umyo.getYaw(R);
        lxoffset = _90DEG - umyo.getPitch(L);
        lyoffset = _90DEG - umyo.getYaw(L);
        return true;
    }
    return false;
}