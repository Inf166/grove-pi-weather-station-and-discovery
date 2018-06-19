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
#include <time.h>
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

//Danger RGB Codes
#define HIGHPRIO 1
#define MIDDLEPRIO 2
#define LOWPRIO 3
//Initialisierung
    float tempreMax;
    float tempreMin;

    float feuchtigkeitMax;
    float feuchtigkeitMin;

    int gerauschMax;
    int gerauschMin;

    int wasserkontaktMax;
    int wasserkontaktMin;
//Sensoren überall
    int getColision(int pport){
        int port = pport; //connect the sensor to a digital port
        //Exit on failure to start communications with the GrovePi
        if(init()==-1)
            exit(1);
        pinMode(port,INPUT); // set the mode of the sensor

            if(digitalRead(port) == 0){ //collision detected...
                return 1;
            }
    }

    float getTempre(int pport){
        int port = pport; //Sensor an digital Port D8 anschließen
        float temp = 0;
        init(); //Initialisierung des GrovePi's
        pinMode(port, INPUT); //Modus festlegen (pinMode([PortNr.],[0(INPUT)/1(OUTPUT)])
        // do indefinitely

            //get only Temperature
            getTemperature(&temp, port);
            return temp;


    }

    float getFeuchtigkeit(int pport){
        int port = pport; //Sensor an digital Port D8 anschließen
        float humidity = 0;
        init(); //Initialisierung des GrovePi's
        pinMode(port, INPUT); //Modus festlegen (pinMode([PortNr.],[0(INPUT)/1(OUTPUT)])

            //get only Humidity
            getHumidity(&humidity, port);
            return humidity;


    }

    int getGerausch(int pport){
        int port = pport; //Audio-Sensor an analogen Port A0 anschließen
        int sound = 0; //Lautstärke
        init(); //Initialisierung des GrovePi's
        pinMode(port, INPUT); //Modus festlegen (pinMode([PinNr.],[0(INPUT)/1(OUTPUT)])


            sound = analogRead(port); //Frage Port A0 ab und speichere Wert in Variable
            return sound;


    }

    int getBewegung(int pport){
        int port = pport; //connect the sensor to a digital port
        int motion = -1;

        //Exit on failure to start communications with the GrovePi
        if(init()==-1)
            exit(1);

        pinMode(port,INPUT); // set the mode of the sensor


            // get the value of the sensor on a digital port
            motion = digitalRead(port);

            if(motion == 255)
                return 0;

            if(motion == 1){ //motion detected...
                return 1;
            }
    }

    int getWasserkontakt(int pport){
        int port = pport; //connect sensor to this port
        int value;      //return value of the sensor
        //Exit on failure to start communications with the GrovePi
        if(init()==-1)
            exit(1);

            //get the value of an analog sensor
            value = analogRead(port);
            if(value == -1)
                printf("IO Error");
            else return value;
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

    int strtoken(char *str, char **token, int size){ // Funktion zum zerteilen von Strings
        int i = 0;                                     // Teilt diese an der Leerzeichen und speichert sie in den bei "**token" angegebenen Speicherort (Array)
        char separator[] = " ";
        token[0] = strtok(str, separator);
        while(token[i++] && i < size){
            token[i] = strtok(NULL,separator);
        }
        return (i);
    }

    void createServer(){
        //Erstelle Server Socket
        int server_socket;
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket <0) {
            perror("Error: unable to open a socket\n");
        }
        //Definiere Server Adress-Familie
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;           //Adress-Familie
        server_address.sin_port = htons(5678);         //Portnummer
        server_address.sin_addr.s_addr = INADDR_ANY;   //Vlt noch ändern in IP Adresse

        int option = 1;
        setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const void *) &option, sizeof(int)); //int eventuell option // Port Reuse
        //bind den socket zu unser IP und port
        if ((bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address))) <0) {
            perror("Error: unable to bind\n"); // Wenn bind fehlschlägt gibt es eine Fehlermeldug.
        } else{ puts("Bind erfolgreich.\n"); // Sonst Bestätigung
        }
        //listen to conections
        listen(server_socket, 5);
        puts("Warten auf verbindung...\n");
        //Lege Client Socket an
        struct sockaddr_in client;
        socklen_t client_len=sizeof(struct sockaddr_in);
        int pid, fileDesc;
        static int counter=0;
        while (1) {
            fileDesc = accept(server_socket, (struct sockaddr *) &client, &client_len); //TODO soll nicht iwie der vater zuhören? und akzeptieren?
            puts("Verbindung akzeptiert\n");
            if ((pid = fork()) <0) // Fehlerfall im FORK
            {
                perror("Fork fehlgeschlagen\n");
                close(fileDesc);
                continue;
            }
            else if (pid > 0) //Parentprozess nach dem FORK //TODO also hier?
            {
                close(fileDesc);
                counter++;
                printf("Hier läuft der Parentprozess\n");




                char string[256];
                char delimiter[] = " ";
                char *ptr;
                printf("Eingabe bitte");
                scanf("%s", &string);
                // initialisieren und ersten Abschnitt erstellen
                ptr = strtok(string, delimiter);

                while(ptr != NULL) {
                    printf("Abschnitt gefunden: %s\n", ptr);
                    // naechsten Abschnitt erstellen
                    ptr = strtok(NULL, delimiter);
                }
                continue;
            }
            else if (pid == 0) //Childprozess nach dem FORK Hier beginnt die Kommunikation mit dem Client
            {
                char buf[100];
                counter++;
                snprintf(buf, sizeof buf, "Hallo User %d\n", counter);
                send (fileDesc, buf, strlen(buf), 0);
                int running=1;
                while(running) { //running als Abbruchsbedingung für die Kommunikation - Wird running geändert ist die Server-Client-Kommunikation abgeschlossen
                    char client_cmd[256];
                    char *args[3];

                    recv(fileDesc, &client_cmd, sizeof(client_cmd), 0);
                    printf("User %i: %s\n",counter, client_cmd);

                    strtoken(client_cmd,args,3); // teilt die Client-Nachricht in 2 Teile an den Leerstellen WICHTIG!! Das 2. Wort muss mit einem Leerzeichen enden!!

                    if (strcmp(args[0], "PEERS") == 0) {     //Liste aller verbundenen Clients
                        //TODO Tabellen ausgabe
                        setLCDTextmitRGB("Alles cool yo!", LOWPRIO);
                    } else if (strcmp(args[0], "CONNECT") == 0) {     //Verbinde mit client via IP und PORT
                        //TODO IP und Port werden nicht richtig übergeben
                        printf("Port: %s und IP %s\n",args[2], args[1]);
                        snprintf(buf, sizeof buf, "Port: %s und IP %s\n",args[2], args[1]);
                        send (fileDesc, buf, strlen(buf), 0);

                        // Wir machen einen Socket:
                        int client_socket; // ADRESSE ODER PROTOKOLLFAMILE / SOCKET TYP / PROTOKOLL (TCP)
                        client_socket = socket(AF_INET, SOCK_STREAM, 0);
                        printf("Lege neuen Socket für andere Server an\n");
                        //Genauere Adresse fuer den Socket
                        struct sockaddr_in serv_ad;
                        serv_ad.sin_family = AF_INET;           //Adress-Familie
                        int port;
                        sscanf(args[2], "%d", &port);
                        serv_ad.sin_port = htons(port);//Portnummer
                        printf("Portnummer %d\n", port);//Portnummer
                        serv_ad.sin_addr.s_addr = inet_addr(args[1]);   //Vlt noch ändern in IP Adresse
                        printf("IP Adresse %s\n", args[1]);
                        printf("Uebernehme eingegebene IP und Port Nummern\n");
                        //     Typ              Cast zur Adresse              Länge der Adresse
                        int conect_status = connect(client_socket, (struct sockaddr *) &serv_ad, sizeof(serv_ad));
                        //Auffangen von Connection error
                        if(conect_status == -1){
                            printf("Verbindung fehlgeschlagen...\n\n");
                        }
                        if(conect_status == 0){
                            printf("Verbindung hat geklappt...\n\n");
                        }

                        //Empfangen von Daten
                        char server_antwort[256];
                        recv(client_socket, &server_antwort, sizeof(server_antwort), 0);

                        //Ausgabe
                        printf("Dateien empfangen: %s\n", server_antwort);
                    } else if (strcmp(args[0], "E") == 0) {     //Verbinde mit client via IP und PORT
                        snprintf(buf, sizeof buf, "R.I.P. in Piece, User %d\n", counter);
                        send (fileDesc, buf, strlen(buf), 0);
                        running = 0;
                    } else { //Ist das 1. Wort unbekant gibt es einne Fehlerausgabe
                        char message [40]={0};
                        sprintf(message,"Fehlerhafte Eingabe.\n");
                        send (fileDesc, message, strlen(message), 0);

                    }

                }
                close(fileDesc); // wird die whileschleife verlassen wird die Verbindung aufgelöst
                break;
            }
        }
        //close Funktion
        close(server_socket);
    }

    void createConnection(char *ip, int port){
        // Wir machen einen Socket:
        int client_socket; // ADRESSE ODER PROTOKOLLFAMILE / SOCKET TYP / PROTOKOLL (TCP)
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        printf("Lege neuen Socket für andere Server an\n");
        //Genauere Adresse fuer den Socket
        struct sockaddr_in serv_ad;
        serv_ad.sin_family = AF_INET;           //Adress-Familie
        serv_ad.sin_port = htons(port);//Portnummer
        serv_ad.sin_addr.s_addr = inet_addr(ip);   //Vlt noch ändern in IP Adresse
        //     Typ              Cast zur Adresse              Länge der Adresse
        int conect_status = connect(client_socket, (struct sockaddr *) &serv_ad, sizeof(serv_ad));
        //Auffangen von Connection error
        if(conect_status == -1){
            printf("Verbindung fehlgeschlagen...\n\n");
        }
        if(conect_status == 0){
            printf("Verbindung hat geklappt...\n\n");
        }

        //Empfangen von Daten
        char server_antwort[256];
        recv(client_socket, &server_antwort, sizeof(server_antwort), 0);
        writeLog(server_antwort); //schreibt das log
        //Ausgabe
        printf("Dateien empfangen: %s\n", server_antwort);
    }

    void test(){
        while(1){
            setLCDTextmitRGB("Initialisiere...", LOWPRIO);
            pi_sleep(1000);
            setLCDTextmitRGB("Initialisiere..", LOWPRIO);
            pi_sleep(1000);
            setLCDTextmitRGB("Initialisiere.", LOWPRIO);
            pi_sleep(1000);
        }
    }

