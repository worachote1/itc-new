include <Arduino_FreeRTOS.h>
#include "queue.h"
#include "semphr.h"
#define RED 8
#define GREEN 10
#define YELLOW 9

#define SW1 5
#define SW2 6
#define SW3 7

SemaphoreHandle_t redMutex, greenMutex;
QueueHandle_t redQueue, yellowQueue, greenQueue;

void setup()
{
    Serial.begin(9600);
    redQueue = xQueueCreate(1, sizeof(bool));
    yellowQueue = xQueueCreate(1, sizeof(bool));
    greenQueue = xQueueCreate(1, sizeof(bool));
    redMutex = xSemaphoreCreateMutex();
    greenMutex = xSemaphoreCreateMutex();
    xSemaphoreGive(redMutex);
    xSemaphoreGive(greenMutex);
    xTaskCreate(redSwitch, "redSwitch", 64, NULL, 1, NULL);
    xTaskCreate(yellowSwitch, "redSwitch", 64, NULL, 1, NULL);
    xTaskCreate(greenSwitch, "redSwitch", 64, NULL, 1, NULL);
    xTaskCreate(red, "red", 64, NULL, 1, NULL);
    xTaskCreate(yellow, "yellow", 64, NULL, 1, NULL);
    xTaskCreate(green, "green", 64, NULL, 1, NULL);
}
void redSwitch(void *value)
{
    pinMode(SW1, INPUT);
    while (true)
    {
        if (digitalRead(SW1))
        {
            xQueueSend(redQueue, NULL, 0);
        }
        vTaskDelay(20);
    }
}
void yellowSwitch(void * value)
{
    pinMode(SW2, INPUT);
    while (true)
    {
        if (digitalRead(SW2) && xSemaphoreTake(redMutex, 0) && xSemaphoreTake(greenMutex, 0))
        {
            xSemaphoreGive(redMutex);
            xSemaphoreGive(greenMutex);
            xQueueSend(yellowQueue, NULL, 0);
        }
        vTaskDelay(20);
    }
}
void greenSwitch(void * value)
{
    pinMode(SW3, INPUT);
    while (true)
    {
        if (digitalRead(SW3) && xSemaphoreTake(redMutex, 0))
        {
            xSemaphoreGive(redMutex);
            xQueueSend(greenQueue, NULL, 0);
        }
        vTaskDelay(20);
    }
}
void red(void * value)
{
    pinMode(RED, OUTPUT);
    while (true)
    {
        if(xQueueReceive(redQueue, NULL, pdMS_TO_TICKS(3000)))
        {
            digitalWrite(RED, !digitalRead(RED));
        }
        else
        {
            digitalWrite(RED, LOW);
        }
        if(digitalRead(RED))
        {
            xSemaphoreTake(redMutex, 0);
        }
        else
        {
            xSemaphoreGive(redMutex);
        }
        vTaskDelay(10);
    }
}
void yellow(void * value)
{
    pinMode(YELLOW, OUTPUT);
    while (true)
    {
        if (xQueueReceive(yellowQueue, NULL, 0))
        {
            digitalWrite(YELLOW, HIGH);
            vTaskDelay(pdMS_TO_TICKS(500));
            digitalWrite(YELLOW, LOW);
            vTaskDelay(pdMS_TO_TICKS(500));
            digitalWrite(YELLOW, HIGH);
            vTaskDelay(pdMS_TO_TICKS(500));
            digitalWrite(YELLOW, LOW);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
void green(void * value)
{
    pinMode(GREEN, OUTPUT);
    while (true)
    {
        if(xQueueReceive(greenQueue, NULL, pdMS_TO_TICKS(3000)))
        {
            digitalWrite(GREEN, !digitalRead(GREEN));
        }
        else
        {
            digitalWrite(GREEN, LOW);
        }
        if(digitalRead(GREEN))
        {
            xSemaphoreTake(greenMutex, 0);
        }
        else
        {
            xSemaphoreGive(greenMutex);
        }
        vTaskDelay(10);
    }
}
void loop() {}
