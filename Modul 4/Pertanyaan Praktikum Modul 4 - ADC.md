# Praktikum Sistem Tertanam - Modul 4 ADC

## Pertanyaan Praktikum
1. Apa fungsi perintah `analogRead()` pada rangkaian praktikum ini?
2. Mengapa diperlukan fungsi `map()` dalam program tersebut?
3. Modifikasi program berikut agar servo hanya bergerak dalam rentang 30° hingga 150°, meskipun potensiometer tetap memiliki rentang ADC 0–1023. Jelaskan program pada file README.md

## Jawaban

### 1. Apa fungsi perintah `analogRead()` pada rangkaian praktikum ini?
Perintah `analogRead()` berfungsi untuk membaca nilai analog dari potensiometer yang terhubung ke pin analog pada Arduino (dalam hal ini, pin A0). Fungsi ini mengonversi sinyal analog yang diterima oleh pin tersebut menjadi nilai digital dalam rentang 0 hingga 1023. Nilai ini kemudian digunakan untuk mengontrol posisi servo motor.

### 2. Mengapa diperlukan fungsi `map()` dalam program tersebut?
Fungsi `map()` diperlukan untuk mengonversi nilai yang dibaca dari `analogRead()` (rentang 0-1023) menjadi rentang yang sesuai untuk kontrol servo motor, yaitu 0-180 derajat. Servo motor hanya bisa bergerak dalam rentang 0-180 derajat, sementara potensiometer memberikan nilai ADC dalam rentang 0–1023. Fungsi map() mengubah nilai ADC yang besar ini menjadi rentang yang sesuai untuk sudut servo yang bisa dimengerti dan digunakan oleh motor.

### 3. Modifikasi program berikut agar servo hanya bergerak dalam rentang 30° hingga 150°, meskipun potensiometer tetap memiliki rentang ADC 0–1023
#### Kode
```
#include <Servo.h> // library untuk servo motor

Servo myservo; // membuat objek servo

// ===================== PIN SETUP =====================
const int potensioPin = A0;   // pin analog untuk potensiometer
const int servoPin = 9;       // pin digital untuk servo

// ===================== VARIABEL =====================
int pos = 0; // menyimpan sudut servo
int val = 0; // menyimpan nilai ADC dari potensiometer

void setup() {
  // Hubungkan servo ke pin yang sudah ditentukan
  myservo.attach(servoPin);

  // Aktifkan komunikasi serial untuk monitoring
  Serial.begin(9600);
}

void loop() {
  // ===================== PEMBACAAN ADC =====================
  // Membaca nilai potensiometer dengan rentang 0–1023
  val = analogRead(potensioPin);

  // ===================== KONVERSI DATA =====================
  // Mengubah nilai ADC menjadi sudut servo dalam rentang 30° hingga 150°
  pos = map(val, 0, 1023, 30, 150);  // Modifikasi rentang sudut servo

  // ===================== OUTPUT SERVO =====================
  // Menggerakkan servo sesuai hasil mapping
  myservo.write(pos);

  // ===================== MONITORING DATA =====================
  // Menampilkan nilai ADC dan sudut servo ke Serial Monitor
  Serial.print("ADC Potensio: ");
  Serial.print(val);
  Serial.print(" | Sudut Servo: ");
  Serial.println(pos);

  // ===================== STABILISASI =====================
  // Memberi jeda agar servo bergerak lebih stabil
  delay(15);
}
```

#### Penjelasan
1. Import Library </br>
Baris ini mengimpor pustaka Servo yang digunakan untuk mengontrol servo motor.

2. Membuat Objek Servo </br>
Mendeklarasikan objek servo bernama myservo yang akan digunakan untuk mengendalikan motor servo yang terhubung ke Arduino.

3. Deklarasi Pin dan Variabel </br>
Untuk menetapkan pin analog A0 sebagai pin input untuk membaca nilai dari potensiometer dan pin digital 9 sebagai pin output untuk mengendalikan motor servo. Selain itu juga mendeklarasikan variabel pos untuk menyimpan nilai sudut servo (0–180 derajat) dan variabel val untuk menyimpan nilai ADC (0–1023) yang dibaca dari potensiometer.

4. Setup </br>
`myservo.attach(servoPin);` untuk menghubungkan motor servo ke pin 9 pada Arduino, sesuai dengan pin yang telah ditetapkan sebelumnya. `Serial.begin(9600);` untuk menginisialisasi komunikasi serial pada kecepatan 9600 baud. Ini memungkinkan Arduino untuk mengirimkan data ke Serial Monitor di komputer.

5. Loop Utama </br>
Fungsi `analogRead(potensioPin)` digunakan untuk membaca nilai analog dari potensiometer yang terhubung ke pin A0, dengan rentang nilai 0 hingga 1023, yang mewakili posisi potensiometer dari yang paling rendah hingga paling tinggi. Nilai yang dibaca kemudian dipetakan ke rentang sudut servo 30° hingga 150° menggunakan fungsi `map(val, 0, 1023, 30, 150)`, yang memastikan bahwa pergerakan servo hanya berada dalam rentang tersebut meskipun potensiometer memberikan nilai ADC antara 0 hingga 1023. Setelah itu, fungsi `myservo.write(pos)` digunakan untuk menggerakkan servo sesuai dengan sudut yang telah dihitung dan disimpan dalam variabel pos. Program juga menampilkan nilai ADC dari potensiometer dan sudut servo pada Serial Monitor dengan menggunakan `Serial.print()` dan `Serial.println()`, yang memungkinkan pengguna untuk memantau perubahan nilai secara real-time. Untuk memastikan pergerakan servo lebih stabil dan mengurangi "jitter", fungsi `delay(15)` memberikan jeda selama 15 milidetik setelah setiap pergerakan servo.