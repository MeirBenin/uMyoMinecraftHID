#include "controller.h"

Controller::Controller()
{
    stdio_init_all();
}

void Controller::update()
{
    hid.update();
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
        rightHeand();
        leftHeand();
    }
}

void Controller::rightHeand()
{
    int butuon = 0;
    if (umyo.getMuscleLevel(R) > 1000)
        butuon = MOUSE_BUTTON_LEFT;
    hid.setMouseState(butuon, -getDelta(readX(R)), -getDelta(readY(R)));
}

void Controller::leftHeand()
{
    int deltax = getDelta(readX(L));
    int deltay = getDelta(readY(L));
    if (deltax < 0)
        hid.setKeyboardKey(HID_KEY_D);
    if (deltax > 0)
        hid.setKeyboardKey(HID_KEY_A);
    if (deltay < 0)
        hid.setKeyboardKey(HID_KEY_W);
    if (deltay > 0)
        hid.setKeyboardKey(HID_KEY_S);
    if (umyo.getMuscleLevel(L) > 1000)
        hid.setMouseState(MOUSE_BUTTON_RIGHT, 0, 0);
}

int Controller::getDelta(float rad)
{

    if ((rad >= _70DEG && rad <= _110DEG) ||
        (rad >= _250DEG && rad <= _290DEG))
        return 0;

    if ((rad >= _60DEG && rad <= _70DEG) ||
        (rad >= _290DEG && rad <= _300DEG))
        return 5;

    if ((rad >= _110DEG && rad <= _120DEG) ||
        (rad >= _240DEG && rad <= _250DEG))
        return -5;

    if ((rad >= _0DEG && rad <= _60DEG) ||
        (rad >= _300DEG && rad <= _360DEG))
        return 10;

    if ((rad >= _120DEG && rad <= _180DEG) ||
        (rad >= _180DEG && rad <= _240DEG))
        return -10;

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