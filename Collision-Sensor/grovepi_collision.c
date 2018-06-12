#include "../grovepi.h"

/**
*   The collision sensor detects every collision.
*   it returns "0" in case of a collision
*
**/

int main(){

    int port = 8; //connect the sensor to a digital port

    //Exit on failure to start communications with the GrovePi
	if(init()==-1)
		exit(1);

    pinMode(port,INPUT); // set the mode of the sensor

	while(1){

         if(digitalRead(port) == 0){ //collision detected...
            printf("ERSCHÜTTERUNG ERKANNT!");
            fflush(stdout);
            pi_sleep(200);
        }
        pi_sleep(100);
	}
	return 0;

}
