🧰 How to Install ESP32 Board (Espressif) on Arduino IDE<br>

Board Target: LilyGo T-Display ESP32-S3<br>
Recommended Version: 2.0.14<br>

#
![ESP32 LNbits Transfer](https://github.com/user-attachments/assets/e3a71137-c9e0-469b-a181-99d9060eccee)
#

🧰 Step 1: Add ESP32 Board URL to Arduino Preferences<br>

Open Arduino IDE.<br>

Go to File → Preferences.<br>

In the “Additional Boards Manager URLs” field, paste the following URL:<br>

https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json<br>


Click OK to save.<br>

#

📥 Step 2: Install ESP32 Board Package<br>

Go to Tools → Board → Boards Manager…<br>

Type esp32 in the search box.<br>

Find the entry named esp32 by Espressif Systems.<br>

Select version 2.0.14 from the dropdown menu.<br>

Click Install.<br>

✅ Once installed, the package provides support for all ESP32 variants including ESP32-S3.<br>

#

🧭 Step 3: Select LilyGo T-Display ESP32-S3 Board<br>

Go to Tools → Board → esp32<br>

Choose ESP32S3 Dev Module (works for LilyGo T-Display ESP32-S3 boards).<br>

#

⚙️ Step 4: Configure Board Settings<br>
Arduino IDE Setting	Value<br>
Board	ESP32S3 Dev Module<br>
Port	(Select the correct COM port)<br>
USB CDC On Boot	Enable<br>
CPU Frequency	240MHz (WiFi)<br>
Flash Mode	QIO 80MHz<br>
Flash Size	16MB (128Mb)<br>
Partition Scheme	16M Flash (3M APP / 9.9MB FATFS)<br>
PSRAM	OPI PSRAM<br>
Upload Mode	UART0 / Hardware CDC<br>
Upload Speed	921600<br>
USB Mode	CDC and JTAG<br>

![Configure ESP32](https://github.com/user-attachments/assets/83eb59c6-9605-4194-bf4a-9cfb923294ff)

📌 These settings match typical LilyGo T-Display ESP32-S3 boards and ensure USB works correctly for both flashing and Serial Monitor.<br>

#

🧪 Step 5: Test with Example Sketch<br>

Go to File → Examples → 01.Basics → Blink<br>

Upload the sketch to confirm the board is working.<br>

Open Serial Monitor (Ctrl+Shift+M) to check output if your code uses Serial.<br>

#

💻 Example Code: Sending Lightning Payments via LNbits<br>

Below is a simple example program that demonstrates how to use an ESP32 board (e.g. LilyGo T-Display ESP32-S3) to:<br>

Connect to Wi-Fi<br>

Resolve a Lightning Address to an LNURL-Pay endpoint<br>

Request a BOLT11 invoice for a specific amount<br>

Pay that invoice using an LNbits wallet API<br>

#

🧠 How the Code Works (Step by Step)<br>

Include Required Libraries<br>

#include <WiFi.h><br>
#include <HTTPClient.h><br>
#include <ArduinoJson.h><br>

WiFi.h: Connects ESP32 to your Wi-Fi network.<br>

HTTPClient.h: Handles HTTPS requests to LNURL and LNbits endpoints.<br>

ArduinoJson.h: Parses JSON responses (version 5 syntax is used here).<br>

Configure Wi-Fi and LNbits API<br>

const char* WIFI_SSID     = "xxxx";<br>
const char* WIFI_PASSWORD = "xxxx";<br>
const char* LNBITS_API_KEY = "xxxx";<br>
const char* LNBITS_API_URL = "xxxx";<br>

Fill in your Wi-Fi credentials and LNbits Admin key + API URL (e.g. https://yourlnbits.com/api/v1/payments).<br>

Resolve Lightning Address → Get Invoice<br>

String invoice = get_lnurl_invoice("lyricalweather78@walletofsatoshi.com", 21);<br>

Splits the Lightning address into name and domain.<br>

Requests https://domain/.well-known/lnurlp/name to get the callback URL.<br>

Calls the callback with an amount in millisatoshis to request an invoice.<br>

Returns a BOLT11 invoice string.<br>

Pay Invoice via LNbits API<br>

pay_invoice(invoice);<br>

Sends a POST request to your LNbits wallet with:<br>

If successful, it returns a payment_hash and prints confirmation to Serial Monitor.<br>

#

Main Flow<br>

Starts serial communication.<br>

Connects to Wi-Fi.<br>

Requests an invoice for 21 satoshis to a test Lightning address.<br>

Pays the invoice via LNbits.<br>

All status messages print to the Serial Monitor for debugging.<br>

#

📌 Key Notes<br>

This code uses ArduinoJson v5 syntax. If you have v6 installed, you’ll need to adapt the parsing lines.<br>

amount_sats is multiplied by 1000 because LNURL expects millisatoshis.<br>

Make sure your LNbits wallet has sufficient funds and the correct Admin key is used.<br>

Works best on ESP32-S3 boards with Wi-Fi connectivity and stable power.<br>

#
