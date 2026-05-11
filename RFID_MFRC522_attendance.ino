#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>

// WiFi credentials

//main part is hotspot set at 2.4GHZ Only then only get set 
//Enter your wifi name or ssid name 
const char* ssid = "";
// you can enter your wifi password
const char* password = "";

// Google Script Web App URL 
// set anyone with link 
String serverName = "";

// RFID pins (changed to avoid I2C conflict)
#define SS_PIN 5     // SDA → GPIO 5
#define RST_PIN 27   // RST → GPIO 27
MFRC522 rfid(SS_PIN, RST_PIN);

// LCD setup (I2C on default pins: SDA=21, SCL=22)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  lcd.clear();
  lcd.print("WiFi Connected");

  SPI.begin();       // uses default: SCK=18, MISO=19, MOSI=23
  rfid.PCD_Init();
}

void loop() {
  // Look for new RFID card
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Get UID
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

  Serial.println("Card UID: " + uid);

  // Here uid Means enter your RFID tag Number then only it RUN Properly Otherwise error or not run correctly 

  // if you not know or unknow with how you / we get rfid tag number then simply follow this command 
  // 1st is you upload on your microontroller then control + shift + M   ==> you entre on serial monitoring Screen 
  // there you see number and enter it and run 




  // Map UID to person details
  String name, phone, email;
  if (uid == "F1E74295") {
    name = "Sourabh";
    phone = "987";
    email = "sourabh@example.com";
  } 
  else if (uid == "934F6A5") {
    name = "girish";
    phone = "123457890";
    email = "girish@example.com";
  }
  else if (uid == "1672B95") {
    name = "Aditya";
    phone = "8844661235";
    email = "aditya@example.com";
  }
  else {
    name = "UNKNOW Person";
    phone = "-";
    email = "-";
  }

  // Date & time (replace with NTP/RTC later)
  String date = "2025-12-03";
  String time = "11.45";

  // Display on LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(name);
  lcd.setCursor(0,1);
  lcd.print("Entry OK");

  // Send to Google Sheets
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"name\":\"" + name + "\",\"id\":\"" + uid + "\",\"date\":\"" + date + "\",\"time\":\"" + time + "\",\"phone\":\"" + phone + "\",\"email\":\"" + email + "\"}";

    int httpResponseCode = http.POST(jsonData);
    Serial.println("HTTP Response code: " + String(httpResponseCode));
    http.end();
  }

  delay(3000); // wait before next scan
}