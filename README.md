🧰 How to Install ESP32 Board (Espressif) on Arduino IDE

Board Target: LilyGo T-Display ESP32-S3
Recommended Version: 2.0.14

🪄 Step 1: Add ESP32 Board URL to Arduino Preferences

Open Arduino IDE.

Go to File → Preferences.

In the “Additional Boards Manager URLs” field, paste the following URL:

https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json


Click OK to save.

📥 Step 2: Install ESP32 Board Package

Go to Tools → Board → Boards Manager…

Type esp32 in the search box.

Find the entry named esp32 by Espressif Systems.

Select version 2.0.14 from the dropdown menu.

Click Install.

✅ Once installed, the package provides support for all ESP32 variants including ESP32-S3.

🧭 Step 3: Select LilyGo T-Display ESP32-S3 Board

Go to Tools → Board → esp32

Choose ESP32S3 Dev Module (works for LilyGo T-Display ESP32-S3 boards).

⚙️ Step 4: Configure Board Settings
Arduino IDE Setting	Value
Board	ESP32S3 Dev Module
Port	(Select the correct COM port)
USB CDC On Boot	Enable
CPU Frequency	240MHz (WiFi)
Flash Mode	QIO 80MHz
Flash Size	16MB (128Mb)
Partition Scheme	16M Flash (3M APP / 9.9MB FATFS)
PSRAM	OPI PSRAM
Upload Mode	UART0 / Hardware CDC
Upload Speed	921600
USB Mode	CDC and JTAG

📌 These settings match typical LilyGo T-Display ESP32-S3 boards and ensure USB works correctly for both flashing and Serial Monitor.

🧪 Step 5: Test with Example Sketch

Go to File → Examples → 01.Basics → Blink

Upload the sketch to confirm the board is working.

Open Serial Monitor (Ctrl+Shift+M) to check output if your code uses Serial.
