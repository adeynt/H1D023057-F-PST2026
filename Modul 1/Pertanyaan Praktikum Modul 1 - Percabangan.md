# Praktikum Sistem Tertanam - Modul 1 Percabangan

## Pertanyaan Praktikum
1. Pada kondisi apa program masuk ke blok `if`?
2. Pada kondisi apa program masuk ke blok `else`?
3. Apa fungsi dari perintah `delay(timeDelay)`?
4. Jika program memiliki alur **mati → lambat → cepat → reset (mati)**, ubah menjadi: **lambat → cepat → sedang → mati (tanpa reset langsung)** dan jelaskan setiap baris kode.

## Jawaban

### 1. Pada kondisi apa program masuk ke blok `if`?

Program masuk ke blok `if` saat kondisi ini bernilai benar:
```
if (timeDelay <= 100)
```
Artinya, ketika nilai `timeDelay` kurang dari atau sama dengan 100, program akan menjalankan isi blok `if`, yaitu:
```
delay(3000);   // jeda sebelum reset
timeDelay = 1000; // reset ke awal
```
Jadi, blok `if` dipakai saat kedipan LED sudah sangat cepat, lalu program memberi jeda 3 detik dan mengembalikan `timeDelay` ke nilai awal 1000.

### 2. Pada kondisi apa program masuk ke blok `else`?
Program masuk ke blok else saat kondisi pada `if` tidak terpenuhi, yaitu ketika:
```
timeDelay > 100
```
Pada kondisi ini, program menjalankan:
```
timeDelay -= 100;
```
Artinya nilai `timeDelay` dikurangi 100 setiap selesai satu siklus nyala-mati LED, sehingga kedipan LED menjadi makin cepat secara bertahap.

### 3. Apa fungsi dari perintah `delay(timeDelay)`?
`delay(timeDelay)` berfungsi untuk memberi jeda sesuai nilai variabel `timeDelay` dalam milidetik.

Contoh penggunaan:
```
digitalWrite(ledPin, HIGH);
delay(timeDelay);
```
Bagian ini berarti LED dinyalakan, lalu program menunggu selama `timeDelay` milidetik, sehingga LED tetap menyala selama waktu tersebut.
```
digitalWrite(ledPin, LOW);
delay(timeDelay);
```
Bagian ini berarti LED dimatikan, lalu program menunggu selama `timeDelay` milidetik, sehingga LED tetap mati selama waktu tersebut.

Jadi, perintah ini mengatur lama nyala dan lama mati LED, yang akhirnya menentukan kecepatan kedip LED. Semakin besar `timeDelay`, semakin lambat kedipannya. Semakin kecil `timeDelay`, semakin cepat kedipannya.

### 4. Jika program memiliki alur **mati → lambat → cepat → reset (mati)**, ubah menjadi: **lambat → cepat → sedang → mati
#### Source Code
```
const int ledPin = 6;
int timeDelay = 1000;
int mode = 0;  // 0 = lambat ke cepat, 1 = cepat ke sedang, 2 = mati

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (mode == 2) {
    digitalWrite(ledPin, LOW); // pastikan LED mati
    while (true) {
      // berhenti total di kondisi mati
    }
  }

  digitalWrite(ledPin, HIGH);
  delay(timeDelay);

  digitalWrite(ledPin, LOW);
  delay(timeDelay);

  if (mode == 0) {
    timeDelay -= 100;   // dari lambat menuju cepat
    if (timeDelay <= 100) {
      mode = 1;
    }
  } 
  else if (mode == 1) {
    timeDelay += 100;   // dari cepat menuju sedang
    if (timeDelay >= 500) {
      mode = 2;
    }
  }
}
```
#### Penjelasan
1. Inisialisasi variabel
```
const int ledPin = 6;
```
Baris ini digunakan untuk menyimpan nomor pin yang terhubung ke LED, yaitu pin 6. Kata const berarti nilai tersebut tetap dan tidak akan diubah selama program berjalan.
```
int timeDelay = 1000;
```
Baris ini menyatakan variabel `timeDelay` dengan nilai awal 1000 milidetik atau 1 detik. Nilai ini menentukan lama jeda saat LED menyala dan mati. 
```
int mode = 0;
```
Variabel mode digunakan untuk mengatur tahapan alur program. Nilai 0 berarti alur dari lambat ke cepat, nilai 1 berarti alur dari cepat ke sedang, dan nilai 2 berarti LED masuk kondisi mati.

