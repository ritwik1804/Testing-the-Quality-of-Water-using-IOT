#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_TEMPLATE_ID "TMPLJVB2lRs3"
#define BLYNK_TEMPLATE_NAME "Water Quality"
#define BLYNK_AUTH_TOKEN "nhQxSYV3NxeqBr27IJZWwsgpBaZAb4kl"

// Replace with your Wi-Fi network credentials
char ssid[] = "Sidhartha's Galaxy S20 FE 5G";
char password[] = "943032175";

// Replace with your Blynk auth token
char auth[] = "nhQxSYV3NxeqBr27IJZWwsgpBaZAb4kl";


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// OneWire bus and temperature sensor
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// TDS meter variables
int tdsValue = 0;
float tdsTemp = 25.0;

// Blynk virtual pins
#define TDS_PIN A0
#define TEMP_PIN D3

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Start Blynk
  Blynk.begin(auth, ssid, password);

  // Initialize display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Initialize temperature sensor
  sensors.begin();
}

void loop() {
  // Read temperature
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  // Read TDS value
  tdsValue = analogRead(A0);
  tdsTemp = 1.0 / (1.0 + 0.02 * (tempC - 25.0));
  float tdsPPM = tdsValue * (tdsTemp / 0.5) * 2.0;

  // Display data on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("TDS Meter");
  display.setCursor(0, 10);
  display.println("TDS: " + String(tdsPPM) + " ppm");
  display.setCursor(0, 20);
  display.println("Temp: " + String(tempC) + " C");
  display.display();

  // Update Blynk
  Blynk.virtualWrite(TDS_PIN, tdsPPM);
  Blynk.virtualWrite(TEMP_PIN, tempC);



  delay(1000);
}
