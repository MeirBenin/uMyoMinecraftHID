#include "radio.h"

Radio::Radio(void)
{
    // create a new radio object
    m_radio = new RF24(CE, CSN);
    // initialize the transceiver on the SPI bus
    spi.begin(spi0, SCK, TX, RX);

    // enable the radio hardware
    if (!m_radio->begin())
    {
        printf("Radio hardware is not responding\n");
        return ;
    }
    // some hardcoded settings for umyo radio
    m_radio->setAddressWidth(4);
    m_radio->setChannel(83);
    m_radio->setPayloadSize(32);
    m_radio->setRetries(0, 0);
    m_radio->setAutoAck(0);
    m_radio->disableDynamicPayloads();
    m_radio->disableCRC();
    m_radio->setCRCLength(RF24_CRC_DISABLED);

    // hardcoded address of the umyo
    uint8_t addres[8] = {0x70, 0x67, 0xB0, 0xE5, 0, 0, 0, 0};

    // open reading pipe
    m_radio->openReadingPipe(0, addres);

    // put radio in RX mode
    m_radio->startListening();
}

void Radio::readPack(uint8_t buffer[32])
{
    if (!m_radio)
    {
        printf("Radio not initialized\n");
        return;
    }
    if (m_radio->available())
    {
        m_radio->read(buffer, 32);
    }
}