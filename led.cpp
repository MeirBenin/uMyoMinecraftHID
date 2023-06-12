#include "led.h"

Led::Led()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

void Led::on()
{
    gpio_put(LED_PIN, 1);
}

void Led::off()
{
    gpio_put(LED_PIN, 0);
}

void Led::toggle()
{
    gpio_put(LED_PIN, !gpio_get(LED_PIN));
}
