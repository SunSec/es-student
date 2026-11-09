// добавляем заголовочный файл функций ввода-вывода
// добавляем заголовочный файл функций работы с GPIO
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// объявляем константу вывода светодиода
const uint LED_PIN = 25;

const uint BUTTON_PIN = 15;

const uint DEBOUNCE_MS = 20;

bool get_button_debounce(uint pin)
{
    bool state = gpio_get(pin);
    sleep_ms(DEBOUNCE_MS);
    return state && gpio_get(pin);
}

int main()
{
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
            gpio_put(LED_PIN, led);
        }

        // запоминаем текущее состояние пина кнопки, как предыдущее
        previous = current;
    }
}