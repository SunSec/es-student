// защита от повторного включения
#pragma once

// имя устройства, версия прошивки
#define DEVICE_NAME "es-led-module"
#define FIRMWARE_VERSION "1.0.0"

// название проекта и ссылка на ваш репозиторий курса
#define DEVICE_PROJECT "134-led-module"
#define DEVICE_REPO "https://github.com/<ваш логин>/es-student"

// запасное значение DEVICE_BOARD на случай сборки без определения компилятора
#ifndef DEVICE_BOARD
#define DEVICE_BOARD "unknown"
#endif

void device_info(void);

    