# Praktikum Sistem Tertanam - Modul 5 Multitasking

## Pertanyaan Praktikum
1. Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!
2. Bagaimana cara menambahkan task keempat? Jelaskan langkahnya!
3. Modifikasilah program dengan menambah sensor (misalnya potensiometer), lalu gunakan nilainya untuk mengontrol kecepatan LED! Bagaimana hasilnya? Jelaskan program pada file README.md.

## Jawaban

### 1. Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!
Ketiga task tersebut berjalan secara bergantian, bukan bersamaan (paralel). Arduino Uno menggunakan mikrokontroler dengan single-core (satu inti prosesor), sehingga ia hanya bisa mengeksekusi satu instruksi pada satu waktu. Mekanisme ini disebut time-slicing atau context-switching. FreeRTOS menggunakan sebuah Scheduler (penjadwal) yang dengan sangat cepat memindahkan kontrol prosesor dari satu task ke task lainnya. Ketika sebuah task memanggil fungsi `vTaskDelay()`, task tersebut akan masuk ke mode Blocked (menunggu), dan Scheduler akan langsung mengalihkan prosesor untuk menjalankan task lain yang berstatus Ready. Karena perpindahan ini terjadi dalam hitungan milidetik, di mata kita program seolah-olah berjalan bersamaan (multitasking).

### 2. Bagaimana cara menambahkan task keempat? Jelaskan langkahnya!
Untuk menambahkan task keempat, langkah-langkahnya adalah sebagai berikut:

#### a. Deklarasi Fungsi: Buat purwarupa (prototype) fungsi di awal program, contoh: 
```
void Task4(void *pvParameters);
```

#### b. Buat Task di `setup()`: Tambahkan pemanggilan fungsi `xTaskCreate()` untuk task keempat di dalam `void setup()`, contoh:
```
xTaskCreate(Task4, "task4", 128, NULL, 1, NULL);
```

#### c. Definisikan Fungsi Task: Buat isi dari fungsi task keempat di bagian bawah program, pastikan menggunakan perulangan tanpa henti (`while(1)` atau `for(;;)`) dan memiliki vTaskDelay agar tidak memonopoli CPU:
```
void Task4(void *pvParameters) {
  while(1) {
    // Logika task 4 disini
    vTaskDelay( 500 / portTICK_PERIOD_MS );
  }
}
```

### 3. Modifikasilah program dengan menambah sensor (misalnya potensiometer), lalu gunakan nilainya untuk mengontrol kecepatan LED!
#### Kode
```
#include <Arduino_FreeRTOS.h>

void TaskBlink1( void *pvParameters );
void TaskBlink2( void *pvParameters );
void Taskprint( void *pvParameters );

void setup() {
  Serial.begin(9600);
  xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, NULL );
  xTaskCreate(TaskBlink2, "task2", 128, NULL, 1, NULL );
  xTaskCreate(Taskprint, "task3", 128, NULL, 1, NULL );
  vTaskStartScheduler();
}

void loop() {}

void TaskBlink1(void *pvParameters) {
  pinMode(8, OUTPUT);
  while(1) {
    int potValue = analogRead(A0); 
    int delayTime = map(potValue, 0, 1023, 50, 1000); 

    Serial.print("Task1 Delay: ");
    Serial.println(delayTime);

    digitalWrite(8, HIGH); 
    vTaskDelay( delayTime / portTICK_PERIOD_MS ); 
    digitalWrite(8, LOW); 
    vTaskDelay( delayTime / portTICK_PERIOD_MS ); 
  }
}

void TaskBlink2(void *pvParameters) {
  pinMode(7, OUTPUT);
  while(1) {
    Serial.println("Task2");
    digitalWrite(7, HIGH); 
    vTaskDelay( 300 / portTICK_PERIOD_MS ); 
    digitalWrite(7, LOW); 
    vTaskDelay( 300 / portTICK_PERIOD_MS ); 
  }
}

void Taskprint(void *pvParameters) {
  int counter = 0;
  while(1) {
    counter++;
    Serial.print("Counter: ");
    Serial.println(counter); 
    vTaskDelay(500 / portTICK_PERIOD_MS); 
  }
}
```

#### Penjelasan
1. Impor Pustaka dan Deklarasi Fungsi </br>
```
#include <Arduino_FreeRTOS.h>
```
Mengimpor library utama FreeRTOS agar board Arduino dapat menjalankan fungsi-fungsi Real-Time Operating System.
```
void TaskBlink1( void *pvParameters );
void TaskBlink2( void *pvParameters );
void Taskprint( void *pvParameters );
```
Ketiga baris ini adalah "Deklarasi Fungsi" (atau Prototype). Ini memberitahu kompiler Arduino (C++) sejak awal bahwa di bawah nanti akan ada fungsi bernama TaskBlink1, TaskBlink2, dan Taskprint. Parameter void *pvParameters adalah standar FreeRTOS yang memungkinkan kita mengirim parameter tertentu saat task dijalankan, meskipun di praktikum ini kita tidak mengirim parameter apa-apa (bernilai NULL).

2. Inisialisasi Sistem dan Pembuatan Task (Fungsi setup) </br>
```
void setup() {
```
Pada Arduino, semua perintah di dalam kurawal ini hanya dieksekusi satu kali saat mikrokontroler pertama kali dihidupkan atau di-reset.
```
Serial.begin(9600);
```
Memulai komunikasi Serial antara Arduino dan komputer dengan kecepatan baud rate 9600 bit per detik. Ini wajib agar kita bisa melihat output di Serial Monitor.
```
xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, NULL );
```
Membuat Task pertama. Penjelasan parameternya:

