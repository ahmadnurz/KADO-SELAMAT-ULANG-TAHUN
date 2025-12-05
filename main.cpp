/*
  Happy Birthday + OLED SSD1306 Display with Password Lock
  Arduino Nano
  + Tombol Start/Input 1 pin D3 (button1Pin)
  + Tombol Input 2 pin D9 (button2Pin)
  + Fix Buzzer Tidak Bunyi Terus
  + FIX: Slideshow menggunakan print statis (tanpa efek ketik) agar teks muncul instan.
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Inisialisasi display OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ==== PIN BUZZER & TOMBOL ====
int speakerPin = 12;      // PIN BUZZER
int button1Pin = 3;       // TOMBOL START / INPUT 1 (D3)
int button2Pin = 9;       // TOMBOL INPUT 2 (D9)

// ==== KONFIGURASI PASSWORD ====
const String correctPassword = "1212"; // '1' = Tombol 1 (D3), '2' = Tombol 2 (D9)

// ==== DATA LAGU ====
int length = 28;
char notes[] = "GGAGcB GGAGdc GGxecBA yyecdc";
int beats[] = {
  2,2,8,8,8,16, 1,
  2,2,8,8,8,16, 1,
  2,2,8,8,8,8,16, 1,
  2,2,8,8,8,16
};
int tempo = 150;

// =======================
// FUNGSI FEEDBACK BUNYI
// =======================
void playClick() {
  // Bunyi klik pendek untuk feedback tombol
  // Menggunakan fungsi playTone agar lebih bersih
  int toneValue = 500; // Frekuensi
  int duration = 50;   // Durasi pendek
  long cycles = (long)duration * 1000L / (toneValue * 2);

  for (long i = 0; i < cycles; i++) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(toneValue);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(toneValue);
  }

  digitalWrite(speakerPin, LOW);
}

// =======================
// FUNGSI PLAY TONE FIX
// =======================
void playTone(int toneValue, int duration) {
  long cycles = (long)duration * 1000L / (toneValue * 2);

  for (long i = 0; i < cycles; i++) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(toneValue);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(toneValue);
  }

  digitalWrite(speakerPin, LOW);
}

// =======================
// FUNGSI PLAY NOTE FIX
// =======================
void playNote(char note, int duration) {
  char names[] = {
    'C','D','E','F','G','A','B',
    'c','d','e','f','g','a','b',
    'x','y'
  };

  // Nada dalam mikrodetik
  int tones[] = {
    1915,1700,1519,1432,1275,1136,1014,
     956, 853, 759, 716, 638, 568, 506,
     655, 715 // 'x' and 'y'
  };

  int SPEE = 5; // Kecepatan
  int found = 0;

  for (int i = 0; i < 17; i++) {
    if (names[i] == note) {
      int newduration = duration / SPEE;
      playTone(tones[i], newduration);
      found = 1;
      break;
    }
  }

  if (!found) {
    digitalWrite(speakerPin, LOW);
  }
}

// =======================
// FUNGSI TAMPILAN KETIK OLED
// Digunakan untuk efek ketik pada teks pendek/awal
// =======================
void typeOLED(String text, int x, int y, int speed) {
  display.setCursor(x, y);

  for (int i = 0; i < text.length(); i++) {
    display.print(text[i]);
    display.display();

    // Bunyi klik pendek setiap huruf
    playClick();

    delay(speed);    // kecepatan antar huruf
  }

  // Menghapus delay(500) di sini agar jika ada 
  // baris typeOLED berurutan, jedanya tidak terlalu lama.
  // delay(500); 
}


void playPasswordSound() {
  // Backsound sederhana (3 nada)
  playTone(500, 100); 
  delay(50);
  playTone(500, 100);
  delay(50);
  playTone(400, 100);
   delay(50);
  playTone(300, 100);
}

// =======================
// FUNGSI INPUT PASSWORD
// =======================
bool passwordEntry() {
  String inputPassword = "";
  int inputCount = 0;
  int buttonState1;
  int buttonState2;

  while (inputCount < correctPassword.length()) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Tampilkan prompt password
    display.setCursor(10, 20);
    display.println("Masukkan Password:");

    // Tampilkan masked input [****]
    display.setCursor(45, 35);
    display.print("[");
    for (int i = 0; i < correctPassword.length(); i++) {
      if (i < inputCount) {
        display.print("*");
      } else {
        display.print("-");
      }
    }
    display.println("]");
    display.display();

    // Tunggu input tombol
    bool inputReceived = false;
    while (!inputReceived) {
      // Menggunakan PULLUP, jadi LOW = Ditekan
      buttonState1 = digitalRead(button1Pin);
      buttonState2 = digitalRead(button2Pin);

      if (buttonState1 == LOW) {
        inputPassword += "1"; // Tombol 1 ditekan
        playClick();
        delay(200); // Debounce
        // Tunggu tombol dilepas untuk menghindari double-input
        while (digitalRead(button1Pin) == LOW);
        inputReceived = true;
      } else if (buttonState2 == LOW) {
        inputPassword += "2"; // Tombol 2 ditekan
        playClick();
        delay(200); // Debounce
        // Tunggu tombol dilepas
        while (digitalRead(button2Pin) == LOW);
        inputReceived = true;
      }
    }

    inputCount++;
  }

  // Tampilkan input terakhir [****]
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 20);
  display.println("Masukkan Password:");
  display.setCursor(45, 35);
  display.println("[****]");
  display.display();
  delay(500);

  // Cek Password
  if (inputPassword.equals(correctPassword)) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(40, 20);
    display.println("PASSWORD");
    display.setCursor(50, 35);
    display.println("BENAR");
    display.display();
    playPasswordSound();
    delay(2000);
    return true;
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(40, 20);
    display.println("PASSWORD");
    display.setCursor(50, 35);
    display.println("SALAH");
    display.display();
    delay(1500);
    return false;
  }
}

// =======================
//           SETUP
// =======================
void setup() {
  // Serial.begin(9600); // Opsional: untuk debug
  pinMode(speakerPin, OUTPUT);
  pinMode(button1Pin, INPUT_PULLUP);  // tombol aktif LOW
  pinMode(button2Pin, INPUT_PULLUP);  // tombol aktif LOW

  // OLED START
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    // Gagal inisialisasi, loop tak terbatas (hanya untuk perangkat keras)
    for(;;);
  }

  // TAMPILAN AWAL
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(30, 20);
  display.println("Tekan tombol");

  display.setCursor(32, 35);
  display.println("untuk mulai");

  display.display();
}

// =======================
//            LOOP
// =======================
void loop() {

  // ======================
  // MENUNGGU TOMBOL DITEKAN (START)
  // ======================
  while (digitalRead(button1Pin) == HIGH) {
    // Tombol 1 (D3) belum ditekan → tunggu
  }

  delay(200); // Debounce
  // Tambahkan tunggu tombol dilepas untuk START
  while (digitalRead(button1Pin) == LOW);


  // ======================
  // PROSES INPUT PASSWORD
  // ======================
  bool passwordCorrect = passwordEntry();

  if (!passwordCorrect) {
    // Password salah, kembali ke loop awal (menunggu start)
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(30, 20);
    display.println("Tekan tombol");
    display.setCursor(32, 35);
    display.println("untuk mulai");
    display.display();
    return; // Langsung kembali ke awal loop
  }


  // ======================
  // PASSWORD BENAR: TAMPILKAN UCAPAN
  // ======================
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 15);
  display.println("Happy");

  display.setCursor(10, 40);
  display.println("Birthday!");
  display.display();

  // ======================
  // MAIN LAGU
  // ======================
  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      // Memberi jeda (istirahat)
      digitalWrite(speakerPin, LOW); // Pastikan buzzer mati saat jeda
      delay(beats[i] * tempo);
    } else {
      playNote(notes[i], beats[i] * tempo);
    }

    // Jeda antar nada
    digitalWrite(speakerPin, LOW);
    delay(tempo);
  }

  delay(1500); // jeda selesai lagu

  // ==========================================
  // MODE SLIDESHOW (NEXT PAGE dgn tombol)
  // MENGGUNAKAN STATIC PRINT (BUKAN TYPEWRITER)
  // ==========================================

  int pageIndex = 0;    // mulai dari slide pertama
  bool slideshowRunning = true;
  int maxPages = 4; // Jumlah total halaman slideshow

  while (slideshowRunning) {

    // -----------------------------
    // Tampilkan slide sesuai halaman (STATIS)
    // -----------------------------
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE); // Pastikan warna sudah diset

    if (pageIndex == 0) {
      display.setTextSize(1);
      display.setCursor(10, 5);
      display.println("Selamat");
      display.setCursor(10, 20);
      display.println("Ulang Tahun ke-19");
      display.setTextSize(1);
      display.setCursor(10, 35);
      display.println("Ahmad Nur Yulhaidir");
      display.setCursor(90, 55);
      display.println("next>>");
    }

    else if (pageIndex == 1) {
      display.setCursor(5, 5);
      display.println("Semoga diberikan");
      display.setCursor(5, 20);
      display.println("umur yang panjang,");
      display.setCursor(5, 35);
      display.println("sehat, dan sukses");
      display.setCursor(5, 50);
      display.println("di setiap langkah.");
    }

    else if (pageIndex == 2) {
      display.setTextSize(2);
      display.setCursor(10, 20);
      display.println("Tetap");
      display.setCursor(10, 40);
      display.println("Semangat!");
    }

    else if (pageIndex == 3) {
      display.setTextSize(1);
      display.setCursor(10, 15);
      display.println("Terima kasih.");
      display.setCursor(10, 35);
      display.println("Tekan tombol untuk");
      display.setCursor(10, 50);
      display.println("memulai lagi!");
    }

    // TAMPILKAN SEMUA TEKS SEKALIGUS
    display.display();


    // -----------------------------
    // Tunggu tombol untuk NEXT PAGE
    // -----------------------------
    // Hanya tunggu tombol 1 (D3) untuk melanjutkan slideshow
    while (digitalRead(button1Pin) == HIGH) {
      // tunggu ditekan
    }

    playClick(); // Feedback tombol untuk pindah halaman
    delay(200); // debounce tombol

    // Tunggu tombol dilepas
    while (digitalRead(button1Pin) == LOW);

    pageIndex++;

    // Jika sudah halaman terakhir, keluar slideshow
    if (pageIndex >= maxPages) {
      slideshowRunning = false;
    }
  }

  // Setelah slideshow selesai, kembali ke loop awal (tampilan "Tekan tombol untuk mulai")
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(30, 20);
  display.println("Tekan tombol");
  display.setCursor(32, 35);
  display.println("untuk mulai");
  display.display();
}
