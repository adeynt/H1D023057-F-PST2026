# Praktikum Sistem Tertanam - Modul 3 UART

## Pertanyaan Praktikum
1. Jelaskan proses dari input keyboard hingga LED menyala/mati!
2. Mengapa digunakan `Serial.available()` sebelum membaca data? Apa yang terjadi jika baris tersebut dihilangkan?
3. Modifikasi program agar LED berkedip (blink) ketika menerima input '2' dengan kondisi jika ‘2’ aktif maka LED akan terus berkedip sampai perintah selanjutnya diberikan dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!
4. Tentukan apakah menggunakan `delay()` atau `milis()`! Jelaskan pengaruhnya terhadap sistem

## Jawaban

### 1. Jelaskan proses dari input keyboard hingga LED menyala/mati!
1. Input dari Keyboard pada Serial Monitor</br>
Proses dimulai ketika pengguna mengetik karakter pada Serial Monitor, misalnya `1` atau `0`. Karakter tersebut kemudian dikirim dari komputer ke Arduino melalui komunikasi serial dengan baud rate 9600 bps, sesuai inisialisasi pada `Serial.begin(9600);`.
2. Pemeriksaan Data yang Masuk </br>
Setelah data masuk ke Arduino, program pada fungsi `loop()` akan memeriksa apakah terdapat data yang tersedia di buffer serial menggunakan `Serial.available()`. Langkah ini penting untuk memastikan bahwa data benar-benar ada sebelum dibaca.
3. Pembacaan Data Serial</br>
Apabila terdapat data pada buffer serial, program akan membaca satu karakter menggunakan `Serial.read()` dan menyimpannya ke dalam variabel data. Karakter inilah yang selanjutnya akan diproses sebagai perintah untuk mengendalikan LED.
3. Proses Percabangan Perintah </br>
Setelah data berhasil dibaca, program melakukan percabangan untuk menentukan aksi yang harus dilakukan. Jika karakter yang diterima adalah `1`, maka Arduino menjalankan `digitalWrite(PIN_LED, HIGH);` sehingga pin 12 bernilai `HIGH` dan LED menyala. Jika karakter yang diterima adalah `0`, maka Arduino menjalankan `digitalWrite(PIN_LED, LOW);` sehingga LED mati.
4. Penanganan Input yang Tidak Sesuai </br>
Apabila karakter yang diterima bukan `1`, `0`, atau karakter `ENTER`, maka program akan menampilkan pesan **“Perintah tidak dikenal”** pada Serial Monitor. Hal ini bertujuan untuk memberi informasi kepada pengguna bahwa input yang dimasukkan tidak sesuai dengan perintah yang tersedia.

### 2. Mengapa digunakan Serial.available() sebelum membaca data? Apa yang terjadi jika baris tersebut dihilangkan?
`Serial.available()` digunakan untuk memastikan bahwa benar-benar ada data yang masuk ke buffer serial sebelum program mencoba membacanya. Fungsi ini penting agar `Serial.read()` hanya dijalankan saat data tersedia.

Apabila baris `Serial.available()` dihilangkan, maka program akan tetap mencoba membaca data setiap kali `loop()` berjalan, meskipun sebenarnya tidak ada data yang masuk. Hal ini dapat menyebabkan pembacaan data tidak valid, misalnya menghasilkan nilai kosong atau `-1`. Akibatnya, program dapat memberikan respons yang tidak sesuai, seperti menampilkan pesan kesalahan terus-menerus atau membaca input yang sebenarnya belum dikirim oleh pengguna.

