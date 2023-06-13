#include "controller.h"

Controller::Controller()
{
    stdio_init_all();
}

void Controller::run()
{
    while (true)
    {
        umyo.update();
        umyo.update();
        if (!connect())
            continue;
        if (!isMenu())
        {
            rightHeand();
            leftHeand();
        }
        sendHID();
    }
}

void Controller::rightHeand()
{
    if (umyo.getMuscleLevel(R) > 1000)
        mouseButtons = MOUSE_BUTTON_LEFT;
    mouseX = -getDelta(readX(R));
    mouseY = -getDelta(readY(R));
    int z = getDelta(readZ(R));
    if (z > 0)
        incNum();
    if (z < 0)
        decNum();
}

void Controller::leftHeand()
{
    int x = -getDelta(readX(L));
    int y = getDelta(readY(L));
    int z = getDelta(readZ(L));

    if (x > 0)
        pushKey(HID_KEY_D);
    if (x < 0)
        pushKey(HID_KEY_A);
    if (y > 0)
        pushKey(HID_KEY_W);
    if (y < 0)
        pushKey(HID_KEY_S);
    if (z > 0)
        pushKey(HID_KEY_SPACE);
    if (z < 0)
        pushKey(HID_KEY_SHIFT_LEFT);
    if (umyo.getMuscleLevel(L) > 1000)
        mouseButtons = MOUSE_BUTTON_RIGHT;
}

int Controller::getDelta(float rad)
{

    if ((rad >= _50DEG && rad <= _130DEG) ||
        (rad >= _230DEG && rad <= _300DEG))
        return 0;

    if ((rad >= _0DEG && rad <= _50DEG) ||
        (rad >= _300DEG && rad <= _360DEG))
        return 10;

    if ((rad >= _130DEG && rad <= _180DEG) ||
        (rad >= _180DEG && rad <= _230DEG))
        return -10;

    // if ((rad >= _0DEG && rad <= _60DEG) ||
    //     (rad >= _300DEG && rad <= _360DEG))
    //     return 10;

    // if ((rad >= _120DEG && rad <= _180DEG) ||
    //     (rad >= _180DEG && rad <= _240DEG))
    //     return -10;

    return 0;
}

float Controller::readX(int hand)
{
    float offset = 0;
    if (hand == R)
        offset = rxoffset;
    if (hand == L)
        offset = lxoffset;
    float x = umyo.getYaw(hand) + offset;
    if (x > _360DEG)
        x -= _360DEG;
    if (x < 0)
        x += _360DEG;
    return x;
}
float Controller::readZ(int hand)
{
    float offset = 0;
    if (hand == R)
        offset = rzoffset;
    if (hand == L)
        offset = lzoffset;
    float z = umyo.getRoll(hand) + offset;
    if (z > _360DEG)
        z -= _360DEG;
    if (z < 0)
        z += _360DEG;
    return z;
}
float Controller::readY(int hand)
{
    float offset = 0;
    if (hand == R)
        offset = ryoffset;
    if (hand == L)
        offset = lyoffset;
    float y = umyo.getPitch(hand) + offset;
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
    // static bool isMenu = false;
    // if (specialAction())
    // {
    //     led.on();
    //     isMenu = !isMenu;
    //     pushKey(HID_KEY_E);
    //     return true;
    // }
    // if (!isMenu)
    //     led.off();
    // return isMenu;
    return false;
}

bool Controller::connect()
{
    hid.update();
    if (umyo.getDeviceCount() >= 2)
        return true;
    led.toggle();
    sleep_ms(100);
    return false;
}

bool Controller::initIMU()
{
    umyo.update();
    if (!connect())
        return false;
    led.on();
    if (specialAction())
    {
        rxoffset = _90DEG - umyo.getYaw(R);
        ryoffset = _90DEG - umyo.getPitch(R);
        rzoffset = _90DEG - umyo.getRoll(R);
        lxoffset = _90DEG - umyo.getYaw(L);
        lyoffset = _90DEG - umyo.getPitch(L);
        lzoffset = _90DEG - umyo.getRoll(L);
        led.off();
        return true;
    }
    hid.update();
    return false;
}

void Controller::pushKey(uint8_t keycode)
{
    for (int i = 0; i < 6; i++)
    {
        if (keys[i] == HID_KEY_NONE)
        {
            keys[i] = keycode;
            return;
        }
    }
}

void Controller::incNum()
{
    num++;
    if (num > 9)
        num = 1;
    setNum();
}

void Controller::decNum()
{
    num--;
    if (num < 1)
        num = 9;
    setNum();
}

void Controller::setNum()
{
    switch (num)
    {
    case 1:
        pushKey(HID_KEY_1);
        break;
    case 2:
        pushKey(HID_KEY_2);
        break;
    case 3:
        pushKey(HID_KEY_3);
        break;
    case 4:
        pushKey(HID_KEY_4);
        break;
    case 5:
        pushKey(HID_KEY_5);
        break;
    case 6:
        pushKey(HID_KEY_6);
        break;
    case 7:
        pushKey(HID_KEY_7);
        break;
    case 8:
        pushKey(HID_KEY_8);
        break;
    case 9:
        pushKey(HID_KEY_9);
        break;
    default:
        break;
    }
}

void Controller::sendHID()
{
    while (!hid.setKeyboardState(keys))
        ;
    while (!hid.setMouseState(mouseButtons, mouseX, mouseY))
        ;
    mouseButtons = 0x00;
     for (int i = 0; i < 6; i++)
    {
        keys[i] == HID_KEY_NONE;
    }
}
