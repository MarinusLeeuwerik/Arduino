/* Veelzijdige ultrasoon afstandssensor */
/* Door Marinus Leeuwerik */
/* */
/* */
/* Gebruikt: LCD Keypad Shield , Adafruit 8x8 bi-color LED matrix, HCSR04 Ultrasonic sensor, blauwe led */
/* PINNUMMERS: Arduino -> item*/
/* LCD Keypadshield: A0 -> A0 ; D3 -> D4 ; D5 -> D5 ; D6 -> D6 ; D7 -> D7 ; D8 -> D8 ; D9 -> D9 ; D10 -> D10 ; 5V -> 5V ; GRND -> GRND */
/* Matrix: A4 -> C(CLOCK) ; A5 -> D(DATA) ; GRND -> - ; 3.3V -> + */
/* HCSR04 Sensor: 5V -> Vcc ; GRND -> GRND ; D2 -> Trig ; D4 -> Echo */
/* Blauwe LED: D13 -> Anode(+) ; GRND -> Kathode(-) */

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include <LiquidCrystal.h>

/* Pinnummers, trigger en echo defineren */
const int triggerPin = 2;
const int echoPin = 4;
const int led = 13;

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();
LiquidCrystal lcd(8, 9, 3, 5, 6, 7);

void setup()
{
	// Begin seriële communicatie
	Serial.begin(9600);
	matrix.begin(0x70);
	// Initialiseer lcd scherm
	lcd.begin(16, 2);
	lcd.print("test");
}

/* Bitmap patronen voor matrix vaststellen, 1 is aan, 0 is uit */
static const uint8_t PROGMEM 
	far_bmp[] =
	{
		B00000000,
		B00000000,
		B00000000,
		B00011000,
		B00011000,
		B00000000,
		B00000000,
		B00000000,
	},
	intermediate_bmp[] = 
	{
		B00000000,
		B00000000,
		B00111100,
		B00111100,
		B00111100,
		B00111100,
		B00000000,
		B00000000,
	},
	close_bmp[] =
	{
		B00000000,
		B01111110,
		B01111110,
		B01111110,
		B01111110,
		B01111110,
		B01111110,
		B00000000,
	},
	danger_bmp[] =
	{
		B11111111,
		B11111111,
		B11111111,
		B11111111,
		B11111111,
		B11111111,
		B11111111,
		B11111111,
	};

void loop()
{
	/* Variabelen */
	long echo, dist;

	/* LOW/HIGH betrouwbaarheid */
	pinMode(triggerPin, OUTPUT);
	digitalWrite(triggerPin, LOW);
	delayMicroseconds(2);
	digitalWrite(triggerPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(triggerPin, LOW);

	/* Uitlezen sensor */
	pinMode(echoPin, INPUT);
	echo = pulseIn(echoPin, HIGH);

	/* Omzetten tijd-afstand */
	dist = ttd(echo);
	
	/* Printen naar zowel matrix, lcd scherm & led */
	if (dist <= 200) // Niets doen als de afstand groter is dan 200 centimeter
	{
		Serial.println();
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Afstand: ");
		lcd.print(dist);
		lcd.print(" cm");

		if (dist > 0 && dist <= 5) /* Gevaar */
		{
			matrix.clear();
			matrix.drawBitmap(0, 0, danger_bmp, 8, 8, LED_RED);
			matrix.writeDisplay();
			pinMode(led, OUTPUT);
			digitalWrite(led, HIGH);
		}
		else if (dist > 5 && dist < 15) /* Dichtbij */
		{
			matrix.clear();
			matrix.drawBitmap(0, 0, close_bmp, 8, 8, LED_YELLOW);
			matrix.writeDisplay();
			pinMode(led, OUTPUT);
			digitalWrite(led, LOW);
		}
		else if (dist >= 15 && dist < 35) /* Gematigd */
		{
			matrix.clear();
			matrix.drawBitmap(0, 0, intermediate_bmp, 8, 8, LED_GREEN);
			matrix.writeDisplay();
			pinMode(led, OUTPUT);
			digitalWrite(led, LOW);
		}
		else if (dist >= 35 && dist <= 75) /* Ver */
		{
			matrix.clear();
			matrix.drawBitmap(0, 0, far_bmp, 8, 8, LED_GREEN);
			matrix.writeDisplay();
			pinMode(led, OUTPUT);
			digitalWrite(led, LOW);
		} 
		else
		{
			matrix.clear();
			pinMode(led, OUTPUT);
			digitalWrite(led, LOW);
		}
	}
	
	delay(1000); // Delay voor overzichtelijkheid & verversingstijd van LCD scherm
}

long ttd(long microseconds) //TimeToDistance (ttd)
{
	/* Geluidssnelheid is 340 m/s dus 29 microsecondes/cm */
	return microseconds / 29 / 2;
}

