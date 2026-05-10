# Praktikum Sistem Tertanam - Modul 5 Komunikasi Task

## Pertanyaan Praktikum
1. Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!
2. Apakah program ini berpotensi mengalami race condition? Jelaskan!
3. Modifikasilah program dengan menggunakan sensor DHT sesungguhnya sehingga informasi yang ditampilkan dinamis. Bagaimana hasilnya? Jelaskan program pada file README.md.

## Jawaban

### 1. Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!
Kedua task berjalan secara bergantian yang dikendalikan oleh sinkronisasi antrean (Queue). Mekanismenya adalah task display menunggu data dengan status memblokir diri (portMAX_DELAY) hingga antrean (my_queue) terisi. Sementara itu, read_data bekerja mengirimkan struct data ke dalam antrean, lalu menahan diri (vTaskDelay(100)). Begitu read_data mengirim data, antrean terisi, sehingga memicu Scheduler untuk membangunkan task display. Task display mengambil data tersebut, mencetaknya ke Serial Monitor, dan kembali menunggu data selanjutnya.
### 2. Apakah program ini berpotensi mengalami race condition? Jelaskan!
Program ini sangat tidak berpotensi mengalami race condition pada pembacaan datanya. Mengapa? Karena pertukaran data (suhu dan kelembapan) dibungkus menggunakan fitur Queue bawaan FreeRTOS yang secara internal sudah thread-safe. FreeRTOS menangani mutual exclusion saat data dimasukkan (Send) dan dikeluarkan (Receive) dari memori queue. Namun, karena keduanya menggunakan Serial.begin sebagai jalur output bersama, jika kedua task sama-sama memanggil Serial.print di waktu yang bersamaan, output monitor bisa tumpang tindih. Namun dalam kode ini, hanya task display yang mengakses Serial, sehingga aman.

### 3. Modifikasilah program dengan menggunakan sensor DHT sesungguhnya sehingga informasi yang ditampilkan dinamis.
#### KOde
```
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h> 

#define DHTPIN 2       
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE); 

struct readings{
  int temp;
  int h;
};

QueueHandle_t my_queue;

void setup() {
  Serial.begin(9600);
  dht.begin(); 
  
  my_queue = xQueueCreate(1, sizeof(struct readings));
  
  xTaskCreate(read_data, "read sensors", 128, NULL, 0, NULL);
  xTaskCreate(display, "display", 128, NULL, 0, NULL);
}

void loop() {}

void read_data(void *pvParameters){
  struct readings x;
  for(;;){
    x.h = dht.readHumidity();       
    x.temp = dht.readTemperature(); 

    xQueueSend(my_queue, &x, portMAX_DELAY);
    
    // DHT memerlukan jeda sekitar 2 detik antar pembacaan
    vTaskDelay(2000 / portTICK_PERIOD_MS); 
  }
}

void display(void *pvParameters){
  struct readings x;
  for(;;){
    if(xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS){
      Serial.print("temp = ");
      Serial.print(x.temp);
      Serial.println(" C");
      Serial.print("humidity = ");
      Serial.print(x.h);
      Serial.println(" %");
    }
  }
}
```

#### Penjelasan
1. Impor Pustaka dan Konfigurasi Sensor DHT </br>
```
#include <Arduino_FreeRTOS.h>
#include <queue.h>
```
Mengimpor pustaka utama FreeRTOS, dan pustaka queue.h yang membawakan fitur Queue (antrean). Antrean berfungsi sebagai "pipa data" aman untuk saling lempar-tangkap informasi antar task yang terpisah.
```
#include <DHT.h> 
```
Mengimpor pustaka pihak ketiga yang diunduh dari Library Manager untuk membaca data dari sensor suhu dan kelembapan DHT.
```
#define DHTPIN 2       
#define DHTTYPE DHT11
```
Membuat konstanta global: DHTPIN diset ke angka 2 (kaki data sensor dihubungkan ke pin digital 2), dan DHTTYPE didefinisikan sebagai DHT11.
```
DHT dht(DHTPIN, DHTTYPE); 
```
Membuat sebuah objek bernama dht dari class dasar DHT, serta memasukkan konfigurasi pin dan tipe yang baru saja didefinisikan ke dalam objek tersebut.

2. Deklarasi Struktur Data dan Handle Antrean </br>
```
struct readings{
  int temp;
  int h;
};
```
Mendeklarasikan tipe data struktur buatan bernama readings. Ini ibarat keranjang yang memiliki dua sekat kotak memori: satu bernama temp (untuk suhu), dan h (untuk persentase humidity/kelembapan).
```
QueueHandle_t my_queue;
```
Membuat sebuah variabel pointer global bertipe Queue Handle bernama my_queue. Tujuannya sebagai tempat menyimpan referensi/nomor ID dari antrean yang akan kita ciptakan.

