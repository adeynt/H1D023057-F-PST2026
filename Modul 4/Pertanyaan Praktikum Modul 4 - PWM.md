# Praktikum Sistem Tertanam - Modul 4 PWM

## Pertanyaan Praktikum
1. Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi `analogWrite()`!
2. Apa hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255)?
3. Modifikasilah program berikut agar LED hanya menyala pada rentang kecerahan sedang, yaitu hanya ketika nilai PWM berada pada rentang 50 sampai 200. Jelaskan program pada file README.md.

## Jawaban

### 1. Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi `analogWrite()`!
LED dapat diatur kecerahannya menggunakan fungsi `analogWrite()` karena fungsi ini mengirimkan sinyal PWM (Pulse Width Modulation) ke pin yang terhubung dengan LED. PWM adalah teknik untuk menghasilkan sinyal analog semu dengan cara mengubah rasio waktu sinyal "nyala" dan "mati" (duty cycle). Semakin besar duty cycle (waktu "nyala" lebih lama), maka LED akan terlihat lebih terang. Sebaliknya, jika duty cycle kecil (waktu "nyala" lebih pendek), LED akan terlihat lebih redup. Fungsi `analogWrite()` di Arduino mengatur duty cycle dari sinyal PWM yang dikirimkan ke pin LED, sehingga LED dapat bervariasi kecerahannya.

### 2. Apa hubungan antara nilai ADC (0-1023) dan nilai PWM (0-255)?
Nilai ADC (0-1023) yang dibaca dari potensiometer adalah nilai analog yang dikonversi menjadi nilai digital dengan rentang 0 hingga 1023. Nilai ini menggambarkan posisi potensiometer yang berada di antara 0 dan 5 volt. Untuk menggunakan nilai ini dalam kontrol LED, kita perlu mengonversinya menjadi nilai PWM (0-255), yang merupakan rentang yang digunakan oleh fungsi `analogWrite()` di Arduino.
Untuk melakukan konversi, digunakan fungsi `map()` yang mengubah nilai ADC (0-1023) menjadi nilai PWM (0-255). Fungsi `map()` akan mengambil nilai ADC yang berkisar antara 0 hingga 1023 dan mengubahnya menjadi rentang yang sesuai untuk PWM, yaitu 0 hingga 255. Dengan kata lain, nilai ADC yang lebih tinggi akan menghasilkan nilai PWM yang lebih tinggi, yang membuat LED semakin terang.

### 3. Modifikasilah program berikut agar LED hanya menyala pada rentang kecerahan sedang, yaitu hanya ketika nilai PWM berada pada rentang 50 sampai 200.
#### Kode
```
#include <Arduino.h> // library dasar Arduino

// ===================== PIN SETUP =====================
// Tentukan pin yang digunakan untuk potensiometer dan LED PWM
const int potPin = A0;   // pin analog untuk potensiometer
const int ledPin = 9;    // pin digital PWM untuk LED

// ===================== VARIABEL =====================
// Variabel untuk menyimpan hasil pembacaan dan konversi PWM
int nilaiADC = 0;  // nilai awal ADC
int pwm = 0;       // nilai awal PWM

void setup() {

  // ===================== OUTPUT SETUP =====================
  // Atur pin LED sebagai output
  pinMode(ledPin, OUTPUT);

  // ===================== SERIAL MONITOR =====================
  // Aktifkan komunikasi serial untuk melihat data pembacaan
  Serial.begin(9600);
}

void loop() {

  // ===================== PEMBACAAN SENSOR =====================
  // Baca nilai analog dari potensiometer (rentang 0–1023)
  nilaiADC = analogRead(potPin);

  // ===================== PEMROSESAN DATA (SCALING) =====================
  // Ubah nilai ADC (0–1023) menjadi nilai PWM (0–255)
  pwm = map(nilaiADC, 0, 1023, 0, 255);

  // ===================== BATASAN KECERAHAN LED =====================
  // LED hanya menyala jika PWM berada di antara 50 dan 200
  if (pwm < 50 || pwm > 200) {
    pwm = 0; // LED mati jika berada di luar rentang kecerahan sedang
  }

  // ===================== OUTPUT PWM =====================
  // Kirim sinyal PWM ke LED untuk mengatur kecerahan
  analogWrite(ledPin, pwm);

  // ===================== MONITORING DATA =====================
  // Tampilkan data ADC dan PWM ke Serial Monitor
  Serial.print("ADC: ");
  Serial.print(nilaiADC);

  Serial.print(" | PWM: ");
  Serial.println(pwm);

  // ===================== STABILISASI SISTEM =====================
  // Delay untuk menstabilkan pembacaan dan tampilan data
  delay(50);
}
```

#### Penjelasan
1. Import Library </br>
Kode ini mengimpor library Arduino dengan menggunakan `#include <Arduino.h>`, yang memungkinkan penggunaan berbagai fungsi dasar Arduino, seperti analogRead(), analogWrite(), dan Serial.begin(), untuk memudahkan pengolahan sinyal dan komunikasi dengan komputer.

2. Deklarasi Pin dan Variabel
Pada bagian ini, pin A0 digunakan untuk membaca nilai potensiometer (yang berkisar antara 0 hingga 1023), sementara pin 9 digunakan untuk mengendalikan LED dengan sinyal PWM. Variabel nilaiADC menyimpan nilai dari potensiometer, dan variabel pwm digunakan untuk menyimpan nilai PWM yang akan mengatur kecerahan LED.

3. Setup
Pada fungsi `setup()`, pin LED diatur sebagai output dengan `pinMode()`, dan komunikasi serial diinisialisasi menggunakan Serial.begin(9600) untuk memantau nilai ADC dan PWM di Serial Monitor. Ini memungkinkan Arduino untuk mengirimkan data ke komputer.

4. Loop Utama
Fungsi `loop()` membaca nilai analog dari potensiometer dengan `analogRead()`, kemudian mengubah nilai ADC menjadi nilai PWM yang sesuai menggunakan fungsi `map()`. Jika nilai PWM berada di luar rentang yang diinginkan (50-200), nilai PWM diatur menjadi 0 untuk mematikan LED. Sinyal PWM dikirim ke pin LED dengan `analogWrite()`, dan data ADC serta PWM ditampilkan di Serial Monitor.