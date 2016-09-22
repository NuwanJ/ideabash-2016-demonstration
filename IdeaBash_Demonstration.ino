/*
 * IdeaBash 2016 Demonstration : Arduino Source Code
 * 
 * Author      : Nuwan Jaliyagoda
 * Create on   : 01 September, 2016
 * Last update : 22 September, 2016
 * 
 * Licensed with  GNU GENERAL PUBLIC LICENSE (Version 3, 29 June 2007)
 * Source code available at https://github.com/NuwanJ/ideabash-2016-demonstration
 * 
 * Checked with Arduino 1.6.9 and SIM 900 GPRS Shield
 * 
 * Connections 
 * 
 * MODULE   ARDUINO 
 * 
 * TX pin --> 7
 * RX pin --> 8
 * GND -----> GND
 * 
 */

// including Software Serial and Wire libraries
#include <SoftwareSerial.h>
#include <Wire.h>

// including clasess for SIM900 GSM Module. Please note that, this isn't a library; only FILES
#include "GPRS_Shield_Arduino.h"
#include "sim900.h"

// Change PIN_TX, PIN_RX as defined on your GSM Module/ Shield (Arduino using Software Serial for communicating with GSM Module
#define PIN_TX    7
#define PIN_RX    8

// Baud rate for communicate with GSM Module/ Shield
#define BAUDRATE  19200

// Maximum length of the message
#define MESSAGE_LENGTH 160

// Variables used by GPRS Library
char message[MESSAGE_LENGTH];
int messageIndex = 0;
char phone[16];
char datetime[24];

// define Arduino PINs that you connected Relays
int relayPin = 12;


GSM gprsTest(PIN_TX,PIN_RX,BAUDRATE);

void setup() {
  Serial.begin(9600);

  pinMode(relayPin, OUTPUT);

  while (!gprsTest.init()) {
    Serial.print("Init Error.\r\n");
    delay(1000);
  }
  delay(3000);
  Serial.println("Init Success, please send SMS message to me!");
}

void loop() {

  messageIndex = gprsTest.isSMSunread();

  if (messageIndex > 0) {

    // Read the message
    gprsTest.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);

    // Delete the SMS after reading. That may help you to save inbox memory.
    gprsTest.deleteSMS(messageIndex);

    Serial.print("From number: ");
    Serial.println(phone);
    Serial.print("Datetime: ");
    Serial.println(datetime);
    Serial.print("Message: ");
    Serial.println(message);

    processMessage(message);
  }

  // for debugging purpose. you can use this code block and Serial Monitor for send commands insted of SMS.
  /*
    if (Serial.available() > 0) {
      String message = Serial.readString();
      Serial.println(message);
      processMessage(message);
    }*/
}


void processMessage(String message) {

  // message == "on" -> Relay=ON
  // message == "off"-> Relay=OFF

  if (message == "on") {
    digitalWrite(relayPin, HIGH);

  } else if (message == "off") {
    digitalWrite(relayPin, LOW);
  }
}


