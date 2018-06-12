#include "../grovepi.h"
#include <math.h>

#define INPUT 0
#define OUTPUT 1

/**
* Potentiometer (Drehwiderstand) mit dem verschiedene Werte innerhalb von 300° angenommen werden können.
* kann zum Dimmen von Lichtquellen, Steuern von Anzeigen und Übertragen auf Schrittmotoren dienen.
**/

int main(){
	int led = 5; //LED an digital Port D5 anschließen
	int potentiometer = 0; //POT an analogen Port A0 anschließen
	float brightness = 0; //Helligkeit der LED

	int adc_ref = 5; //Referenz-Spannung des ADCs ist 5V
	int grove_vcc = 5; //Vcc des GrovePis ist für gewöhnlich 5V
	int full_angle = 300; //bis zu 300° umdrehung sind möglich

	int sensor_value = 0; //aktueller Wert des Sensors
	float voltage = 0; //aktueller Spannungswert
	float degrees = 0; //aktueller Gradzahl des Sensors


	init(); //Initialisierung des GrovePi's
	pinMode(led, OUTPUT); //Modus festlegen (pinMode([PinNr.],[0(INPUT)/1(OUTPUT)])
	pinMode(potentiometer, INPUT);

	pi_sleep(1000); //warte 1s

	// do indefinitely
	while(1){
        //get the value of the analog sensor
		sensor_value = analogRead(potentiometer);
        //calculate the voltage of the actual state
		voltage = (sensor_value * adc_ref / 1023);
        //calculate the degrees of the actual state
		degrees = (voltage * full_angle) / grove_vcc;
        // calculate the brightness which the LED shall show
		brightness = (int)(degrees / full_angle * 255); //Berechnung für Ausgabe
        // writes the value of "brightness" to the analog port
		analogWrite(led, brightness); //PWM-Signale über analogWrite() steuern

		printf("[Sensorwert %d][Grad = %.2f] [Helligkeit = %.2f]\n", sensor_value,degrees, brightness);

		pi_sleep(500); //warte 0,5s
	}
return 0;
}
