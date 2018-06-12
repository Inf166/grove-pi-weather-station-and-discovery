#include "../grovepi.h"

/**
* The wiki suggests the following sensor values:
* 		Min  Typ  Max  Condition
* 		0    0    0    sensor in open air
* 		0    20   300  sensor in dry soil
* 		300  580  700  sensor in humid soil
* 		700  940  950  sensor in water

* 	Sensor values observer:
* 		Val  Condition
* 		0    sensor in open air
* 		18   sensor in dry soil
* 		425  sensor in humid soil
* 		690  sensor in water
**/
int main(void)
{
	int port = 0; //connect sensor to this port
	int value;      //return value of the sensor

	//Exit on failure to start communications with the GrovePi
	if(init()==-1)
		exit(1);

	while(1)
	{
	    //get the value of an analog sensor
		value = analogRead(port);
		printf("Feuchtigkeit: %d\n",value);
		if(value == -1)
			printf("IO Error");
	}
   	return 1;
}
