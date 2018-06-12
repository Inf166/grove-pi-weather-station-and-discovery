#include "grovepi.h"
#include <stdio.h>
#include <string.h>

#define INPUT 0
#define OUTPUT 1
int main() {
    char str1[15];

    printf("Bitte Eingabe.\n");
    printf("GET TEMPERATURE\n");
    printf("GET HUMIDITY\n");
    printf("GET LIGHT\n");
    printf("GET SOUND\n");

    while(1){
        printf(">");
        scanf("%s", &str1);
        char temp[15] = "TEMPERATURE";
        char hum[15] = "HUMIDITY";
        char light[15] = "LIGHT";
        char sound[15] = "SOUND";
        printf("Eingabe erkannt\n Eingabe: %s \n", str1);
        if(strcmp(str1, temp)==0){
            printf("Temperatur erkannt\n");
            int port = 8; //Sensor an digital Port D8 anschließen
            float temp = 0, humidity = 0;


            init(); //Initialisierung des GrovePi's
            pinMode(port, INPUT); //Modus festlegen (pinMode([PortNr.],[0(INPUT)/1(OUTPUT)])

            pi_sleep(1000); //wait 1s

            // do indefinitely
            getTemperature(&temp, port);
            printf("[temp = %.02f C]\n", temp);
        } else if(strcmp(str1, hum)==0) {
            printf("Humidity erkannt\n");
            int port = 8; //Sensor an digital Port D8 anschließen
            float temp = 0, humidity = 0;


            init(); //Initialisierung des GrovePi's
            pinMode(port, INPUT); //Modus festlegen (pinMode([PortNr.],[0(INPUT)/1(OUTPUT)])

            pi_sleep(1000); //wait 1s

            // do indefinitely
            //get only Humidity
            getHumidity(&humidity, port);
            printf("[humidity = %.02f%%]\n", humidity);
        }else if (strcmp(str1, light)==0) {
            printf("Light erkannt\n");
            int port = 7; //connect the light sensor to this port
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
        }else if (strcmp(str1, sound)==0) {
            printf("Sound erkannt\n");
            int port = 0; //Audio-Sensor an analogen Port A0 anschließen
            int sound = 0; //Lautstärke

            init(); //Initialisierung des GrovePi's
            pinMode(port, INPUT); //Modus festlegen (pinMode([PinNr.],[0(INPUT)/1(OUTPUT)])

            pi_sleep(1000); //warte 1s

            // do indefinitely
            sound = analogRead(port); //Frage Port A0 ab und speichere Wert in Variable
            printf("SOUND: %d db\n", sound);

            pi_sleep(100); //warte 0,1s
        }
    }
}
