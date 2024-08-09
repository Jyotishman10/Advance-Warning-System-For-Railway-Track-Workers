#include <LoRa.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"

#define SS 15
#define RST 16
#define DI0 2

const char *ssid = "Worker";
const char *password = "12345";

#define msg " You have entered in a Working Zone"

AsyncWebServer server(80);

void setup()
{

    Serial.begin(9600);

    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.on("/alert", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", msg); });

    server.begin();

    pinMode(0, OUTPUT);
    while (!Serial)
        ;

    Serial.println("LoRa receiver starting.....");
    LoRa.setPins(SS, RST, DI0);
    SPI.begin();

    while (!LoRa.begin(433E6))
    {
        Serial.println("LoRa initialization failed");

        delay(1000);
    }
    Serial.println("LoRa Initializing OK!");
}

void loop()
{

    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
        while (LoRa.available())
        {
            Serial.println("Received...");

            String LoRaData = LoRa.readString();
            Serial.println(LoRaData);
            if (LoRaData == "1")
            {
                digitalWrite(0, HIGH);
            }
            else
            {
                digitalWrite(0, LOW);
            }
        }
    }
}