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

//#include "../grovepi.c" //Muss für Raspberry-Funktionen aktiv sein
//TODO Raspberry Funktionen - Logik der Schleifen (Doppelte Ausgabe) haha

#define INPUT 0
#define OUTPUT 1

// Funktion zum zerteilen von Strings. Weitere Infos unten
int strtoken(char *str, char **token, int size);

// Raspberry-Funktionen anlegegen
float getTemp(int port);
int getSound(int port);
float getLuftfeuchtigkeit(int port);
int getLicht(int port);

int main(){
    puts("Start...\n");
//	init(); //Raspberry init. //Muss für Raspberry-Funktionen aktiv sein

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
    int pid, fileDesc;
    static int counter=0;
    while (1) {
        fileDesc = accept(server_socket, (struct sockaddr *) &client, &client_len);
        puts("Verbindung akzeptiert\n");
        if ((pid = fork()) <0) // Fehlerfall im FORK
        {
            perror("Fork fehlgeschlagen\n");
            close(fileDesc);
            continue;
        }
        else if (pid > 0) //Parentprozess nach dem FORK
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

                //Nachricht bearbeiten
                strtoken(client_cmd,args,2); // teilt die Client-Nachricht in 2 Teile an den Leerstellen WICHTIG!! Das 2. Wort muss mit einem Leerzeichen enden!!
                if (strcmp(args[0], "GET")==0){ // Das erste wort wird geprüft. Ist es ein GET wird auf das 2. geschaut
                  if (strcmp(args[1], "TEMPERATURE") == 0) { // Ist das 2. Wort das gesuchte
                      char message [40]={0};
                      float t=getTemp(8); //Fordere den gesuchten Wert vom Raspberry an (Ausgelagerte Funktionen)
                      sprintf(message,"Temperatur: %.02f C\n",t); //Gibt Wert aus und speichert in message

                      send (fileDesc, message, strlen(message), 0); //schickt message an den Client
                  } else if (strcmp(args[1], "HUMIDITY") == 0) {
                      char message [40]={0};
                      float t=getLuftfeuchtigkeit(8);
                      sprintf(message,"Luftfeuchtigkeit: %.02f Prozent \n",t);

                      send (fileDesc, message, strlen(message), 0);
                  } else if (strcmp(args[1], "LIGHT") == 0) {
                      char message [40]={0};
                      int t=getLicht(4);
                      sprintf(message,"Resistance: %d\n",t);

                      send (fileDesc, message, strlen(message), 0);
                  } else if (strcmp(args[1], "SOUND") == 0) {
                      char message [40]={0};
                      int t = getSound(3);
                      sprintf(message,"Sound: %d\n",t);

                      send (fileDesc, message, strlen(message), 0);
                  } else{ //Ist das 2. Wort unbekannt git es eine Fehlermeldung
                    char message [40]={0};
                    sprintf(message,"Fehlerhafte Eingabe.\n");
                    send (fileDesc, message, strlen(message), 0);
                  }

                } else if (strcmp(args[0], "END") == 0) { //Befehl um die Verbindung zu trennen
                    char message [40]={0};
                    running=0; //Abbruchbedingung der whileschleife erfüllt
                    sprintf(message,"Wird beendet.\n");
                    send (fileDesc, message, strlen(message), 0);
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

// Raspberry-Fubktionen definieren - Übernommen von Team 59
float getTemp(int port)
{
	float temp; // Beispieldaten. Muss =0 wenn Raspberry aktiv
//    pinMode(port, INPUT);  //Muss für Raspberry-Funktionen aktiv sein
//    pi_sleep(1000);  //Muss für Raspberry-Funktionen aktiv sein
//	getTemperature(&temp, port);  //Muss für Raspberry-Funktionen aktiv sein
    return temp;
}

float getLuftfeuchtigkeit(int port)
{
	float humidity; // Beispieldaten. Muss =0 wenn Raspberry aktiv
//    getHumidity(&humidity, port);  //Muss für Raspberry-Funktionen aktiv sein
//	pi_sleep(1000); //wait 1s  //Muss für Raspberry-Funktionen aktiv sein
	return humidity;

}

int getLicht(int port)
{
    int value; // Beispieldaten. Muss =0 wenn Raspberry aktiv
//    float resistance;  //Muss für Raspberry-Funktionen aktiv sein
//    value = analogRead(port);  //Muss für Raspberry-Funktionen aktiv sein
//    resistance = (float)(1023 - value) * 10 / value;  //Muss für Raspberry-Funktionen aktiv sein
//    return resistance;  //Muss für Raspberry-Funktionen aktiv sein
	return value;  //Muss für Raspberry-Funktionen INAKTIV sein
}

int getSound(int port)
{
	int sound; // Beispieldaten. Muss =0 wenn Raspberry aktiv
//	pinMode(port, INPUT);  //Muss für Raspberry-Funktionen aktiv sein
//	pi_sleep(1000);  //Muss für Raspberry-Funktionen aktiv sein
//	int i = 0;  //Muss für Raspberry-Funktionen aktiv sein
//	int summe = 0;  //Muss für Raspberry-Funktionen aktiv sein
//	while(i<5)  //Muss für Raspberry-Funktionen aktiv sein
//	{  //Muss für Raspberry-Funktionen aktiv sein
//        sound = analogRead(port);  //Muss für Raspberry-Funktionen aktiv sein
//        pi_sleep(100);  //Muss für Raspberry-Funktionen aktiv sein
//        summe = summe + sound;  //Muss für Raspberry-Funktionen aktiv sein
//        i++;  //Muss für Raspberry-Funktionen aktiv sein
//	}  //Muss für Raspberry-Funktionen aktiv sein
//	return summe /i;  //Muss für Raspberry-Funktionen aktiv sein
return sound;
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
