#include <Arduino_FreeRTOS.h>
#include "queue.h"
#define LED1 5
#define LED2 6
#define LED3 7
struct InQ
{
    uint8_t pin;
    QueueHandle_t queue;
    bool flag;
} dataPack[] = {{2, nullptr, 0}, {3, nullptr, 0}, {4, nullptr, 0}};
void setup()
{
    Serial.begin(9600);
    dataPack[0].queue = xQueueCreate(32, sizeof(bool));
    dataPack[1].queue = xQueueCreate(1, sizeof(bool));
    dataPack[2].queue = xQueueCreate(1, sizeof(bool));
    xTaskCreate(buttonPress, "buttonPressBA", 128, dataPack + 0, 1, nullptr);
    xTaskCreate(buttonPress, "buttonPressBB", 128, dataPack + 1, 1, nullptr);
    xTaskCreate(buttonPress, "buttonPressBC", 128, dataPack + 2, 1, nullptr);
    xTaskCreate(led1, "LEDst", 64, nullptr, 1, nullptr);
    xTaskCreate(led2, "LEDnd", 64, nullptr, 1, nullptr);
    xTaskCreate(led3, "LEDrd", 64, nullptr, 1, nullptr);
}
// 1 Tick 15 ms
void buttonPress(void *val)
{
    InQ &data = *(InQ *)val;
    pinMode(data.pin, INPUT_PULLUP);
    while (true)
    {
        bool isPressed = !digitalRead(data.pin);
        if (isPressed && isPressed != data.flag)
        {
            Serial.println(xQueueSend(data.queue, &isPressed, 0));
        }
        data.flag = isPressed;
        vTaskDelay(10);
    }
}
void led1(void *val)
{
    pinMode(LED1, OUTPUT);
    while (true)
    {
        if (xQueueReceive(dataPack[0].queue, nullptr, 10))
        {
            digitalWrite(LED1, HIGH);
            vTaskDelay(pdMS_TO_TICKS(3000));
        }
        else
        {
            digitalWrite(LED1, LOW);
        }
    }
}
void led2(void *val)
{
    pinMode(LED2, OUTPUT);
    bool blinking = false;
    while (true)
    {
        if (xQueueReceive(dataPack[1].queue, nullptr, 1))
            blinking = !blinking;
        if (blinking)
        {
            digitalWrite(LED2, !digitalRead(LED2));
            vTaskDelay(pdMS_TO_TICKS(500) - 1);
        }
        else
        {
            digitalWrite(LED2, LOW);
        }
    }
}
void led3(void *val)
{
    pinMode(LED3, OUTPUT);
    while (true)
    {
        if (xQueueReceive(dataPack[2].queue, nullptr, 10))
        {
            xQueueSend(dataPack[2].queue, nullptr, 0);
            for (size_t i = 0; i < 6; i++)
            {
                digitalWrite(LED3, !digitalRead(LED3));
                vTaskDelay(pdMS_TO_TICKS(500));
            }
            xQueueReceive(dataPack[2].queue, nullptr, 0);
        }
    }
}
void loop()
{
}
