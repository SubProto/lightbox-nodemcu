#include <ArduinoOTA.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

float star[6];

const char* ssid = ".......";
const char* password = "........";
#define PIN 5
#define BRIGHTNESS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(106, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("lightbox");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, 0xffffff);
    strip.show();
  }
  for(uint16_t i=100; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, 0xff00ff);
    strip.show();
  }
  star[0] = 10;
  star[1] = 70;
  star[2] = 130;
  star[3] = 190;
  star[4] = 250;
  star[5] = 310;
}

uint16_t intensity;
float inc;

void loop() {
  ArduinoOTA.handle();
  for(uint16_t i=0; i<6; i++) {
    intensity = (int)(sin(star[i] / 108) * 10) + 20;
    strip.setPixelColor(100+i, intensity | intensity << 8 | intensity << 16);
    strip.show();
    inc = (float) random(1,30);
    star[i] += inc;
    if (star[i] >= 360) 
    {
        star[i] = 0;
    }
  }
  delay(30);
}
