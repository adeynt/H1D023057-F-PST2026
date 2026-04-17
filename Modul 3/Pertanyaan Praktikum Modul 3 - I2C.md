# Praktikum Sistem Tertanam - Modul 3 I2C

## Pertanyaan Praktikum
1. Jelaskan bagaimana cara kerja komunikasi I2C antara Arduino dan LCD pada rangkaian tersebut!
2. Apakah pin potensiometer harus seperti itu? Jelaskan yang terjadi apabila pin kiri dan pin kanan tertukar!
3. Modifikasi program dengan menggabungkan antara UART dan I2C (keduanya sebagai output) sehingga:
- Data tidak hanya ditampilkan di LCD tetapi juga di Serial Monitor
- Adapun data yang ditampilkan pada Serial Monitor sesuai dengan table berikut:

| ADC: 0 | Volt: 0.00 V | Persen: 0% |

Tampilan jika potensiometer dalam kondisi diputar paling kiri
- ADC: 0 0% | setCursor(0, 0) dan Bar (level) | setCursor(0, 1)
- Berikan penjelasan disetiap baris kode nya dalam bentuk README.md!
4. Lengkapi table berikut berdasarkan pengamatan pada Serial Monitor

| ADC | Volt (V) | Persen (%) |
|-----|----------|------------|
| 1   | 0.00     | 0.10       |
| 21  | 0.10     | 2.05       |
| 49  | 0.24     | 4.79       |
| 74  | 0.36     | 7.23       |
| 96  | 0.47     | 9.38       |

## Jawaban

### 1. Jelaskan bagaimana cara kerja komunikasi I2C antara Arduino dan LCD pada rangkaian tersebut!
Pada rangkaian ini, komunikasi antara Arduino dan LCD menggunakan protokol I2C. I2C adalah komunikasi serial yang hanya memerlukan dua jalur utama, yaitu SDA (Serial Data) untuk pengiriman data dan SCL (Serial Clock) untuk sinkronisasi clock. Pada Arduino Uno, jalur SDA berada di pin A4 dan SCL berada di pin A5, selain itu LCD juga mendapat catu daya melalui pin VCC dan GND. Pada komunikasi ini, Arduino berperan sebagai master, sedangkan LCD I2C berperan sebagai slave.

Cara kerjanya dimulai ketika Arduino membaca nilai analog dari potensiometer melalui pin A0 menggunakan `analogRead(pinPot)`. Nilai tersebut kemudian diproses oleh Arduino untuk ditampilkan ke LCD. Agar LCD dapat menerima data, Arduino mengirimkan data melalui jalur SDA dan SCL ke alamat I2C LCD yang telah ditentukan pada kode, yaitu `0x27`. Setelah itu, LCD menampilkan data pada posisi kursor yang telah diatur dengan `lcd.setCursor()`. Pada program ini, baris pertama LCD digunakan untuk menampilkan nilai ADC, sedangkan baris kedua digunakan untuk menampilkan indikator level dalam bentuk bar.

### 2. Apakah pin potensiometer harus seperti itu? Jelaskan yang terjadi apabila pin kiri dan pin kanan tertukar!
Pin potensiometer tidak harus persis seperti itu, tetapi ada satu bagian yang harus tetap, yaitu pin tengah harus terhubung ke A0 karena pin tengah merupakan keluaran yang dibaca oleh ADC Arduino. Sementara itu, dua pin samping dihubungkan ke 5V dan GND.

Apabila pin kiri dan pin kanan tertukar, rangkaian tetap dapat bekerja dan tidak merusak komponen. Namun, arah perubahan nilainya akan menjadi terbalik. Jika sebelumnya saat potensiometer diputar ke kanan nilai ADC meningkat, maka setelah ditukar justru saat diputar ke kanan nilai ADC menurun. Jadi, yang berubah bukan fungsi dasarnya, melainkan arah pembacaannya.

