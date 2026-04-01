int timer = 100;  
// delay. Semakin besar nilainya, semakin lambat perpindahan LED.

void setup() {
  // Inisialisasi pin 11 sampai 14 sebagai OUTPUT
  for (int ledPin = 11; ledPin <= 14; ledPin++) {
    pinMode(ledPin, OUTPUT);
  }
}

void loop() {
  // Looping dari pin rendah ke tinggi
  for (int ledPin = 11; ledPin <= 14; ledPin++) {
    digitalWrite(ledPin, HIGH);  
    delay(timer);                
    digitalWrite(ledPin, LOW);   
  }

  // Looping dari pin tinggi ke rendah
  for (int ledPin = 14; ledPin >= 11; ledPin--) {
    digitalWrite(ledPin, HIGH);  
    delay(timer);                
    digitalWrite(ledPin, LOW);   
  }
}