//es folgen die Funktionen um die Maximal- und Minimalwerte der jeweiligen Sensoren zu ermitteln

     float getTempreMax(float tempre, float tempreMax ) {
    	if (tempreMax == NULL || tempre > tempreMax) {
    		tempreMax = tempre;
  		}
  		return tempreMax;
	  }

	   float getTempreMin(float tempre, float tempreMin) {
  	if (tempreMin == NULL || tempre < tempreMin) {
    		tempreMin = tempre;
		}
		return tempreMin;
	}

    	float getFeuchtigkeitMax(float feuchtigkeit, float feuchtigkeitsMax) {

        	if (feuchtigkeitMax == NULL || feuchtigkeit > feuchtigkeitMax) {
        		feuchtigkeitMax = feuchtigkeit;
    		}
    		return feuchtigkeitMax;
    	}

    	float getFeuchtigkeitMin(float feuchtigkeit, float feuchtigkeitsMin) {


        	if (feuchtigkeitMin == NULL || feuchtigkeit < feuchtigkeitMin) {
        		feuchtigkeitMin = feuchtigkeit;
    		}
    		return feuchtigkeitMin;
    	}

    	int getGerauschMax(int gerausch, int gerauschMax) {

        	if (gerauschMax == NULL || gerausch > gerauschMax) {
        		gerauschMax = gerausch;
    		}
    		return gerauschMax;
    	}

    	int getGerauschMin(int gerausch int gerauschMin) {

        	if (gerauschMin == NULL || gerausch < gerauschMin) {
        		gerauschMin = gerausch;
    		}
    		return gerauschMin;
    	}

    	int getWasserkontaktMax(int wasserkontakt, int wasserkontaktMax) {


        	if (wasserkontaktMax == NULL || wasserkontakt > wasserkontaktMax) {
        		wasserkontaktMax = wasserkontakt;
    		}
    		return wasserkontaktMax;
    	}

    	int getWasserkontaktMin(int wasserkontakt, int wasserkontaktMin) {


        	if (wasserkontaktMin == NULL || wasserkontakt < wasserkontaktMin) {
        		wasserkontaktMin = wasserkontakt;
    		}
    		return wasserkontaktMin;
    	}

