#include "../grovepi.h"

#define INPUT 0
#define OUTPUT 1

/**
* PWM = Puls-Weiten-Modulation
* Es können mehr als nur die Werte "0" und "1" übertragen werden,
* sodass unterschiedliche Helligkeiten dargestellt werden können (im Fall einer LED)
**/

int main(){
	//D5 ist ein digitaler Port der PWM Signale verarbeiten kann
	int led = 5; //LED an digital Port D5 anschließen
	float brightness = 0; //Helligkeit der LED

	init(); //Initialisierung des GrovePi's
	pinMode(led, OUTPUT); //Modus festlegen (pinMode([PinNr.],[0(INPUT)/1(OUTPUT)])

	pi_sleep(1000); //warte 1s

	// do indefinitely
	while(1){

		if(brightness > 255) //Wenn der Helligkeitswert den Wert erreicht, wieder von vorne anfangen
			brightness = 0;

		analogWrite(led, brightness); //PWM-Signale über analogWrite() steuern
		float percentage_brightness = 100 * (float)brightness / 255; //Berechnung für Ausgabe

		printf("[pin %d][led brightness = %.2f%%]\n", led, percentage_brightness);

		brightness += 5; //Erhöhung des Helligkeitswertes
		pi_sleep(500); //warte 0,5s
	}
return 0;
}