2. Fungsi `setup()`
```
void setup() {
  pinMode(ledPin, OUTPUT);
}
```
`pinMode(ledPin, OUTPUT);` berfungsi untuk mengatur pin 12 sebagai output, sehingga pin tersebut dapat digunakan untuk menyalakan dan mematikan LED.

3. Fungsi `loop()`
```
void loop() {
```
Fungsi `loop()` akan dijalankan berulang-ulang selama Arduino masih aktif. Semua alur utama program terjadi di dalam bagian ini.

4. Kondisi saat program masuk ke mode mati
```
if (mode == 2) {
  digitalWrite(ledPin, LOW); // pastikan LED mati
  while (true) {
    // berhenti total di kondisi mati
  }
}
```
Bagian ini akan dijalankan jika nilai mode sama dengan 2. Artinya, program sudah mencapai tahap akhir yaitu mati.`digitalWrite(ledPin, LOW);` digunakan untuk memastikan LED dalam keadaan mati. `while (true)` membuat program berhenti terus di situ, sehingga alur tidak kembali lagi ke awal. Ketika mode sudah 2, LED akan tetap mati dan program tidak melakukan reset langsung.

5. Menyalakan LED
```
digitalWrite(ledPin, HIGH);
delay(timeDelay);
```
Pada bagian ini LED dinyalakan dengan `digitalWrite(ledPin, HIGH);`. Setelah itu, program menjalankan `delay(timeDelay);`, yaitu menunggu selama nilai `timeDelay` milidetik. Selama jeda ini, LED tetap menyala.

6. Mematikan LED
```
digitalWrite(ledPin, LOW);
delay(timeDelay);
```
Setelah LED menyala, program mematikan LED dengan `digitalWrite(ledPin, LOW);`. Kemudian program kembali menunggu selama `timeDelay` milidetik. Pada jeda ini, LED tetap mati.

7. Lambat ke cepat
```
if (mode == 0) {
  timeDelay -= 100;
```
Jika mode bernilai 0, berarti program sedang berada pada tahap pertama, yaitu mengubah kedipan LED dari lambat menjadi cepat. `timeDelay -= 100;` berarti nilai `timeDelay` dikurangi 100 milidetik setiap selesai satu siklus kedip. Akibatnya, jeda semakin pendek dan LED berkedip semakin cepat.

8. Perpindahan dari cepat ke sedang
```
if (timeDelay <= 100) {
  mode = 1;
}
```
Jika `timeDelay` sudah mencapai 100 atau kurang, berarti LED sudah berada pada kondisi cepat. Maka nilai mode diubah menjadi 1. Perubahan ini menandakan bahwa program tidak lagi bergerak dari lambat ke cepat, tetapi masuk ke tahap berikutnya, yaitu dari cepat ke sedang.

9. Cepat ke sedang
```
else if (mode == 1) {
  timeDelay += 100;   // dari cepat menuju sedang
```
Jika mode bernilai 1, program berada pada tahap kedua. Pada tahap ini `timeDelay += 100;` membuat nilai `timeDelay` bertambah 100 setiap siklus. Karena jeda bertambah, kedipan LED yang sebelumnya sangat cepat akan menjadi lebih lambat, lalu mencapai kecepatan sedang.

10. Perpindahan dari sedang ke mati
```
if (timeDelay >= 500) {
  mode = 2;
}
```
Jika nilai `timeDelay` sudah mencapai 500, program menganggap LED telah berada pada kondisi sedang. Setelah itu, mode diubah menjadi 2. Artinya program masuk ke tahap akhir, yaitu mati. Pada pengulangan berikutnya, program akan menjalankan:
```
if (mode == 2)
```
sehingga LED dimatikan dan program berhenti total.

### Kesimpulan
Kesimpulannya, program masuk ke blok `if` saat nilai `timeDelay` kurang dari atau sama dengan 100, sedangkan program masuk ke blok `else` saat nilai `timeDelay` masih lebih besar dari 100. Perintah `delay(timeDelay)` berfungsi untuk memberi jeda sesuai nilai `timeDelay`, sehingga LED dapat tetap menyala atau tetap mati selama waktu tertentu. Pada program awal, alurnya adalah LED berkedip dari lambat menjadi cepat lalu kembali reset ke awal. Setelah dimodifikasi, alur program berubah menjadi **lambat → cepat → sedang → mati**, sehingga LED tidak langsung reset, tetapi berhenti pada kondisi mati setelah melalui semua tahap kecepatan.
