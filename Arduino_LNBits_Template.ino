#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>   // version 5

#define LN_ADDRESS_DESTINATION "lyricalweather78@walletofsatoshi.com"
#define SAT_AMOUNT 90

// === CONFIGURATION ===
const char* WIFI_SSID     = "xxxx";
const char* WIFI_PASSWORD = "xxxx";

// LNbits API info
#define LNBITS_BASE_URL "xxxx"
#define LNBITS_API_KEY "xxxx"
#define LNBITS_PAYMENTS_ENDPOINT "/api/v1/payments"

// === FUNCTIONS ===
String get_lnurl_invoice(String lightning_address, int amount_sats) {
  String name = lightning_address.substring(0, lightning_address.indexOf('@'));
  String domain = lightning_address.substring(lightning_address.indexOf('@') + 1);
  String lnurlp_url = "https://" + domain + "/.well-known/lnurlp/" + name;

  HTTPClient http;
  http.begin(lnurlp_url);
  int httpCode = http.GET();
  if (httpCode != 200) {
    Serial.printf("Failed to resolve lightning address. HTTP %d\n", httpCode);
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
  if (callback == "") {
    Serial.println("Callback URL missing.");
    return "";
  }

  // Request invoice with amount in millisats
  String callback_url = callback + "?amount=" + String(amount_sats * 1000);
  http.begin(callback_url);
  httpCode = http.GET();
  if (httpCode != 200) {
    Serial.printf("Failed to get invoice. HTTP %d\n", httpCode);
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

  String url = String(LNBITS_BASE_URL) + String(LNBITS_PAYMENTS_ENDPOINT);
  http.begin(url);
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
    Serial.println("✅ Payment sent successfully!");
    Serial.println("Payment hash: " + String((const char*) root["payment_hash"]));
  } else {
    Serial.println("❌ Payment failed.");
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

  String wos_ln_address = LN_ADDRESS_DESTINATION;
  int amount_sats = SAT_AMOUNT;

  delay(500);

  String invoice = get_lnurl_invoice(wos_ln_address, amount_sats);

  if (invoice.length() > 0) {
    Serial.println("Invoice (BOLT11):");
    Serial.println(invoice);

    pay_invoice(invoice);
  } else {
    Serial.println("Failed to get invoice.");
  }
}

void loop() {
  // nothing here
}