### 3. Modifikasi program dengan menggabungkan antara UART dan I2C (keduanya sebagai output)
#### Kode
```
#include <Wire.h>                  // Memanggil library Wire untuk komunikasi I2C
#include <LiquidCrystal_I2C.h>     // Memanggil library untuk mengontrol LCD I2C
#include <Arduino.h>               // Memanggil library dasar Arduino

LiquidCrystal_I2C lcd(0x27, 16, 2); // Membuat objek LCD dengan alamat I2C 0x27, 16 kolom, 2 baris

const int pinPot = A0;             // Menentukan bahwa potensiometer terhubung ke pin analog A0

void setup() {                     // Fungsi setup dijalankan satu kali saat Arduino mulai aktif
  Serial.begin(9600);              // Memulai komunikasi serial dengan baud rate 9600 bps
  lcd.init();                      // Menginisialisasi LCD agar siap digunakan
  lcd.backlight();                 // Menyalakan lampu latar LCD
}                                  // Menutup fungsi setup

void loop() {                      // Fungsi loop dijalankan berulang terus-menerus
  int nilai = analogRead(pinPot);  // Membaca nilai ADC dari potensiometer pada pin A0
  float volt = nilai * 5.0 / 1023.0; // Mengubah nilai ADC menjadi tegangan dalam volt
  int persen = nilai * 100 / 1023; // Mengubah nilai ADC menjadi persentase 0–100%
  int panjangBar = map(nilai, 0, 1023, 0, 16); // Memetakan nilai ADC ke panjang bar 0–16 kolom

  Serial.print("ADC: ");           // Menampilkan teks "ADC: " pada Serial Monitor
  Serial.print(nilai);             // Menampilkan nilai ADC ke Serial Monitor
  Serial.print(" | Volt: ");       // Menampilkan teks pemisah " | Volt: "
  Serial.print(volt, 2);           // Menampilkan nilai tegangan dengan 2 angka di belakang koma
  Serial.print(" V | Persen: ");   // Menampilkan teks pemisah " V | Persen: "
  Serial.print(persen);            // Menampilkan nilai persentase ke Serial Monitor
  Serial.println("%");             // Menampilkan simbol persen lalu pindah ke baris berikutnya

  lcd.setCursor(0, 0);             // Meletakkan kursor di kolom 0 baris 0 LCD
  lcd.print("ADC: ");              // Menampilkan teks "ADC: " pada baris pertama LCD
  lcd.print(nilai);                // Menampilkan nilai ADC pada LCD
  lcd.print(" ");                  // Memberi satu spasi sebagai pemisah
  lcd.print(persen);               // Menampilkan nilai persentase pada LCD
  lcd.print("%   ");               // Menampilkan simbol persen dan spasi untuk menghapus sisa karakter lama

  lcd.setCursor(0, 1);             // Meletakkan kursor di kolom 0 baris 1 LCD
  for (int i = 0; i < 16; i++) {   // Melakukan perulangan sebanyak 16 kali sesuai jumlah kolom LCD
    if (i < panjangBar) {          // Jika indeks kolom masih kurang dari panjang bar
      lcd.print((char)255);        // Menampilkan karakter blok penuh sebagai indikator level
    } else {                       // Jika indeks kolom sudah melebihi panjang bar
      lcd.print(" ");              // Menampilkan spasi kosong pada sisa kolom
    }                              // Menutup blok if-else
  }                                // Menutup perulangan for

  delay(200);                      // Memberikan jeda 200 milidetik agar tampilan tidak terlalu cepat berubah
}                                  // Menutup fungsi loop
```
#### Penjelasan
1. Deklarasi LCD</br>
Pada bagian ini dibuat objek LCD I2C dengan alamat 0x27, ukuran 16 kolom, dan 2 baris. Deklarasi ini diperlukan agar Arduino dapat mengakses dan mengendalikan LCD sesuai alamat perangkat yang digunakan.
2. Deklarasi Pin Potensiometer</br>
Bagian ini digunakan untuk menentukan bahwa potensiometer terhubung ke pin analog A0. Pin tersebut akan dipakai Arduino untuk membaca nilai tegangan dari potensiometer dalam bentuk data ADC.
3. Fungsi setup()</br>
Fungsi setup() dijalankan satu kali saat Arduino mulai aktif. Pada bagian ini, komunikasi serial diinisialisasi dengan baud rate 9600 bps agar data dapat ditampilkan pada Serial Monitor. Selain itu, LCD juga diinisialisasi dan lampu latarnya dinyalakan agar tampilan pada layar dapat terlihat.
4. Fungsi loop()</br>
Fungsi loop() dijalankan secara terus-menerus selama Arduino aktif. Pada bagian ini, Arduino membaca nilai ADC dari potensiometer, kemudian mengubahnya menjadi nilai tegangan dan persentase. Setelah itu, nilai tersebut juga dipetakan menjadi panjang bar agar dapat ditampilkan dalam bentuk indikator level pada LCD.
5. Proses Pembacaan Nilai Potensiometer</br>
Arduino membaca nilai analog dari pin A0 menggunakan analogRead(). Nilai yang diperoleh berada pada rentang 0 sampai 1023. Nilai ini merepresentasikan besar tegangan yang dihasilkan oleh potensiometer berdasarkan posisi putarannya.
6. Proses Konversi ke Tegangan dan Persentase</br>
Nilai ADC yang telah diperoleh kemudian diubah menjadi nilai tegangan dalam satuan volt dengan asumsi tegangan referensi sebesar 5V. Selain itu, nilai tersebut juga diubah ke dalam bentuk persentase agar lebih mudah dipahami. Dengan demikian, satu input potensiometer dapat ditampilkan dalam tiga bentuk, yaitu ADC, volt, dan persen.
7. Proses Pemetaan ke Bar LCD</br>
Nilai ADC dipetakan ke panjang bar dengan rentang 0 sampai 16. Hal ini dilakukan karena LCD memiliki 16 kolom, sehingga panjang indikator level dapat disesuaikan dengan besar kecilnya nilai potensiometer.
8. Output ke Serial Monitor</br>
Pada bagian ini, program menampilkan data hasil pembacaan ke Serial Monitor. Data yang ditampilkan meliputi nilai ADC, nilai tegangan dalam volt, dan nilai persentase. Tampilan ini memudahkan pengguna untuk memantau hasil pembacaan secara detail melalui komputer.
9. Output ke LCD Baris Pertama</br>
Baris pertama LCD digunakan untuk menampilkan nilai ADC dan nilai persentase. Posisi tampilan diatur menggunakan lcd.setCursor(0, 0) agar data selalu muncul pada baris pertama layar LCD.
9. Output ke LCD Baris Kedua</br>
Baris kedua LCD digunakan untuk menampilkan indikator level dalam bentuk bar. Program melakukan perulangan sebanyak 16 kali sesuai jumlah kolom LCD. Jika posisi kolom masih berada dalam panjang bar yang telah ditentukan, maka LCD akan menampilkan karakter blok penuh. Jika tidak, kolom akan dibiarkan kosong.
10. Jeda Pembaruan Tampilan</br>
Program menggunakan delay(200) untuk memberikan jeda selama 200 milidetik pada setiap siklus pembacaan. Tujuannya agar tampilan pada LCD dan Serial Monitor tidak berubah terlalu cepat sehingga hasil pembacaan lebih mudah diamati.

### 4. Lengkapi table berikut berdasarkan pengamatan pada Serial Monitor

| ADC | Volt (V) | Persen (%) |
|-----|----------|------------|
| 1   | 0.00     | 0          |
| 21  | 0.10     | 2          |
| 49  | 0.24     | 4          |
| 74  | 0.36     | 7          |
| 96  | 0.47     | 9          |