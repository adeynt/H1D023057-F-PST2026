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
  // Mengubah nilai ADC menjadi sudut servo 0–180 derajat
  pos = map(val, 0, 1023, 0, 180);

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