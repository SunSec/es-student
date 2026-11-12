// добавляем заголовочный файл функций ввода-вывода
#include <stdio.h>
#include "pico/stdlib.h"

// добавляем заголовочный файл функций работы с GPIO
#include "hardware/gpio.h"

#include "led.h"
#include "log.h"

const uint BUTTON_PIN = 15;

const uint DEBOUNCE_MS = 20;

bool get_button_debounce(uint pin)
{
    bool state = gpio_get(pin);
    sleep_ms(DEBOUNCE_MS);
    return state && gpio_get(pin);
}

void handle_command(int command)
{
    if (command == 'e')
    {
        led_set(true);
        LOG_INF("led %s\n", led_is_on() ? "on" : "off");
    }
    else if (command == 'd')
    {
        led_set(false);
        LOG_INF("led %s\n", led_is_on() ? "on" : "off");
    }
    else if (command == 'v')
    {
        log_version();
    }
    else
    {
        LOG_ERR("unknown command: %c\n", command);
    }
}

int main()
{   
    // включаем стандартный ввод-вывод
    stdio_init_all();

    // инициализируем пин светодиода    
    led_init();

    // инициализируем пин кнопки    
    gpio_init(BUTTON_PIN);
    // настраиваем пин кнопки на вход    
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    // подтягиваем кнопку к питанию
    gpio_pull_up(BUTTON_PIN);

    // объявляем переменные
    bool led = false;
    bool previous = false;

    while (1)
    {
        // читаем состояние пина кнопки с задержкой
        bool current = get_button_debounce(BUTTON_PIN);

        // проверяем, нужно ли переключить светодиод
        if (previous == true && current == false)
        {
            led_toggle();
            LOG_INF("led %s\n", led_is_on() ? "on" : "off");
        }

        // запоминаем текущее состояние пина кнопки, как предыдущее
        previous = current;

        int command = getchar_timeout_us(0);

        if (command == PICO_ERROR_TIMEOUT)
        {
            continue;
        }

        LOG_DBG("got %c\n", command);
        handle_command(command);
    }
}