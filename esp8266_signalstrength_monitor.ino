#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "credentials.h"

#define LED_PIN 14
#define SCAN_PERIOD 2000
#define BLINK_TIME 100
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

ESP8266WiFiMulti WiFiMulti;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  // clear the display
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.display();

  // connect wifi
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID1, PASS1);
  WiFiMulti.addAP(SSID2, PASS2);
  Serial.print("Wait for wifi... ");
  int period_count = 0;
  while (WiFiMulti.run() != WL_CONNECTED) {
    char buffer[20];
    strcpy(buffer, "Connecting");
    int k=0;
    while (k++ < period_count) {
      strcat(buffer, ".");
    }
    period_count++;
    if (period_count > 3) period_count=0;
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println(buffer);
    display.display();
    delay(500);
  }

  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("Acq. SSID:\n");
  display.println(WiFi.SSID());
  display.display();
}

int32_t getRSSI(const char* target_ssid) {
  byte available_networks = WiFi.scanNetworks();

  for (int network = 0; network < available_networks; network++) {
    char ssid[16];
    WiFi.SSID(network).toCharArray(ssid, 16);
    if (strcmp(ssid, target_ssid) == 0) {
      return WiFi.RSSI(network);
    }
  }
  return 0;
}

void loop() {
  
char buffer[50];
  char ssid[16];
  WiFi.SSID().toCharArray(ssid, 16);
  sprintf(buffer, "Signal\nstrength\n%d RSSI", getRSSI(ssid));
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println(buffer);
  display.display();

  digitalWrite(LED_PIN, HIGH);
  delay(BLINK_TIME);
  digitalWrite(LED_PIN, LOW);
  delay(SCAN_PERIOD);
}
