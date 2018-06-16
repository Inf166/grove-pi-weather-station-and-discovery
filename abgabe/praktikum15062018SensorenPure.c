//
// Created by Joel Mai on 23.05.2018.
//
//Include Libaries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>

#include "../grovepi.h" //Muss für Raspberry-Funktionen aktiv sein

#define INPUT 0
#define OUTPUT 1

//Ports predfined
#define LCDPORT 5
#define MOTIONPORT 6
#define COLISIONPORT 7
#define TEMPHUMPORT 8
#define SOUNDPORT 3
#define MOISTUREPORT 2

    void getColision(int pport){
        int port = pport; //connect the sensor to a digital port
        //Exit on failure to start communications with the GrovePi
        if(init()==-1)
            exit(1);
        pinMode(port,INPUT); // set the mode of the sensor

            if(digitalRead(port) == 0){ //collision detected...
                printf("ERSCHÜTTERUNG ERKANNT!");
                fflush(stdout);

            }


    }

    void getTempre(int pport){
        int port = pport; //Sensor an digital Port D8 anschließen
        float temp = 0;
        init(); //Initialisierung des GrovePi's
        pinMode(port, INPUT); //Modus festlegen (pinMode([PortNr.],[0(INPUT)/1(OUTPUT)])
        // do indefinitely

            //get only Temperature
            getTemperature(&temp, port);
            printf("[temp = %.02f C]\n", temp);


    }

    void getFeuchtigkeit(int pport){
        int port = pport; //Sensor an digital Port D8 anschließen
        float humidity = 0;
        init(); //Initialisierung des GrovePi's
        pinMode(port, INPUT); //Modus festlegen (pinMode([PortNr.],[0(INPUT)/1(OUTPUT)])

            //get only Humidity
            getHumidity(&humidity, port);
            printf("[humidity = %.02f%%]\n", humidity);


    }

    void getGerausch(int pport){
        int port = pport; //Audio-Sensor an analogen Port A0 anschließen
        int sound = 0; //Lautstärke
        init(); //Initialisierung des GrovePi's
        pinMode(port, INPUT); //Modus festlegen (pinMode([PinNr.],[0(INPUT)/1(OUTPUT)])


            sound = analogRead(port); //Frage Port A0 ab und speichere Wert in Variable
            printf("SOUND: %d\n", sound);


    }

    void getBewegung(int pport){
        int port = pport; //connect the sensor to a digital port
        int motion = -1;

        //Exit on failure to start communications with the GrovePi
        if(init()==-1)
            exit(1);

        pinMode(port,INPUT); // set the mode of the sensor


            // get the value of the sensor on a digital port
            motion = digitalRead(port);

            if(motion == 255)
                printf("PIR-Motion: IO Error!");

            if(motion == 1){ //motion detected...
                printf("BEWEGUNG ERKANNT!");
                fflush(stdout);

            }
    }

    void getWasserkontakt(int pport){
        int port = pport; //connect sensor to this port
        int value;      //return value of the sensor
        //Exit on failure to start communications with the GrovePi
        if(init()==-1)
            exit(1);

            //get the value of an analog sensor
            value = analogRead(port);
            printf("Feuchtigkeit: %d\n",value);
            if(value == -1)
                printf("IO Error");

    }

    void setLCDText(){
        char text[200];
        float value = 332.0;
        // connect to the i2c-line
        connectLCD();
        // set text and RGB color on the LCD
        setText("Dies ist ein neuer Text");
        setRGB(216, 76, 216);
        //switching throw several colors
        for(int value = 0; value < 256; value++)
        {
            setRGB(value, 255 - value, 0);
        }
        // set final color
        setRGB(0, 255, 0);
        // save the text with a value in char array "text" with the function "sprintf()"
        sprintf(text, "Text mit Wert: %02f", value);
        //show the text
        setText(text);
    }

int main(){
    // Raspberry Sensoren working
    init();
    getColision(COLISIONPORT);
    getTempre(TEMPHUMPORT);
    getFeuchtigkeit(TEMPHUMPORT);
    getGerausch(SOUNDPORT);
    getBewegung(MOTIONPORT);
    getWasserkontakt(MOISTUREPORT);
    setLCDText();
    setText("Du Nutte");
}
