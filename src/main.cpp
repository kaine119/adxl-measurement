#include <Arduino.h>
#include <ADXL345.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <queue>
#include <memory>

#define BUFFER_SIZE 200

void task_txReadings(void *params);
void task_readAdxl(void *params);

ADXL345 adxl;
typedef struct Reading {
  unsigned long timestamp;
  float x, y, z;
} Reading;

int measured_i = 0;

QueueHandle_t queue_readings;

void setup() {
  Serial.setTxBufferSize(2000 * 1024);
  Serial.begin(250000);

  adxl.init();

  queue_readings = xQueueCreate(15000, sizeof(Reading));
  
  TaskHandle_t taskHandle_txReadings;
  xTaskCreatePinnedToCore(task_txReadings, "task_txReadings", 20 * 1024, nullptr, 1, &taskHandle_txReadings, 1);

  TaskHandle_t taskHandle_readAdxl;
  xTaskCreatePinnedToCore(task_readAdxl, "task_readAdxl", 20 * 1024, nullptr, 2, &taskHandle_readAdxl, 0);
}

void loop() {}

void task_txReadings(void* params) {
  Reading reading;
  while (1) {
    while (xQueueReceive(queue_readings, &reading, 0) != errQUEUE_EMPTY) {
      Serial.printf("%d,%f,%f,%f\n", reading.timestamp, reading.x, reading.y, reading.z);
      // vTaskDelay(2 / portTICK_PERIOD_MS);
    }
    // vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void task_readAdxl(void* params) {
  int* reading;
  Reading new_reading;

  TickType_t last_wake = xTaskGetTickCount();
  const TickType_t freq = 1;
  BaseType_t wasDelayed;

  while (1) {
    reading = adxl.read();
    new_reading = {
      .timestamp = micros(),
      .x = adxl.get_SI_x(reading),
      .y = adxl.get_SI_y(reading),
      .z = adxl.get_SI_z(reading)
    };
    xQueueSend(queue_readings, &new_reading, 0);
    // wasDelayed = xTaskDelayUntil(&last_wake, freq);
  }
}
