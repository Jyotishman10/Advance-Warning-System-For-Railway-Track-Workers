#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>

#define ss 5
#define rst 14
#define dio0 2

#define echo 22
#define trig 21

void setup()
{
    Serial.begin(4800);
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);

    while (!Serial)
        ;
    Serial.println("LoRa Transmitter starting");

    LoRa.setPins(ss, rst, dio0);

    while (!LoRa.begin(433E6))
    {
        Serial.println("Failed to initialize LoRa module");
        delay(1000);
    }
    Serial.println("LoRa module initialized successfully");
}

void loop()
{
    digitalWrite(trig, LOW);
    delayMicroseconds(2);

    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    long duration = pulseIn(echo, HIGH);
    float distance = duration * 0.034 / 2;

    Serial.println(distance);

    if (distance < 40)
    {

        LoRa.beginPacket();
        LoRa.print("1");
        LoRa.endPacket();
    }
    else
    {
        LoRa.beginPacket();
        LoRa.print("0");
        LoRa.endPacket();
    }

    delay(1000);
}
