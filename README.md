Happy Birthday OLED + Password Lock (Arduino Nano)

Proyek ini merupakan sistem ucapan ulang tahun interaktif menggunakan Arduino Nano, OLED SSD1306, dual-button password input, dan buzzer yang memainkan lagu.
Pengguna harus memasukkan password terlebih dahulu sebelum animasi ucapan, pemutaran lagu, dan slideshow ditampilkan.

✨ Fitur Utama

🔐 Password Lock System (default: 1212, bisa diganti)

🎵 Pemutar lagu dengan nada yang disusun manual

🖥️ OLED SSD1306 untuk tampilan teks animasi & slideshow

🔊 Click sound setiap tombol ditekan

▶️ Slideshow ucapan dengan tombol untuk next page

🎂 Ucapan ulang tahun lengkap

🛠️ Komponen yang Digunakan
Komponen	Jumlah
Arduino Nano	1
OLED SSD1306 I2C 128x64	1
Buzzer aktif/pasif	1
Push Button	2
Kabel jumper	—
🗂️ Wiring Diagram
OLED SSD1306 (I2C)
OLED	Arduino Nano
VCC	5V
GND	GND
SDA	A4
SCL	A5
Buzzer
Buzzer	Arduino
+	D12
-	GND
Tombol
Tombol	Arduino
Button 1	D3
Button 2	D9

Menggunakan INPUT_PULLUP → tombol aktif saat LOW

🔑 Password

Password default:

const String correctPassword = "1212";


Bisa diubah sesuai kebutuhan.

📺 Alur Program

Tampilan awal: "Tekan tombol untuk mulai".

Pengguna menekan tombol → masuk ke menu password.

Sistem menampilkan [****] sesuai jumlah input.

Jika password benar:

Menampilkan teks “Happy Birthday!”

Memainkan lagu

Menampilkan slideshow ucapan

Jika salah:

Menampilkan "PASSWORD SALAH"

Kembali ke menu awal

🖼️ Slideshow Ucapan

Terdapat 4 halaman yang bisa dipindah menggunakan tombol:

Selamat ulang tahun + nama

Doa & harapan

Pesan personal

Thank you

📦 Library yang Dibutuhkan

Pastikan library berikut sudah terinstall:

Adafruit SSD1306

Adafruit GFX

Wire

▶️ Cara Menggunakan

Upload kode ke Arduino Nano.

Hubungkan OLED dan tombol sesuai wiring.

Nyalakan perangkat.

Tekan tombol untuk mulai.

Masukkan password menggunakan:

Button 1 → angka “1”

Button 2 → angka “2”

Setelah benar → sistem akan memulai animasi ucapan.

💻 Kode Program Lengkap

Kode lengkap tersedia pada file:

main.ino

📝 Lisensi

Proyek ini bebas digunakan untuk keperluan pribadi, hadiah, pembelajaran, atau pengembangan lebih lanjut.

❤️ Credit

Dibuat dengan cinta untuk keperluan ucapan ulang tahun interaktif menggunakan Arduino.