3. Inisialisasi Sistem, Sensor, dan Pembuatan Task (Fungsi setup)</br>
```
void setup() {
  Serial.begin(9600);
```
Awal blok fungsi setup(). Membuka jalur komunikasi serial ke laptop dengan kecepatan 9600 baud.
```
  dht.begin(); 
```
Mengaktifkan dan menginisialisasi modul sensor DHT agar siap melakukan pembacaan.
```
  my_queue = xQueueCreate(1, sizeof(struct readings));
```
Memerintahkan OS membuat satu "pipa antrean". Angka 1 berarti antrean ini kapasitasnya hanya 1 paket data di satu waktu. Ukuran paket datanya diatur dengan sizeof() sesuai dengan besar memori struktur readings. ID antrean ini disimpan ke variabel my_queue.
```
  xTaskCreate(read_data, "read sensors", 128, NULL, 0, NULL);
  xTaskCreate(display, "display", 128, NULL, 0, NULL);
```
Menciptakan Task yang mengeksekusi fungsi read_data dan Task display. Prioritas kedua task diset ke angka 0 (prioritas standar dan seimbang).

4. Fungsi Perulangan Utama </br>
```
void loop() {}
```
Fungsi loop() bawaan Arduino dikosongkan.

5. Definisi Task Producer (Membaca Sensor dan Mengirim Data) </br>
```
void read_data(void *pvParameters){
```
Definisi fungsi untuk task Producer (Penghasil Data) yang bertugas membaca sensor lingkungan.
```
  struct readings x;
```
Di dalam memori lokal task read_data, diciptakan sebuah variabel bernama x yang memiliki cetakan struktur readings.
```
  for(;;){
```
Ini merupakan cara lain penulisan perulangan yang tidak ada batas berhentinya (sama persis dengan while(1)).
```
    x.h = dht.readHumidity();       
```
Membaca persentase kelembapan di udara dari sensor, dan hasil angkanya disimpan ke dalam kotak memori h di dalam variabel struktur x.
```
    x.temp = dht.readTemperature(); 
```
Membaca nilai derajat Celcius dari sensor dan menyimpannya di kotak memori temp dalam keranjang x yang sama.
```
    xQueueSend(my_queue, &x, portMAX_DELAY);
```
Perintah RTOS untuk mengirim data. Artinya: "Kirim salinan data dari keranjang beralamat &x ke dalam pipa antrean my_queue."
Parameter portMAX_DELAY berarti, jika pipa antrean penuh (karena task display telat membacanya), task ini akan menunggu sampai antreannya kosong untuk bisa memasukkan data baru.
```
    vTaskDelay(2000 / portTICK_PERIOD_MS); 
```
Menidurkan task ini selama 2000 ms (2 detik). Sensor DHT lambat secara fisik; ia butuh jeda waktu sekitar 2 detik antar-pembacaan untuk mendapatkan hasil ukur yang akurat, makanya delay-nya diset menjadi 2 detik.

6. Definisi Task Consumer (Menerima dan Menampilkan Data) </br>
```
void display(void *pvParameters){
```
Definisi fungsi untuk task Consumer (Konsumen Data) yang kerjanya menampilkan data ke Serial Monitor.
```
  struct readings x;
```
Task display membuat variabel struktur readings lokal bernama x miliknya sendiri sebagai tempat kosong untuk menerima kiriman data.
```
  for(;;){
```
Memulai infinite loop untuk task display.
```
    if(xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS){
```
Memerintahkan pengambilan data. Artinya: "Ambil paket data dari antrean my_queue dan masukkan isinya ke dalam variabel lokal &x."
Karena ada portMAX_DELAY, jika antrean kosong, task display akan tidur. Task ini baru tereksekusi saat task read_data menembakkan data baru. Jika pengambilan berhasil (pdPASS), maka baris If di bawahnya dijalankan.
```
      Serial.print("temp = ");
      Serial.print(x.temp);
      Serial.println(" C");
```
Mencetak tulisan "temp = ", lalu mencetak nilai temp yang baru saja diambil dari keranjang x, dan ditutup dengan cetakan teks " C" (Celcius) beserta enter baris baru.
```
      Serial.print("humidity = ");
      Serial.print(x.h);
      Serial.println(" %");
```
Mencetak tulisan "humidity = ", mencetak angka h dari keranjang x, lalu diakhiri simbol " %" beserta enter.