### 3. Modifikasi program agar LED berkedip (blink) ketika menerima input '2' dengan kondisi jika ‘2’ aktif maka LED akan terus berkedip sampai perintah selanjutnya diberikan!
#### Kode
```
const int PIN_LED = 12;                 // Menentukan LED terhubung ke pin digital 12

char modeLED = '0';                     // Menyimpan mode LED terakhir: '0' = mati, '1' = nyala, '2' = blink
bool ledState = LOW;                    // Menyimpan status LED saat ini
unsigned long previousMillis = 0;       // Menyimpan waktu perubahan LED terakhir
const unsigned long interval = 500;     // Interval kedip LED 500 ms

void setup() {
  Serial.begin(9600);                   // Memulai komunikasi serial dengan baud rate 9600
  Serial.println("Ketik '1' untuk menyalakan LED, '0' untuk mematikan LED, '2' untuk blink");
  pinMode(PIN_LED, OUTPUT);             // Mengatur pin LED sebagai output
  digitalWrite(PIN_LED, LOW);           // Memastikan LED mati saat awal
}

void loop() {
  if (Serial.available() > 0) {         // Mengecek apakah ada data dari komputer
    char data = Serial.read();          // Membaca 1 karakter dari Serial Monitor

    if (data == '1') {
      modeLED = '1';                    // Mode LED menjadi menyala
      ledState = HIGH;                  // Status LED diatur HIGH
      digitalWrite(PIN_LED, HIGH);      // Menyalakan LED
      Serial.println("LED ON");
    }
    else if (data == '0') {
      modeLED = '0';                    // Mode LED menjadi mati
      ledState = LOW;                   // Status LED diatur LOW
      digitalWrite(PIN_LED, LOW);       // Mematikan LED
      Serial.println("LED OFF");
    }
    else if (data == '2') {
      modeLED = '2';                    // Mode LED menjadi blink
      Serial.println("LED BLINK");
    }
    else if (data != '\n' && data != '\r') {
      Serial.println("Perintah tidak dikenal");
    }
  }

  if (modeLED == '2') {                 // Jika mode blink aktif
    unsigned long currentMillis = millis();   // Mengambil waktu saat ini

    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;   // Memperbarui waktu terakhir
      ledState = !ledState;             // Membalik status LED
      digitalWrite(PIN_LED, ledState);  // Mengubah kondisi LED
    }
  }
}
```
#### Penjelasan
1. Deklarasi Pin</br>
Pada bagian ini, program mendefinisikan pin yang digunakan untuk LED, yaitu pin digital 12.
2. Variabel Kontrol LED</br>
Bagian ini berisi variabel-variabel yang digunakan untuk mengatur perilaku LED. Variabel `modeLED` berfungsi untuk menyimpan mode terakhir LED, apakah mati, menyala, atau berkedip. Variabel `ledState` digunakan untuk menyimpan kondisi LED saat ini. Sementara itu, `previousMillis` dan `interval` digunakan untuk mendukung proses kedipan LED dengan metode `millis()`, sehingga LED dapat berkedip tanpa menghentikan jalannya program utama.
3. Fungsi `setup()`</br>
Fungsi `setup()` dijalankan satu kali saat Arduino mulai aktif. Pada bagian ini, komunikasi serial diinisialisasi dengan baud rate 9600 bps agar Arduino dapat menerima perintah dari Serial Monitor. Selain itu, pin LED diatur sebagai output dan LED dipastikan berada dalam kondisi mati saat awal program dijalankan. Program juga menampilkan petunjuk penggunaan pada Serial Monitor agar pengguna mengetahui perintah yang tersedia.
4. Fungsi `loop()`</br>
Fungsi `loop()` dijalankan secara terus-menerus selama Arduino aktif. Pada bagian ini, program memeriksa apakah ada data yang masuk dari Serial Monitor menggunakan `Serial.available()`. Jika ada, program membaca satu karakter dengan `Serial.read()` lalu memprosesnya dengan percabangan. Jika input adalah `1`, LED akan menyala. Jika input adalah `0`, LED akan mati. Jika input adalah `2`, mode LED berubah menjadi berkedip. Jika input tidak sesuai, program akan menampilkan pesan kesalahan pada Serial Monitor.
5. Proses Pembacaan Input Serial</br>
Pada bagian pembacaan input, Arduino menerima perintah dari pengguna dalam bentuk karakter. Karakter ini menjadi dasar untuk menentukan aksi yang akan dilakukan sistem. Dengan cara ini, komunikasi antara komputer dan Arduino berlangsung secara sederhana tetapi tetap efektif untuk mengendalikan perangkat output seperti LED.
6. Proses Blink LED</br>
Bagian ini hanya dijalankan ketika mode LED berada pada kondisi `2`. Program menggunakan `millis()` untuk menghitung selang waktu tanpa menghentikan eksekusi program. Jika selang waktu yang ditentukan telah tercapai, status LED akan dibalik, dari menyala menjadi mati atau sebaliknya. Dengan metode ini, LED dapat terus berkedip sambil sistem tetap siap menerima perintah baru dari Serial Monitor.
7. Penanganan Input Tidak Valid</br>
Program juga memiliki mekanisme untuk menangani input yang tidak sesuai. Jika karakter yang diterima bukan `1`, `0`, `2`, atau karakter ENTER, maka Arduino akan menampilkan pesan **“Perintah tidak dikenal”**. Hal ini penting agar pengguna mengetahui bahwa input yang diberikan tidak termasuk perintah yang telah ditentukan.

### 4. Tentukan apakah menggunakan `delay()` atau `milis()`! Jelaskan pengaruhnya terhadap sistem
Pada program ini, metode yang digunakan adalah `millis()`. Alasan utamanya karena `millis()` bersifat **non-blocking**, sehingga program tetap dapat membaca input baru dari Serial Monitor meskipun LED sedang berkedip.
Sedangkan, `delay()` bersifat **blocking**, yaitu menghentikan sementara jalannya program selama waktu tertentu. Jika `delay()` digunakan, Arduino akan terlambat merespons perintah baru karena harus menunggu proses penundaan selesai terlebih dahulu. Pengaruh penggunaan `millis()` terhadap sistem adalah sistem menjadi lebih responsif, LED tetap dapat berkedip secara periodik, dan program menjadi lebih efisien serta lebih mudah dikembangkan untuk fungsi tambahan.