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
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>

#include "../../grovepi.h" //Muss für Raspberry-Funktionen aktiv sein
#include "../functions/sensors.c"
#include <time.h>
#include <stdlib.h>


#define INPUT 0
#define OUTPUT 1


int main(){
    // Raspberry Sensoren working
    init();
    connectLCD();
    int color1, color2, color3;
    color1 = 10;
    color2 = 220;
    color3 = 1;
    for (int j = 0; j < 255; ++j) {
        color1 += 1;
        color2 -= 2;
        color3 += 3;
        setRGB(color1, color2, color3);
        pi_sleep(5);
    }

    setText("");

}