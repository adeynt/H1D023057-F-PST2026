#include <Arduino_FreeRTOS.h>

// Deklarasi fungsi Task
void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);
void Taskprint(void *pvParameters);

void setup() {
  // Inisialisasi komunikasi serial pada 9600 bps
  Serial.begin(9600);

  // Pembuatan Task 1
  xTaskCreate(
    TaskBlink1,   // Fungsi task
    "task1",      // Nama task
    128,          // Ukuran stack
    NULL,         // Parameter task
    1,            // Prioritas
    NULL          // Handle task
  );

  // Pembuatan Task 2 (Kekurangan penutup kurung dan NULL sudah diperbaiki)
  xTaskCreate(
    TaskBlink2,
    "task2",
    128, 
    NULL,
    1, 
    NULL 
  );

  // Pembuatan Task 3
  xTaskCreate(
    Taskprint,
    "task3",
    128, 
    NULL,
    1, 
    NULL 
  );

  // Memulai penjadwalan (Scheduler) FreeRTOS
  vTaskStartScheduler();
}

void loop() {
  // Kosong. Kendali program sudah diambil alih oleh Scheduler FreeRTOS
}

// --- Implementasi Task ---

void TaskBlink1(void *pvParameters) {
  pinMode(8, OUTPUT);
  while(1) {
    Serial.println("Task1");
    digitalWrite(8, HIGH); 
    vTaskDelay(200 / portTICK_PERIOD_MS); 
    digitalWrite(8, LOW); 
    vTaskDelay(200 / portTICK_PERIOD_MS); 
  }
}

void TaskBlink2(void *pvParameters) {
  pinMode(9, OUTPUT);
  while(1) {
    Serial.println("Task2");
    digitalWrite(9, HIGH); 
    vTaskDelay(300 / portTICK_PERIOD_MS); 
    digitalWrite(9, LOW); 
    vTaskDelay(300 / portTICK_PERIOD_MS); 
  }
}

void Taskprint(void *pvParameters) {
  int counter = 0;
  while(1) {
    counter++;
    Serial.println(counter); 
    vTaskDelay(500 / portTICK_PERIOD_MS); 
  }
}