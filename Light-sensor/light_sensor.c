#include "../grovepi.h"

/**
*   Sensor detect the light intensity with an intern light resistor and gives a specific value back.
*
*
**/

int main(){
    int port = 0; //connect the light sensor to this port
    int led = 4; //connect a LED to this port
    int value;
    float resistance;
    //threshhold to pass
    int threshhold = 10;

    //Exit on failure to start communications with the GrovePi
	if(init()==-1)
		exit(1);

    pinMode(port,INPUT);
    pinMode(led, OUTPUT);

	while(1){
        //read from the analog Port
        value = analogRead(port);
        //calculate the resistance of the light sensor
        resistance = (float)(1023 - value) * 10 / value;
        //if the resitance passes threshhold, turn on the LED
        if(resistance > threshhold)
            digitalWrite(led, 1);
        else
            digitalWrite(led, 0);

            printf("Value: %d  Resistance: %0.2f \n", value, resistance);

            pi_sleep(100); //wait 0,1s
	}
	return 0;

}
