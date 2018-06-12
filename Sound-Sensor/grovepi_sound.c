/**
* Dieses Beispiel sollte für alle Sensoren gelten, die an einen analogen Port angeschlossen
* werden müssen. Für den Ultraschall-Sensor gibt es zusätzlich eine entsprechende C-Funktion
* "ultrasonicRead(digital Port)". Digitaler Port, da der US schon einen integrierten
* AD-Wandler besitzt.
*/

#include "../grovepi.h"

#define INPUT 0
#define OUTPUT 1

int main()
{
	int port = 0; //Audio-Sensor an analogen Port A0 anschließen
	int sound = 0; //Lautstärke

	init(); //Initialisierung des GrovePi's
	pinMode(port, INPUT); //Modus festlegen (pinMode([PinNr.],[0(INPUT)/1(OUTPUT)])

	pi_sleep(1000); //warte 1s

	// do indefinitely
	while(1)
	{
		sound = analogRead(port); //Frage Port A0 ab und speichere Wert in Variable
		printf("SOUND: %d\n", sound);

		pi_sleep(100); //warte 0,1s

	}
	return 0;
}
