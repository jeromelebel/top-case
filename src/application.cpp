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

/* Define the pins used for the SS (SDA) and RST (reset) pins for BOTH hardware and software SPI */
/* Change as required */
#define SS_PIN      A2      // Same pin used as hardware SPI (SS)
#define RST_PIN     D2

/* Define the pins used for the DATA OUT (MOSI), DATA IN (MISO) and CLOCK (SCK) pins for SOFTWARE SPI ONLY */
/* Change as required and may be same as hardware SPI as listed in comments */
#define MOSI_PIN    D3      // hardware SPI: A5
#define MISO_PIN    D4      //     "     " : A4
#define SCK_PIN     D5      //     "     " : A3

/* Create an instance of the RFID library */
#if defined(_USE_SOFT_SPI)
    RFID(int chipSelectPin, int NRSTPD, uint8_t mosiPin, uint8_t misoPin, uint8_t clockPin);    // Software SPI
#else
    RFID RC522(SS_PIN, RST_PIN);    // Hardware SPI
#endif

Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, D3, WS2812B);

uint8_t RFIDCard1[5] = { 0x04, 0xD0, 0xEF, 0xA6, 0x9D };
uint8_t RFIDCard2[5] = { 0x8D, 0x7B, 0x48, 0xB5, 0x0B };
uint8_t RFIDCard3[5] = { 0xC4, 0x1F, 0x1C, 0xCD, 0x0A };
uint8_t RFIDCardBureau[5] = { 0x7E, 0x0F, 0x90, 0x77, 0x96 };
uint8_t RFIDCardMaison[5] = { 0xC0, 0x99, 0xDE, 0xA3, 0x24 };
uint8_t RFIDCardAmesterdam1[5] = { 0x88, 0x04, 0xF4, 0xAC, 0xD4 };
uint8_t RFIDCardAmesterdam2[5] = { 0x88, 0x04, 0xF5, 0xAC, 0xD5 };

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

void setColorStrip(uint32_t color)
{
	uint16_t i;
	
    for(i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, color);
    }
    strip.show();
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
    setColorStrip(0);
    
    pwm.begin();
    pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

bool compareRFID(uint8_t *card1, uint8_t *card2)
{
	return memcmp(card1, card2, 5) == 0;
}

uint8_t currentRFIDCard[5];
uint8_t blankRFIDCard[5] = { 0, 0, 0, 0, 0};
unsigned long cardDetectedTime = 0;

void rfid_loop()
{
	/* Temporary loop counter */
	uint8_t i;
	
	/* Has a card been detected? */
	if (RC522.isCard() && RC522.readCardSerial()) {
	    /* If so then get its serial number */

		if (compareRFID(currentRFIDCard, RC522.serNum)) {
	    } else if (compareRFID(RFIDCard1, RC522.serNum)) {
			Serial.println("carte 1");
			 setColorStrip(0x0000FF);
	    } else if (compareRFID(RFIDCard2, RC522.serNum)) {
			Serial.println("carte 2");
			setColorStrip(0x00FF00);
	    } else if (compareRFID(RFIDCard3, RC522.serNum)) {
			Serial.println("carte 3");
			setColorStrip(0xFF0000);
	    } else if (compareRFID(RFIDCardBureau, RC522.serNum)) {
			Serial.println("Bureau");
			setColorStrip(0xFFFF00);
	    } else if (compareRFID(RFIDCardMaison, RC522.serNum)) {
			Serial.println("Maison");
			setColorStrip(0xFF00FF);
	    } else if (compareRFID(RFIDCardAmesterdam1, RC522.serNum)) {
			Serial.println("Amesterdam 1");
			setColorStrip(0x0080FF);
	    } else if (compareRFID(RFIDCardAmesterdam2, RC522.serNum)) {
			Serial.println("Amesterdam 2");
			setColorStrip(0x8000FF);
	    } else {
			setColorStrip(0xFFFFFF);
			for(i = 0; i <= 4; i++) {
				if (RC522.serNum[i] < 0x10) {
					Serial.print("0");
		    	}
				Serial.print(RC522.serNum[i],HEX);
				if (i < 4) {
					Serial.print(" ");
				} else {
					Serial.println("");
				}
			}
		}
		cardDetectedTime = millis();
		memcpy(currentRFIDCard, RC522.serNum, 5);
	} else if (!compareRFID(currentRFIDCard, blankRFIDCard) && millis() - cardDetectedTime > 100) {
		setColorStrip(0x000000);
		memcpy(currentRFIDCard, blankRFIDCard, 5);
	}
}

void loop()
{
    for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
        pwm.setPWM(15, 0, pulselen);
        pwm.setPWM(12, 0, pulselen);
    }
    delay(500);
    for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
        pwm.setPWM(15, 0, pulselen);
        pwm.setPWM(12, 0, pulselen);
    }
    delay(500);
    Serial.println("ok");
}
