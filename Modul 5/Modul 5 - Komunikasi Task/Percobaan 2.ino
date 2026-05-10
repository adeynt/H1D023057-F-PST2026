#include <Arduino_FreeRTOS.h>
#include <queue.h>

// Mendefinisikan struktur data untuk menyimpan nilai sensor
struct readings {
  int temp;
  int h;
};

// Mendeklarasikan handle untuk antrean (Queue)
QueueHandle_t my_queue;

// Deklarasi fungsi Task
void read_data(void *pvParameters);
void display(void *pvParameters);

void setup() {
  // Inisialisasi komunikasi serial pada 9600 bps
  Serial.begin(9600);

  // Membuat Queue: menampung 1 item data dengan ukuran sebesar 'struct readings'
  my_queue = xQueueCreate(1, sizeof(struct readings));

  // Membuat Task untuk membaca dan menampilkan data
  xTaskCreate(read_data, "read sensors", 128, NULL, 0, NULL);
  xTaskCreate(display, "display", 128, NULL, 0, NULL);
}

void loop() {
  // Kosong. Kendali program sudah diambil alih oleh Scheduler FreeRTOS
}

// --- Implementasi Task ---

/*
 * Task untuk membaca data sensor (simulasi)
 */
void read_data(void *pvParameters) {
  struct readings x;
  
  for(;;) {
    // Memasukkan nilai simulasi ke dalam struktur
    x.temp = 54;
    x.h = 30;
    
    // Mengirim struktur data ke antrean
    xQueueSend(my_queue, &x, portMAX_DELAY);
    
    // Penunda waktu (delay) selama 100 ticks
    vTaskDelay(100);
  }
}

/*
 * Task untuk menampilkan data dari antrean ke Serial Monitor
 */
void display(void *pvParameters) {
  struct readings x;
  
  for(;;) {
    // Menerima data dari antrean. Jika berhasil (pdPASS), maka cetak ke Serial
    if(xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      Serial.print("temp = ");
      Serial.println(x.temp);
      Serial.print("humidity = ");
      Serial.println(x.h);
    }
  }
}