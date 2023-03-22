#define BLYNK_TEMPLATE_ID "TMPLrOrPkVV1"
#define BLYNK_DEVICE_NAME "Air Quality Monitoring"
#define BLYNK_PRINT Serial

#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"

LiquidCrystal_I2C lcd(0x3F, 16, 2);

  byte degree_symbol[8] = {
  0b00111,
  0b00101,
  0b00111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
  };

#define BLYNK_AUTH_TOKEN "EHKVga-Gc-tMX1OEENX-4KhQBk_-7aE7"

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "**********";
char pass[] = "**********";

BlynkTimer timer;

int gas = A0;
int sensorThreshold = 100;
int buz = D8;
int wled = D6;

#define DHTTYPE DHT11
#define dht_dpin 0

DHT dht(dht_dpin, DHTTYPE);

void sendSensor() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();


  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  int analogSensor = analogRead(gas);

  Blynk.virtualWrite(V2, analogSensor);
  Serial.print("Gas Value: ");
  Serial.println(analogSensor);
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print("    Humidity : ");
  Serial.println(h);
}

void setup() {

  pinMode(buz, OUTPUT);
  pinMode(wled, OUTPUT);

  Serial.begin(9600);

  //pinMode(gas, INPUT);
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  timer.setInterval(30000L, sendSensor);

  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Air Quality");
  lcd.setCursor(3, 1);
  lcd.print("Monitoring");
  delay(2000);
  lcd.clear();
}

void loop() {

  Blynk.run();
  timer.run();

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gasValue = analogRead(gas);

  lcd.setCursor(0, 0);
  lcd.print("Temperature :");
  lcd.setCursor(0, 1);
  lcd.print(t);
  lcd.setCursor(6, 1);
  lcd.write(1);
  lcd.createChar(1, degree_symbol);
  lcd.setCursor(7, 1);
  lcd.print("C");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humidity :");
  lcd.print(h);
  lcd.print("%");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(gasValue);
  lcd.clear();

  if (gasValue < 250) 
  {
    digitalWrite(buz, LOW);
    digitalWrite(wled, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Air Quality: ");
    lcd.print(gasValue);
    lcd.setCursor(0, 1);
    lcd.print("AQ Level Fresh");
    Serial.println("AQ Level Fresh");
    delay(3500);
    lcd.clear();
  } 
   else if (gasValue > 260 && gasValue < 600) 
  {
    digitalWrite(buz, LOW);
    digitalWrite(wled, LOW);
    lcd.setCursor(0, 0);
    lcd.print(gasValue);
    lcd.setCursor(0, 1);
    lcd.print("AQ Level Normal");
    Serial.println("AQ Level Normal");
    delay(3500);
    lcd.clear();
  } 
  else  
  {
    digitalWrite(buz, HIGH);
    digitalWrite(wled, HIGH);
    lcd.setCursor(0, 0);
    lcd.print(gasValue);
    lcd.setCursor(0, 1);
    lcd.print("AQ Level Danger");
    Serial.println("AQ Level Danger");
    delay(3500);
    lcd.clear();
  }
}