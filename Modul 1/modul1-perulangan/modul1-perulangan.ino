int timer = 100;
// delay. Semakin tinggi angkanya, semakin lambat waktunya.

void setup() {
  // gunakan loop for untuk menginisialisasi setiap pin sebagai output
  for (int ledPin = 2; ledPin < 8; ledPin++) {
    pinMode(ledPin, OUTPUT);
  }
}

void loop() {
  // looping dari pin rendah ke tinggi
  for (int ledPin = 2; ledPin < 8; ledPin++) {
    // hidupkan LED pin-nya
    digitalWrite(ledPin, HIGH);
    delay(timer);

    // matikan LED pin-nya
    digitalWrite(ledPin, LOW);
  }

  // looping dari pin tinggi ke rendah
  for (int ledPin = 7; ledPin >= 2; ledPin--) {
    // hidupkan LED pin-nya
    digitalWrite(ledPin, HIGH);
    delay(timer);

    // matikan LED pin-nya
    digitalWrite(ledPin, LOW);
  }
}