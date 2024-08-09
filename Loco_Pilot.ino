#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;

const char *ssid = "ESP-7A563E";

LiquidCrystal_I2C lcd(0x27, 20, 4);

const char *api = "http://192.168.4.1/alert";

void setup()
{
    lcd.init();
    lcd.backlight();
    Serial.begin(9600);
    lcd.setCursor(1, 1);

    lcd.print("Journey started");
}

void loop()
{

    WiFi.begin(ssid);

    delay(3000);
    if (WiFiMulti.run() == WL_CONNECTED)
    {
        String str = httpGETRequest(api);

        lcd.clear();
        if (str != "null")
        {
            int delimiterPosition = str.indexOf('d');

            String firstPart = str.substring(0, delimiterPosition);

            String secondPart = str.substring(delimiterPosition + 1);
            lcd.setCursor(0, 0);
            lcd.print(firstPart);
            lcd.setCursor(0, 1);
            lcd.print(secondPart);
        }
        else
        {
            lcd.setCursor(1, 1);
            lcd.print("Safe journey");
        }
    }
    else
    {
        lcd.clear();
        lcd.setCursor(1, 1);
        lcd.print("Safe journey");
    }
}

String httpGETRequest(const char *serverName)
{
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverName);

    int httpResponseCode = http.GET();

    String payload = "null";

    if (httpResponseCode > 0)
    {

        payload = http.getString();
    }

    http.end();

    return payload;
}
