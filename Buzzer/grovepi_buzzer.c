#include "../grovepi.h"

int main(void)
{
	//Exit on failure to start communications with the GrovePi
	if(init()==-1)
		exit(1);

	//Set pin mode to output on digital port D4
	pinMode(4, 1);
	while(1)
	{
		digitalWrite(4, 1);
		pi_sleep(500);
		digitalWrite(4, 0);
		pi_sleep(500);
	}
   	return 1;
}
