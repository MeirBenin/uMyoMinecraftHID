#pragma once
#include <RF24.h> // RF24 radio object

#define CE 7
#define CSN 8
#define SCK 2
#define TX 3
#define RX 4


class Radio
{
private:
    RF24 *m_radio = nullptr;
public:
    Radio(void);
    void readPack(uint8_t buffer[32]);
};
