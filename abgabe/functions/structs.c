//
// Created by Joel Mai on 04.07.2018.
//
#include <stdio.h>
struct message {
    long mstype;        //msg type - must be >0
    char content[256];
};
//Struct to create Log for Sensors
struct sensorwerte {
    float minTemp;
    float akTemp;
    float maxTemp;

    int colision;

    float minHum;
    float akHum;
    float maxHum;

    int minDB;
    int akDB;
    int maxDB;

    int motion;

    int minWater;
    int akWater;
    int maxWater;
};
struct client {
    char ipdesclient[255];
    int isNotEmpty;
    char connectionEstablished[255];
    struct sensorwerte meineSensorwerte;
};
struct clientarray{
    struct client connectedClients[35];
};