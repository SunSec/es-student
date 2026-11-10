// добавляем заголовочный файл функций ввода-вывода
#include <stdio.h>
#include "pico/stdlib.h"

// добавляем заголовочный файл функций работы с GPIO
#include "hardware/gpio.h"

// объявляем константу вывода светодиода
const uint LED_PIN = 25;

const uint BUTTON_PIN = 15;

const uint DEBOUNCE_MS = 20;

void set_led(bool on)
{
    gpio_put(LED_PIN, on);
    printf("led %s\n", on ? "on" : "off");
}

bool get_button_debounce(uint pin)
{
    bool state = gpio_get(pin);
    sleep_ms(DEBOUNCE_MS);
    return state && gpio_get(pin);
}

bool handle_command(int command, bool led)
{
    if (command == 'e')
    {
        led = true;
        set_led(led);
    }
    else if (command == 'd')
    {
        led = false;
        set_led(led);
    }
    else
    {
        printf("unknown command: %c\n", command);
    }

    return led;
}

int main()
{   
    // включаем стандартный ввод-вывод
    stdio_init_all();

    // инициализируем пин светодиода    
    gpio_init(LED_PIN);
    // настраиваем пин светодиода на выход
    gpio_set_dir(LED_PIN, GPIO_OUT);

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
            led = !led;
            set_led(led);
        }

        // запоминаем текущее состояние пина кнопки, как предыдущее
        previous = current;

        int command = getchar_timeout_us(0);

        if (command == PICO_ERROR_TIMEOUT)
        {
            continue;
        }

        led = handle_command(command, led);
    }
}