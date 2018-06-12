#include "../grovepi.h"

#define INPUT 0
#define OUTPUT 1

/**
* Sensor measures temperature and humidity of the air.
*
**/

int main(){

    int port = 8; //Sensor an digital Port D8 anschließen
	float temp = 0, humidity = 0;

    init(); //Initialisierung des GrovePi's
    pinMode(port, INPUT); //Modus festlegen (pinMode([PortNr.],[0(INPUT)/1(OUTPUT)])

    // do indefinitely
	while(1)
	{
        //get Temperature and Humidity values
		getTemperatureAndHumidity(&temp, &humidity, port);
        printf("[temp = %.02f C][humidity = %.02f%%]\n", temp, humidity);

		//get only Temperature
		getTemperature(&temp, port);
        printf("[temp = %.02f C]\n", temp);

		//get only Humidity
		getHumidity(&humidity, port);
        printf("[humidity = %.02f%%]\n", humidity);

		pi_sleep(1000); //wait 1s
		}
}