- TaskBlink1: Nama fungsi yang akan dijalankan oleh task ini.

- "task1": Nama deskriptif untuk task tersebut (berguna untuk debugging).

- 128: Ukuran alokasi memori (Stack) untuk task ini.

- NULL: Tidak ada parameter khusus yang dikirimkan ke fungsi task.

- 1: Prioritas task. Semakin tinggi angkanya, semakin tinggi prioritasnya.

- NULL: Tidak menggunakan Task Handle (penunjuk/referensi identitas task).
```
xTaskCreate(TaskBlink2, "task2", 128, NULL, 1, NULL );
  xTaskCreate(Taskprint, "task3", 128, NULL, 1, NULL );
```
Menciptakan task kedua dan ketiga dengan konfigurasi parameter dan prioritas yang sama dengan task pertama.
```
vTaskStartScheduler();
```
Baris ini menyalakan Scheduler (penjadwal OS). Mulai dari baris ini, OS FreeRTOS mengambil alih penuh kendali CPU mikrokontroler untuk mengatur kapan "task1", "task2", dan "task3" harus dijalankan secara bergantian.

3. Fungsi Perulangan Utama</br>
```
void loop() {}
```
Fungsi perulangan bawaan Arduino. Sengaja dibiarkan kosong ({}) karena fungsi perulangan digantikan oleh task-task mandiri yang diurus oleh sistem FreeRTOS.

4. Definisi Task 1 (Pembacaan Sensor dan Kontrol LED Dinamis)</br>
```
void TaskBlink1(void *pvParameters) {
```
Mendefinisikan isi (algoritma) dari Task 1.
```
pinMode(8, OUTPUT);
```
Mengatur pin digital 8 pada Arduino sebagai OUTPUT (tempat kita mencolokkan LED pertama).
```
while(1) {
```
Memulai infinite loop (perulangan yang tidak akan pernah berhenti). Semua task FreeRTOS wajib berada dalam perulangan ini agar task tidak otomatis terhapus saat selesai dieksekusi.
```
int potValue = analogRead(A0);
```
Membuat variabel bertipe integer (bilangan bulat) bernama potValue yang isinya didapat dari membaca tegangan sensor potensiometer di pin Analog A0 (nilainya berkisar dari 0 hingga 1023).
```
int delayTime = map(potValue, 0, 1023, 50, 1000);
```
Membuat variabel delayTime. Fungsi map mengonversi rentang nilai pembacaan sensor (0 sampai 1023) menjadi skala yang baru, yaitu antara 50 hingga 1000 (merepresentasikan waktu dalam milidetik).
```
Serial.print("Task1 Delay: ");
Serial.println(delayTime);
```
Mencetak tulisan "Task1 Delay: " ke Serial monitor, lalu dilanjutkan dengan mencetak angka di variabel delayTime dan pindah baris (println).
```
digitalWrite(8, HIGH);
```
Mengirimkan sinyal HIGH (tegangan 5V) ke pin 8, sehingga LED pertama menyala.
```
vTaskDelay( delayTime / portTICK_PERIOD_MS );
```
Fungsi penundaan khusus RTOS. vTaskDelay menidurkan (membawa ke mode Block) hanya Task 1 ini saja selama waktu dari variabel delayTime (hasil putaran potensiometer). Selama Task 1 tertidur, OS akan menjalankan Task 2 atau Task 3. Formula / portTICK_PERIOD_MS mengonversi milidetik menjadi satuan tick sistem waktu OS.
```
digitalWrite(8, LOW);
```
Mengirimkan sinyal LOW (tegangan 0V) ke pin 8, sehingga LED pertama mati.
```
vTaskDelay( delayTime / portTICK_PERIOD_MS );
```
Menidurkan Task 1 lagi selama selang waktu yang sama membiarkan LED dalam posisi mati sebelum siklus while(1) kembali ke awal.

5. Definisi Task 2 (Kontrol LED Statis)</br>
```
void TaskBlink2(void *pvParameters) {
  pinMode(7, OUTPUT);
  while(1) {
    Serial.println("Task2");
    digitalWrite(7, HIGH); 
    vTaskDelay( 300 / portTICK_PERIOD_MS ); 
    digitalWrite(7, LOW); 
    vTaskDelay( 300 / portTICK_PERIOD_MS ); 
  }
}
```
Logikanya persis sama seperti Task 1, namun ini untuk mengatur LED di pin digital 7. Task ini tidak membaca sensor, melainkan selalu menyala statis 300ms dan mati statis 300ms secara berulang serta mencetak "Task2" ke monitor.

6. Definisi Task 3 (Pencetak Counter)</br>
```
void Taskprint(void *pvParameters) {
```
Mendefinisikan isi dari Task 3 (Task pencetak angka counter).
```
int counter = 0;
```
Membuat variabel counter dimulai dari 0. Ini dilakukan di luar loop agar tidak ter-reset ke 0 terus menerus.
```
while(1) {
```
Perulangan tak terhingga untuk Task 3.
```
counter++;
```
Menambah nilai variabel counter sebanyak 1 poin (+1) setiap kali loop dijalankan.
```
Serial.print("Counter: ");
Serial.println(counter);
```
Mencetak tulisan "Counter: " dilanjutkan dengan mencetak angka terbaru di dalam variabel counter.
```
vTaskDelay(500 / portTICK_PERIOD_MS); 
```
Menidurkan Task 3 ini secara statis selama 500ms (setengah detik) sebelum mengulang ke atas. Baris terakhir adalah penutup loop dan penutup fungsi.