//How to write a neat Log 101
      void writeLog(char *ip, char *cmd){


        char ipclient[255] = "255.255.255.255";
    		//char ipclient[255] = ip;
        char sensorname[255];
        char sensorwert[255];
        int size = sizeof(cmd);
        int i = 0;
                                          // Teilt diese an der Leerzeichen und speichert sie in den bei "**token" angegebenen Speicherort (Array)
        char klammerauf[] = "[";
        char klammerzu[] = "]";
        char doppelpoint[] = ":";
        while(i < size){
            if (strcmp(cmd[i], "<") == 0) {
              int run = 1;
              int c = 0;
              while(run){
                ipclient[c] = cmd[i];
                c++;
                i++;
                if(strcmp(cmd[i], ">")){
                  run = 0;
                }
              }
            }
            c = 0;
            if(strcmp(cmd[i], klammerauf)){
              while(cmd[i]!=doppelpoint){
                sensorname[c] = cmd[i];
                i++;
                c++;
              }
              i++;
              c=0;
              while(cmd[i]!=klammerzu){
                sensorwert[c] = cmd[i];
              }
            }
        i++;
        }

          //TODO Check File Ip and Compare with ipclient --> dann Min Max funktion und aktualisierung.

          FILE *fp;

          fp = fopen("Sensordaten.txt", "w");

          if(fp == NULL) {
          	printf(" fp hat keine Datei zugewiesen.\n");
          }else {
          	//<255.255.255.255> [ Sensor: min: xx; derzeitig: xx, max: xx ]

      		fprintf(fp, "<%s> [ COLLISION: derzeitig: %d ]\n", ipclient, getColision(COLISIONPORT) );
      		fprintf(fp, "<%s> [ TEMPERATUR: min: %.2f, derzeitig: %.2f, max: %.2f ]\n", ipclient, getTempreMin(getTempre(TEMPHUMPORT)), getTempre(TEMPHUMPORT), getTempreMax(getTempre(TEMPHUMPORT)) );
      		fprintf(fp, "<%s> [ FEUCHTIGKEIT: min: %.2f, derzeitig: %.2f, max: %.2f ]\n", ipclient, getFeuchtigkeitMin(getFeuchtigkeit(TEMPHUMPORT)), getFeuchtigkeit(TEMPHUMPORT), getFeuchtigkeitMax(getFeuchtigkeit(TEMPHUMPORT)) );
      		fprintf(fp, "<%s> [ GERAEUSCH: min: %d, derzeitig: %d, max: %d ]\n", ipclient, getGerauschMin(getGerausch(SOUNDPORT)), getGerausch(SOUNDPORT), getGerauschMax(getGerausch(SOUNDPORT)) );
      		fprintf(fp, "<%s> [ BEWEGUNG: derzeitig: %d ]\n", ipclient, getBewegung(MOTIONPORT) );
      		fprintf(fp, "<%s> [ WASSER: min: %d, derzeitig: %d, max: %d ]\n", ipclient, getWasserkontaktMin(getWasserkontakt(MOISTUREPORT)), getWasserkontakt(MOISTUREPORT), getWasserkontaktMax(getWasserkontakt(MOISTUREPORT)) );

      	printf("Die Tabelle wurde gespeichert.\n");
      	fclose(fp);


          }
        }
