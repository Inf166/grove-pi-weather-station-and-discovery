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

#include "../grovepi.h" //Muss für Raspberry-Funktionen aktiv sein
#include <time.h>
#include <stdlib.h>


#define INPUT 0
#define OUTPUT 1


int main(){
    // Raspberry Sensoren working
    init();
    connectLCD();
    while(1) {
        srand(time(NULL));   // should only be called once
        int r = rand();
        int b = rand();
        int c = rand();
        setRGB(r, b, c);
    }
}