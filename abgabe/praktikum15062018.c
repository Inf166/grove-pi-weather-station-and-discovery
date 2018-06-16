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
//TODO Raspberry Funktionen - Logik der Schleifen (Doppelte Ausgabe) haha

#define INPUT 0
#define OUTPUT 1

//Ports predfined
#define LCDPORT 5
#define MOTIONPORT 6
#define COLISIONPORT 7
#define TEMPHUMPORT 8
#define SOUNDPORT 3
#define MOISTUREPORT 2




// Funktion zum zerteilen von Strings. Weitere Infos unten
int strtoken(char *str, char **token, int size);

// Raspberry-Funktionen anlegegen
float getTemp(int port);
int getSound(int port);
float getLuftfeuchtigkeit(int port);
int getLicht(int port);
// Raspberry-Fubktionen definieren - Übernommen von Team 59
float getTemp(int port)
{
    float temp;             // Beispieldaten. Muss =0 wenn Raspberry aktiv
    pinMode(port, INPUT);           //Muss für Raspberry-Funktionen aktiv sein
    pi_sleep(1000);                 //Muss für Raspberry-Funktionen aktiv sein
    getTemperature(&temp, port);    //Muss für Raspberry-Funktionen aktiv sein
    return temp;
}

float getLuftfeuchtigkeit(int port)
{
    float humidity;             // Beispieldaten. Muss =0 wenn Raspberry aktiv
    getHumidity(&humidity, port);       //Muss für Raspberry-Funktionen aktiv sein
    pi_sleep(1000); //wait 1s           //Muss für Raspberry-Funktionen aktiv sein
    return humidity;

}

int getLicht(int port)
{
    int value;                                    // Beispieldaten. Muss =0 wenn Raspberry aktiv
    float resistance;                                   //Muss für Raspberry-Funktionen aktiv sein
    value = analogRead(port);                           //Muss für Raspberry-Funktionen aktiv sein
    resistance = (float)(1023 - value) * 10 / value;    //Muss für Raspberry-Funktionen aktiv sein
    return resistance;                                  //Muss für Raspberry-Funktionen aktiv sein
//	return value;                                   //Muss für Raspberry-Funktionen INAKTIV sein
}

int getSound(int port)
{
    int sound;          // Beispieldaten. Muss =0 wenn Raspberry aktiv
    pinMode(port, INPUT);   //Muss für Raspberry-Funktionen aktiv sein
    pi_sleep(1000);         //Muss für Raspberry-Funktionen aktiv sein
    int i = 0;              //Muss für Raspberry-Funktionen aktiv sein
    int summe;          //Muss für Raspberry-Funktionen aktiv sein
    while(i<5)              //Muss für Raspberry-Funktionen aktiv sein
    {                       //Muss für Raspberry-Funktionen aktiv sein
        sound = analogRead(port);   //Muss für Raspberry-Funktionen aktiv sein
        pi_sleep(100);              //Muss für Raspberry-Funktionen aktiv sein
        summe = summe + sound;      //Muss für Raspberry-Funktionen aktiv sein
        i++;                        //Muss für Raspberry-Funktionen aktiv sein
    }                               //Muss für Raspberry-Funktionen aktiv sein
    return summe /i;                //Muss für Raspberry-Funktionen aktiv sein
//return sound;
}

int main(){
    puts("Start...\n");
	init(); //Raspberry init. //Muss für Raspberry-Funktionen aktiv sein

    //Erstelle Server Socket
        int server_socket;
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket <0) {
            perror("Error: unable to open a socket\n");
        }
        puts("Socket wurde erstellt.\n");

    //Definiere Server Adress-Familie
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;           //Adress-Familie
        server_address.sin_port = htons(5678);         //Portnummer
        server_address.sin_addr.s_addr = INADDR_ANY;   //Vlt noch ändern in IP Adresse

        int option = 1;
        setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const void *) &option, sizeof(int)); //int eventuell option

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
        puts("Warte immernoch...\n");
        printf("neue testausgabe");
//TODO TEST für sensoren und chararray append
//    char sensor_werte2[256]="Sensorwerte von 192.168.2.28\n";
//    strncat(sensor_werte2,"Temperatur: %f c \n",  getTemp(8));
//    strncat(sensor_werte2,"Lautstaerke: %i db \n", getSound(7));
//    strncat(sensor_werte2,"Luftfeuchtigkeit: %f \n",  getLuftfeuchtigkeit(8));
//    strncat(sensor_werte2,"Licht: %i \n",  getLicht(5));
//    printf(sensor_werte2);
//    printf("%s",sensor_werte2);
//    printf("%d",getTemp(8));
//    printf("%d",getSound(7));
//    printf("%d",getLuftfeuchtigkeit(8));
//    printf("%d",getLicht(5));

    connectLCD();
    setText("Test");






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
            continue;
        }
        else if (pid == 0) //Childprozess nach dem FORK Hier beginnt die Kommunikation mit dem Client
        {
            char buf[100];
            counter++;
            printf("Hier läuft der Childprozess\nGib einen Befehl ein:\n");
            snprintf(buf, sizeof buf, "Hallo Nutzer %d\n", counter);
            send (fileDesc, buf, strlen(buf), 0);
          	int running=1;
            while(running) { //running als Abbruchsbedingung für die Kommunikation - Wird running geändert ist die Server-Client-Kommunikation abgeschlossen
                char client_cmd[256];
                char *args[2];

                recv(fileDesc, &client_cmd, sizeof(client_cmd), 0);
                printf("Client: %s\n", client_cmd);

                char sensor_werte[256]="Sensorwerte von 192.168.2.28\n";
                strncat(sensor_werte,"Temperatur: %f c \n",  getTemp(TEMPHUMPORT));
                strncat(sensor_werte,"Lautstaerke: %i db \n", getSound(SOUNDPORT));
                strncat(sensor_werte,"Luftfeuchtigkeit: %f \n",  getLuftfeuchtigkeit(MOISTUREPORT));
                strncat(sensor_werte,"Licht: %i \n",  getLicht(5));
                printf(sensor_werte);
                //Nachricht bearbeiten
                strtoken(client_cmd,args,3); // teilt die Client-Nachricht in 2 Teile an den Leerstellen WICHTIG!! Das 2. Wort muss mit einem Leerzeichen enden!!
                if (strcmp(args[0], "PEERS") == 0) {     //Liste aller verbundenen Clients
                    //TODO Tabellen ausgabe
                } else if (strcmp(args[0], "CONNECT") == 0) {     //Verbinde mit client via IP und PORT
                    //TODO Verbinde mit Client
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
    return 0;
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
