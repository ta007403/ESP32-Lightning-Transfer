#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>   // version 5

// === CONFIGURATION ===
const char* WIFI_SSID     = "xxxx";
const char* WIFI_PASSWORD = "xxxx";

const char* LNBITS_API_KEY = "xxxx";
const char* LNBITS_API_URL = "xxxx";

// === FUNCTIONS ===
String get_lnurl_invoice(String lightning_address, int amount_sats) {
  String name = lightning_address.substring(0, lightning_address.indexOf('@'));
  String domain = lightning_address.substring(lightning_address.indexOf('@') + 1);

  String lnurlp_url = "https://" + domain + "/.well-known/lnurlp/" + name;

  HTTPClient http;
  http.begin(lnurlp_url);
  int httpCode = http.GET();
  if (httpCode != 200) {
    Serial.println("Failed to resolve lightning address.");
    http.end();
    return "";
  }

  String payload = http.getString();
  http.end();

  DynamicJsonBuffer jsonBuffer(512);
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (!root.success()) {
    Serial.println("JSON parse failed (step 1).");
    return "";
  }
  String callback = root["callback"].as<String>();

  // Request invoice with amount in millisats
  http.begin(callback + "?amount=" + String(amount_sats * 1000));
  httpCode = http.GET();
  if (httpCode != 200) {
    Serial.println("Failed to get invoice.");
    http.end();
    return "";
  }

  payload = http.getString();
  http.end();

  DynamicJsonBuffer jsonBuffer2(512);
  JsonObject& root2 = jsonBuffer2.parseObject(payload);
  if (!root2.success()) {
    Serial.println("JSON parse failed (step 2).");
    return "";
  }
  String pr = root2["pr"].as<String>();

  return pr;
}

void pay_invoice(String bolt11_invoice) {
  HTTPClient http;
  http.begin(LNBITS_API_URL);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Api-Key", LNBITS_API_KEY);

  String body = "{\"out\":true, \"bolt11\":\"" + bolt11_invoice + "\"}";
  int httpCode = http.POST(body);

  String payload = http.getString();
  http.end();

  if (httpCode == 201) {
    DynamicJsonBuffer jsonBuffer(512);
    JsonObject& root = jsonBuffer.parseObject(payload);
    if (!root.success()) {
      Serial.println("JSON parse failed (response).");
      return;
    }
    Serial.println("Payment sent successfully!");
    Serial.println("Payment hash: " + String((const char*) root["payment_hash"]));
  } else {
    Serial.println("Payment failed.");
    Serial.printf("HTTP %d\n", httpCode);
    Serial.println(payload);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("STEP 1: Serial OK");
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("STEP 2: WiFi begin done");

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  delay(500);

  // Example usage
  String wos_ln_address = "lyricalweather78@walletofsatoshi.com";  // Lightning Address you need to be a destination
  int amount_sats = 21; // Amount you need to transfer sat

  delay(500);

  String invoice = get_lnurl_invoice(wos_ln_address, amount_sats);
  
  if (invoice.length() > 0) {
    Serial.println("Invoice (BOLT11):");
    Serial.println(invoice);
    
    pay_invoice(invoice);
  }
}

void loop() {
  // nothing here
}
