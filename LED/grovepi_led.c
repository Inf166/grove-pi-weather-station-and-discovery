#include "../grovepi.h"

int main()
{
	int led = 4; // Grove LED is connected to digital port D4 on the GrovePi


		init(); // initialize communication w/ GrovePi
		pinMode(led, 1); // set the LED pin as OUTPUT on the GrovePi
		pi_sleep(1000); // wait 1 second

		printf("This example will blink a Grove LED connected to the GrovePi+ on the port labeled D4.\n");
		printf("If you're having trouble seeing the LED blink, be sure to check the LED connection and the port number.\n");
		printf("You may also try reversing the direction of the LED on the sensor.\n\n");
		printf("Connect the LED to the port label D4!\n");


		// do indefinitely
		while(1)
		{
			// 1 second the LED is HIGH -> ON
			digitalWrite(led, 1);
			printf("[pin %d][LED ON]\n", led);
			pi_sleep(1000);

			// and another second LED is LOW -> OFF
			digitalWrite(led, 0);
			printf("[pin %d][LED OFF]\n", led);
			pi_sleep(1000);
		}


	return 0;
}
