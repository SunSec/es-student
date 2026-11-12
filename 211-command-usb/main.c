// добавляем заголовочный файл функций ввода-вывода
#include <stdio.h>
#include "pico/stdlib.h"
#include <string.h>

// добавляем заголовочный файл функций работы с GPIO
#include "hardware/gpio.h"

#include "led.h"
#include "log.h"
#include "device.h"


const uint BUTTON_PIN = 15;
const uint DEBOUNCE_MS = 20;

#define LINE_SIZE 32
char line[LINE_SIZE];
uint line_length = 0;

bool get_button_debounce(uint pin)
{
    bool state = gpio_get(pin);
    sleep_ms(DEBOUNCE_MS);
    return state && gpio_get(pin);
}

void handle_command(const char *command)
{
    if (strcmp(command, "enable") == 0)
    {   // включаем светодиод и сообщаем новое состояние
        led_set(true);
        LOG_INF("led %s\n", led_is_on() ? "on" : "off");
    }
    else if (strcmp(command, "disable") == 0)
    {   // выключаем светодиод и сообщаем новое состояние
        led_set(false);
        LOG_INF("led %s\n", led_is_on() ? "on" : "off");
    }
    else if (strcmp(command, "version") == 0)
    {   // печатаем строку журнала о версии прошивки
        log_version();
    }
    else if (strcmp(command, "info") == 0)
    {   // печатаем паспорт устройства
        device_info();
    }
    else
    {
        LOG_ERR("unknown command: %s\n", command);
    }
}

void read_line(void)
{
    int symbol = getchar_timeout_us(0);

    if (symbol == PICO_ERROR_TIMEOUT)
    {
        return;
    }

    if (symbol == '\r' || symbol == '\n')
    {
        putchar('\n');
        line[line_length] = '\0';

        if (line_length > 0)
        {
            LOG_DBG("got %s\n", line);
            handle_command(line);
        }

        line_length = 0;
        return;
    }

    if (line_length + 1 < LINE_SIZE)
    {
        line[line_length] = (char)symbol;
        line_length = line_length + 1;
        putchar(symbol);
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

        read_line();

        //LOG_DBG("got %c\n", command);
        // handle_command(command);
    }
}