//"Braucht man eine Main-Methode?"
    int main(){
      // Raspberry Sensoren working
        init();
        connectLCD();
        printf("Erwarte Befehl:\n");
        int running = 1;
        while(running){
            char eingabe[256];
            char *args[3];
            scanf("%s", &eingabe);
            strtoken(eingabe,args,3);
            if(strcmp(args[0], "CONNECT") == 0){
                int port;
                sscanf(args[2], "%d", &port);
                createConnection(args[1], port);
            }else if(strcmp(args[0], "GET") == 0){
                if(strcmp(args[1], "SOUND") == 0){
                    char buffer [100];
                    snprintf ( buffer, 100, "[Sound: %d db]\n", getGerausch(SOUNDPORT));
                    puts (buffer);
                }else if(strcmp(args[0], "MOTION") == 0){
                    char buffer [100];
                    if(getGerausch(MOTIONPORT)==1){
                        snprintf ( buffer, 100, "[Motion: DETECTED]\n");
                        puts (buffer);
                    } else {
                        snprintf ( buffer, 100, "[Motion: NONE]\n");
                        puts (buffer);
                    }
                }else if(strcmp(args[0], "COLISION") == 0){
                    char buffer [100];
                    if(getColision(COLISIONPORT)==1){
                        snprintf ( buffer, 100, "[Contact: DETECTED]\n");
                        puts (buffer);
                    } else {
                        snprintf ( buffer, 100, "[Contact: NONE]\n");
                        puts (buffer);
                    }
                }else if(strcmp(args[0], "TEMPERATURE") == 0){
                    char buffer [100];
                    snprintf ( buffer, 100, "[Temperature: %f C]\n", getTempre(TEMPHUMPORT));
                    puts (buffer);
                }else if(strcmp(args[0], "HUMIDITY") == 0){
                    char buffer [100];
                    snprintf ( buffer, 100, "[Humidity: %f %]\n", getFeuchtigkeit(TEMPHUMPORT));
                    puts (buffer);
                }else if(strcmp(args[0], "WATER") == 0){
                    char buffer [100];
                    snprintf ( buffer, 100, "[Water: %d %]\n", getWasserkontakt(MOISTUREPORT));
                    puts (buffer);
                }else if(strcmp(args[0], "FUNKY") == 0){
                    while(1) {
                        srand(time(NULL));   // should only be called once
                        int r = rand();
                        int b = rand();
                        int c = rand();
                        setRGB(r, b, c);
                    }
                }
            }else if(strcmp(args[0], "END") == 0){
                running = 0;
            }
        }


      return 0;
      }
