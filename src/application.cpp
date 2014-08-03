/* FILE:    RC522_RFID_Module_Example
   DATE:    23/03/14
   VERSION: 0.2 Spark

REVISIONS:

23/03/14    Version 0.2, modified by Paul Kourany to run on Spark Core
            with added support for Software SPI
24/07/13    Created version 0.1


This is an example of how to use the RC522 RFID module. The module allows reading
and writing to various types of RFID devices and can be found in our MFRC-522 
(HCMODU0016) and Ultimate RFID (HCARDU0068) kits. This example Arduino sketch uses
the RFID library written by Miguel Balboa to read the pre-programmed serial number 
from RFID cards and tags supplied with our RFID kits. Snapshots and links to the 
library are available on our support forum.


PINOUT:

RC522 MODULE    SPARK HARD SPI  SPARK SOFT SPI
SS                  A2              ANY
SCK                 A3              ANY
MOSI                A5              ANY
MISO                A4              ANY
IRQ                 N/A             N/A
GND                 GND             GND
RST                 D9              ANY
3.3V                3.3V            3.3V


You may copy, alter and reuse this code in any way you like, but please leave
reference to HobbyComponents.com in your comments if you redistribute this code.
This software may not be used directly for the purpose of selling products that
directly compete with Hobby Components Ltd's own range of products.

THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR LACK OF NEGLIGENCE.
HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR ANY DAMAGES,
INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY
REASON WHATSOEVER.
*/


/* Include the RFID library */
/* SEE RFID.h for selecting Hardware or Software SPI modes */
#include "RFID.h"
#include "Adafruit_NeoPixel.h"
#include "Adafruit_PWMServoDriver.h"

#define POWER_SERVO_PIN     D4

/* Define the pins used for the SS (SDA) and RST (reset) pins for BOTH hardware and software SPI */
/* Change as required */
#define SS_PIN      A2      // Same pin used as hardware SPI (SS)
#define RST_PIN     D2

RFID RC522(SS_PIN, RST_PIN);    // Hardware SPI

uint8_t RFIDCardAmesterdam1[5] = { 0x88, 0x04, 0xF4, 0xAC, 0xD4 };
uint8_t RFIDCardAmesterdam2[5] = { 0x88, 0x04, 0xF5, 0xAC, 0xD5 };

Adafruit_PWMServoDriver pwm;

#define ATTACH_SERVO_ID     15
int attachServoOpen = 300;
int attachServoClose = 600;
char latestCard[64] = { 0 };

int closeAttach(String args);
int openAttach(String args);

uint8_t currentServo = 15;

int moveservo(String args)
{
    Serial.print("set value ");
    Serial.print(args.toInt());
    Serial.print(" for servo ");
    Serial.println(currentServo);
    pwm.setPWM(currentServo, 0, args.toInt());
    return 0;
}

int setservo(String args)
{
    Serial.print("set servo ");
    Serial.println(args.toInt());
    currentServo = args.toInt();
    return 0;
}

int releaseservo(String args)
{
    Serial.print("set servo ");
    Serial.println(args.toInt());
    pwm.setPWM(currentServo, 4096, 0);
    return 0;
}

int setfrequency(String args)
{
    Serial.print("set servo ");
    Serial.println(args.toInt());
    pwm.setPWMFreq(args.toInt());
    return 0;
}

int playFrequency(String args)
{
    tone(A6, args.toInt(), 500);
    return 0;
}

void setup()
{ 
    Serial.begin(115200);
    
    /* Enable the SPI interface */
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV8);
    SPI.begin();
    
    /* Initialise the RFID reader */
    RC522.init();
    Serial.println("bonjour");
    
    pinMode(POWER_SERVO_PIN, OUTPUT);
    
    pwm.begin();
    pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
    
    Spark.function("play", playFrequency);
    Spark.function("setservo", setservo);
    Spark.function("moveservo", moveservo);
    Spark.function("releaseservo", releaseservo);
    Spark.function("setfrequency", setfrequency);
    Spark.function("openattach", openAttach);
    Spark.function("closeattach", closeAttach);
    Spark.variable("latestcard", latestCard, STRING);
}

bool compareRFID(uint8_t *card1, uint8_t *card2)
{
	return memcmp(card1, card2, 5) == 0;
}

uint8_t currentRFIDCard[5];
uint8_t blankRFIDCard[5] = { 0, 0, 0, 0, 0};
unsigned long cardDetectedTime = 0;

int openAttach(String args)
{
    tone(A6, 213, 500);
    digitalWrite(POWER_SERVO_PIN, HIGH);
    pwm.setPWM(ATTACH_SERVO_ID, 0, attachServoOpen);
    delay(2000);
    pwm.setPWM(ATTACH_SERVO_ID, 4096, 0);
//    digitalWrite(POWER_SERVO_PIN, LOW);
    return 0;
}

int closeAttach(String args)
{
    tone(A6, 440, 500);
    digitalWrite(POWER_SERVO_PIN, HIGH);
    pwm.setPWM(ATTACH_SERVO_ID, 0, attachServoClose);
    delay(2000);
    pwm.setPWM(ATTACH_SERVO_ID, 4096, 0);
//    digitalWrite(POWER_SERVO_PIN, LOW);
    return 0;
}

void rfid_loop()
{
	/* Has a card been detected? */
	if (RC522.isCard() && RC522.readCardSerial()) {
	    /* If so then get its serial number */

		if (compareRFID(currentRFIDCard, RC522.serNum)) {
	    } else if (compareRFID(RFIDCardAmesterdam1, RC522.serNum)) {
			Serial.println("Amesterdam 1");
			openAttach("");
	    } else if (compareRFID(RFIDCardAmesterdam2, RC522.serNum)) {
			Serial.println("Amesterdam 2");
			closeAttach("");
	    } else {
            tone(A6, 880, 500);
	    }
	    sprintf(latestCard, "%02X:%02X:%02X:%02X:%02X", RC522.serNum[0], RC522.serNum[1], RC522.serNum[2], RC522.serNum[3], RC522.serNum[4]);
		cardDetectedTime = millis();
		memcpy(currentRFIDCard, RC522.serNum, 5);
	} else if (!compareRFID(currentRFIDCard, blankRFIDCard) && millis() - cardDetectedTime > 100) {
		memcpy(currentRFIDCard, blankRFIDCard, 5);
	}
}

void loop()
{
    rfid_loop();
}
