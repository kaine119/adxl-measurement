#include <Arduino.h>
#include <ADXL345.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <queue>

#define BUFFER_SIZE 200

// BluetoothSerial SerialBT;
ADXL345 adxl;
typedef struct Reading {
  unsigned long timestamp;
  float x, y, z;
} Reading;

static int measured_i = 0;

Reading readings[BUFFER_SIZE];

void setup() {
  Serial.setTxBufferSize(200 * 1024);
  Serial.begin(115200);

  adxl.init();
}

void task_txReadings(void* params) {
  for (int i = 0; i < BUFFER_SIZE; i++) {
    Serial.printf("%d,%f,%f,%f\n", readings[i].timestamp, readings[i].x, readings[i].y, readings[i].z);
  }
}

void loop() {
  int* reading = adxl.read();
  readings[measured_i] = {
    .timestamp = micros(),
    .x = adxl.get_SI_x(reading),
    .y = adxl.get_SI_y(reading),
    .z = adxl.get_SI_z(reading)
  };
  measured_i++;
  if (measured_i == BUFFER_SIZE) {
    task_txReadings(nullptr);
    measured_i = 0;
  }
}
