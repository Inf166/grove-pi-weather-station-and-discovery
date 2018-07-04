 #include <stdio.h>
 #include "../../grovepi.h"
 #include <sys/socket.h>
 
  int getColision(int pport){
    int port = pport;
    //if(init()==-1)
    //    exit(1);
    pinMode(port,INPUT);
    if(digitalRead(port) == 0){
        return 1;
    }
    return 0;
    }

    float getTempre(int pport){
        int port = pport;
        float temp = 0;
        //init();
        pinMode(port, INPUT);
        getTemperature(&temp, port);
        return temp;
    }

    float getFeuchtigkeit(int pport){//Sensor an digital Port D8 anschließen
        float humidity = 0;
        //init();
        int port = pport;
        pinMode(port, INPUT);
        getHumidity(&humidity, port);
        return humidity;
    }

    int getGerausch(int pport){
        int port = pport;
        int sound = 0;
        //init();
        pinMode(port, INPUT);
        sound = analogRead(port);
        return sound;
    }

    int getBewegung(int pport){
        int port = pport;
        int motion = -1;
        //if(init()==-1)
        //    exit(1);
        pinMode(port,INPUT);
            motion = digitalRead(port);
            if(motion == 255)
                return 0;
            if(motion == 1){ //motion detected...
                return 1;
            }
        return 0;
    }

    int getWasserkontakt(int pport){
        int port = pport;
        int value;
        //if(init()==-1)
        //    exit(1);
        value = analogRead(port);
        //if(value == -1)
        //    printf("IO Error");
        //else
            return value;
    }
     void setLCDTextmitRGB(const char *str, int a){
         switch(a) {
             case 1: setRGB(255, 0, 0);break;
             case 2: setRGB(255, 255, 0);break;
             case 3: setRGB(0, 255, 0);break;
             default: setRGB(255, 255, 255);break;
         }
         setText(str);
     }

     int strtoken(char *str, char **token, int size){
         int i = 0;
         char separator[] = " ";
         token[0] = strtok(str, separator);
         while(token[i++] && i < size){
             token[i] = strtok(NULL,separator);
         }
         return (i);
     }
 int strtokenmitsepjoe(char *str, char *separator, char **token, int size){
     int i = 0;
     token[0] = strtok(str, separator);
     while(token[i++] && i < size){
         token[i] = strtok(NULL,separator);
     }
     return (i);
 }
