#include "../grovepi.h"

int main()
{
	int port = 4; //Button an digital Port D4 anschließen


		init(); //Initialisierung des GrovePi's
		pinMode(port, INPUT); //Modus festlegen (pinMode([PinNr.],[0(INPUT)/1(OUTPUT)])

		pi_sleep(1000); //warte 1s

		// do indefinitely
		while(1)
		{	//wenn Button gedrückt worden ist...
			if(digitalRead(port) ==  1){
				printf("[pin %d][Button ON]\n", port);
				pi_sleep(1000); //warte 1s
			}
			else{
				printf("[pin %d][Button OFF]\n", port);
				pi_sleep(1000); //warte 1s
			}
		}


	return 0;
}
