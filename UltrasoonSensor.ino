/* Pinnummers, trigger en echo */
const int triggerPin = 2;
const int echoPin = 4;
const int led = 13;
/* Twee resterende pinnen zijn ground en VCC */

void setup()
{
	// Begin seriële communicatie
	Serial.begin(9600);
}

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
	if (dist <= 200) // Niets doen als de afstand groter is dan 200 centimeter
	{
		Serial.print(dist);
		Serial.print(" cm");
		Serial.println();
		if (dist < 15) //Als afstand kleiner is dan 15, led aan en print "Pas op!"
		{
			pinMode(led, OUTPUT);
			digitalWrite(led, HIGH);
			Serial.print("Pas op!");
			Serial.println();
		}
		else {
			pinMode(led, OUTPUT); // Groter dan 15, dan led uit.
			digitalWrite(led, LOW);
		}
	}
	delay(900); // Delay voor overzichtelijkheid
}

long ttd(long microseconds) //TimeToDistance (ttd)
{
		/* Geluidssnelheid is 340 m/s dus 29 microsecondes/cm */
		return microseconds / 29 / 2;
}

