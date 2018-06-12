#include "../grovepi.h"

/**
*   The motion sensor detects every motion in its range.
*   it returns "1" in case of a motion
*
**/

int main(){

    int port = 8; //connect the sensor to a digital port
    int motion = -1;

    //Exit on failure to start communications with the GrovePi
	if(init()==-1)
		exit(1);

    pinMode(port,INPUT); // set the mode of the sensor

	while(1){
        // get the value of the sensor on a digital port
        motion = digitalRead(port);

        if(motion == 255)
            printf("PIR-Motion: IO Error!");

        if(motion == 1){ //motion detected...
            printf("BEWEGUNG ERKANNT!");
            fflush(stdout);
            pi_sleep(200);
        }
        pi_sleep(100);
	}
	return 0;

}
