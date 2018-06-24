
/*
  Klausuraufgabe
    In einem Peer to Peer Netzwerk befinden sich 10 Computer.
    Sie wollen sich gegenseitig Anfragen für ihre Sensor Werte schicken.
    Also verbinden sich die Computer via IP und PORT.
    Nach erfolgreichem Abschluss einer Verbindung, beginnen die Computer
    mit einem Loop. Einer der Beiden fragt spezifisch nach einem Wert.
    Der Andere antwortet mit <MeineIP> [Sensor: Wert].
    Jetzt will der Computer in einem anderen Prozess diese Werte nicht nur speichern,
    sondern auch in einem Chararray ausgeben.
    Dabei wollen beide Prozesse auf 
	ein und das selbe Struct zugreifen.
	
	
	/*"Dabei soll der Server intern verwalten, wann sich welcher Client verbunden hat."
	impliziert doch irgendwie das wird n timecode ben�tigen und so ne art log, nur halt nicht in
	einer externen datei sondern am besten n satruct mit meheren strings, oder n char-array-array sozusagen, zus�tzlich n char-array-array f�r alle
	aktuell verbundenen ohne timecode.  

    //Attribut
    struct client meineClients[10]; //inklusive mir
    //Structs for Clients
    struct sensorwerte {
       float minTemp;
       float akTemp;
       float maxTemp;

       int colision;

       float minHum;
       float akHum;
       float maxHum;

       float minDB;
       float akDB;
       float maxDB;

       int motion;

       int minWater;
       int akWater;
       int maxWater;
    };
    struct client {
       char ipdesclient[255];
       struct sensorwerte sensorwerte;
    };

    Wie lösen Sie das Problem mit Semaphoren und Shared Memory?
*/
/*
  Antwort:
    Der Speicher des Char-Arrays muss mit beiden Prozessen geshared werden.
    Und eine Semaphore gibt an ob einer der Beiden Zugriff genehmigt bekommt.
    Es müssen auch nebenbei Strings verglichen und dann sortiert eingegeben werden.

    Dafür bietet es sich an diese Strings in Structs zu setzen, jeder Client hat demnach
    einen Struct mit sämtlichen Sensorwerten. Es wird dann ein Array angelegt, indem sich
    dann die Sensordaten-Structs aller Clients befinden. Dieser Array wird dann in einem
    Shared Memory Segment gespeichert, sodass alle darauf zugreifen können. Alles spezifische
    regelt Joel dann.

*/
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

    //Structs for Clients
    struct sensorwerte {
       float minTemp;
       float akTemp;
       float maxTemp;

       int colision;

       float minHum;
       float akHum;
       float maxHum;

       float minDB;
       float akDB;
       float maxDB;

       int motion;

       int minWater;
       int akWater;
       int maxWater;
    };
    struct client {
       char ipdesclient[255];
       int isNotEmpty;
       struct sensorwerte meineSensorwerte;
    };

    //Sensoren InItIaLiZe
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
            token[i] = strtok(NULL,separator);//TODO NULL durch str ersetzt
        }
        return (i);
    }

    int main(){
    // Raspberry Sensoren working
    init();
    connectLCD();
        setLCDTextmitRGB("Tadaaa",LOWPRIO);
    //Servershit incoming
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

        // Port Re-Use
        int option = 1;
        setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const void *) &option, sizeof(int));

        //bind den socket zu unser IP und port
        if ((bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address))) <0) {
            perror("Error: unable to bind\n"); // Wenn bind fehlschlägt gibt es eine Fehlermeldug.
        } else{ puts("Bind erfolgreich.\n"); // Sonst Bestätigung
        }

        //LEGE LISTE VON CONNECTED-CLIENTS AN
        struct client connectedClients[35];
//        snprintf(connectedClients[0].ipdesclient[225] , sizeof(connectedClients[0].ipdesclient[225]),inet_ntoa(server_address.sin_addr));
//        connectedClients[0].isNotEmpty = 1;

        //listen to conections
        listen(server_socket, 5);
        puts("Warten auf verbindung...\n");

        //Lege Client Socket an
        struct sockaddr_in client_addr;
        socklen_t client_len=sizeof(struct sockaddr_in);


    //Lets Split into Childs and Parent
    int pid2, fileDesc2;
    static int counter2=0;
        if ((pid2 = fork()) <0) // Fehlerfall im FORK
        {
            perror("Fork fehlgeschlagen\n");
            close(fileDesc2);
//            continue;
        }
        else if (pid2 > 0) //Parentprozess nach dem FORK //TODO also hier?
        {
            close(fileDesc2);
            counter2++;
            printf("Hier läuft der Parentprozess\n");
            char admin_cmd[256];
            char *args2[3];
            printf("Bitte Befehl eingeben: \n");
            scanf("%s", &admin_cmd);
            printf("Debug: %d\n",strtoken(admin_cmd,args2,3));
            printf("Befehl: %s IP: %s Port: %s\n", args2[0], args2[1], args2[2]);
                    if (strcmp(args2[0], "PEERS") == 0) {     //Liste aller verbundenen Clients
                        //TODO Tabellen ausgabe
                        int i = 0;
                        while(connectedClients[i].isNotEmpty!=0){
                            printf("Client-IP: %s\n",connectedClients[i].ipdesclient);

                            printf("Min-DB: %d\n",connectedClients[i].meineSensorwerte.minDB);
                            printf("Ak-DB: %d\n",connectedClients[i].meineSensorwerte.akDB);
                            printf("Max-DB: %d\n",connectedClients[i].meineSensorwerte.maxDB);

                            printf("Min-Water: %d\n",connectedClients[i].meineSensorwerte.minWater);
                            printf("Ak-Water: %d\n",connectedClients[i].meineSensorwerte.akWater);
                            printf("Max-Water: %d\n",connectedClients[i].meineSensorwerte.maxWater);

                            printf("Min-Temp: %\n",connectedClients[i].meineSensorwerte.minTemp);
                            printf("Ak-Temp: %\n",connectedClients[i].meineSensorwerte.akTemp);
                            printf("Max-Temp: %\n",connectedClients[i].meineSensorwerte.maxTemp);

                            printf("Min-Hum: %\n",connectedClients[i].meineSensorwerte.minHum);
                            printf("Ak-Hum: %\n",connectedClients[i].meineSensorwerte.akHum);
                            printf("Max-Hum: %\n",connectedClients[i].meineSensorwerte.maxHum);

                            printf("Colision: %\n",connectedClients[i].meineSensorwerte.colision);

                            printf("Motion: %\n\n",connectedClients[i].meineSensorwerte.motion);
                            i++;
                        }
                        setLCDTextmitRGB("Alles cool yo!", LOWPRIO);
                    } else if (strcmp(args2[0], "CONNECT") == 0) {     //Verbinde mit client via IP und PORT
                        //Wir machen einen Socket:
                        int client_socket; // ADRESSE ODER PROTOKOLLFAMILE / SOCKET TYP / PROTOKOLL (TCP)
                        client_socket = socket(AF_INET, SOCK_STREAM, 0);
                        printf("Lege neuen Socket für andere Server an\n");
                        //Genauere Adresse fuer den Socket
                        struct sockaddr_in serv_ad;
                        serv_ad.sin_family = AF_INET;           //Adress-Familie
                        serv_ad.sin_port = htons((int) args2[2]);         //Portnummer
                        printf("%d\n", (int) args2[2]);
                        printf("%s\n", args2[1]);
                        printf("%s\n", inet_addr(args2[1]));
//                        serv_ad.sin_addr.s_addr = args2[1];
                        serv_ad.sin_addr.s_addr = inet_addr(args2[1]);
//                        serv_ad.sin_addr.s_addr = inet_ntoa(inet_addr(args2[1]));   //Vlt noch ändern in IP Adresse
                        printf("Uebernehme eingegebene IP und Port Nummern\n");
                        //     Typ              Cast zur Adresse              Länge der Adresse
                        int conect_status = connect(client_socket, (struct sockaddr *) &serv_ad, sizeof(serv_ad));
                        //Auffangen von Connection error
                        if(conect_status == -1){
                            printf("Verbindung fehlgeschlagen...\n\n");
                        }
                        //Empfangen von Daten
                        char server_antwort[256];
                        recv(client_socket, &server_antwort, sizeof(server_antwort), 0);

                        //Ausgabe
                        printf("Dateien empfangen: %s\n", server_antwort);
                        //Verbinde mit client via IP und PORT
                    }
//            continue;
        }
        else if (pid2 == 0) //Childprozess nach dem FORK Hier beginnt die Kommunikation mit dem Client
        {
            //Lets Split into Childs and Parent
            int pid, fileDesc;
            static int counter=0;
            while (1) {
                fileDesc = accept(server_socket, (struct sockaddr *) &client_addr, &client_len);
                printf("IP address is: %s\n", inet_ntoa(client_addr.sin_addr));

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
                    char buf[100];
                    printf("Hier läuft der Parentprozess\n");
                    while(1){
                    snprintf(buf, sizeof buf, "<192.168.2.27>\n [TEMP.: %f]\n", getTempre(TEMPHUMPORT));
                    send (fileDesc, buf, strlen(buf), 0);
                    setLCDTextmitRGB(buf, LOWPRIO);
                        pi_sleep(2000);
                    snprintf(buf, sizeof buf, "<192.168.2.27>\n [HUMIDITY: %f]\n", getTempre(TEMPHUMPORT));
                    send (fileDesc, buf, strlen(buf), 0);
                    setLCDTextmitRGB(buf, LOWPRIO);
                        pi_sleep(2000);
                    char* response;
                    if(getBewegung(MOTIONPORT)==1){ response = "DETECTED";setLCDTextmitRGB("DANGER\n MOTION", HIGHPRIO);}
                    else {response = "NONE";setLCDTextmitRGB("<192.168.2.27>\n [Nothing\n here]\n", LOWPRIO);}
                    snprintf(buf, sizeof buf, "<192.168.2.27>\n [MOTION: %s]\n", response);
                    send (fileDesc, buf, strlen(buf), 0);
                        pi_sleep(2000);
                    snprintf(buf, sizeof buf, "<192.168.2.27>\n [SOUND: %d]\n", getGerausch(SOUNDPORT));
                    send (fileDesc, buf, strlen(buf), 0);
                        pi_sleep(2000);
                    if(getColision(COLISIONPORT)==1){ response = "DETECTED";setLCDTextmitRGB("<192.168.2.27>\n [DANGER COLISION]\n", HIGHPRIO);}
                    else {response = "NONE";setLCDTextmitRGB("<192.168.2.27>\n [Nothing here]\n", LOWPRIO);}
                    snprintf(buf, sizeof buf, "<192.168.2.27>\n [COLISION: %s]\n", response);
                    send (fileDesc, buf, strlen(buf), 0);
                    setLCDTextmitRGB(buf, LOWPRIO);
                        pi_sleep(2000);
                    snprintf(buf, sizeof buf, "<192.168.2.27>\n [MOISTURE: %d]\n", getWasserkontakt(MOISTUREPORT));
                    send (fileDesc, buf, strlen(buf), 0);
                    setLCDTextmitRGB(buf, LOWPRIO);
                        pi_sleep(2000);
                    }
<<<<<<< HEAD
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
                        char *args[2];

                        recv(fileDesc, &client_cmd, sizeof(client_cmd), 0);
                        printf("User: %s\n", client_cmd);

                        strtoken(client_cmd, args, 2); // teilt die Client-Nachricht in 2 Teile an den Leerstellen WICHTIG!! Das 2. Wort muss mit einem Leerzeichen enden!!

                        if(strcmp(args[0], "GET") == 0){
                            if(strcmp(args[1], "TEMPERATURE") == 0){
                                snprintf(buf, sizeof buf, "<192.168.2.27> [TEMPERATURE: %f]\n", getTempre(TEMPHUMPORT));
                                send (fileDesc, buf, strlen(buf), 0);
                                setLCDTextmitRGB(buf, LOWPRIO);
                            } else if(strcmp(args[1], "HUMIDITY") == 0){
                                snprintf(buf, sizeof buf, "<192.168.2.27> [HUMIDITY: %f]\n", getTempre(TEMPHUMPORT));
                                send (fileDesc, buf, strlen(buf), 0);
                                setLCDTextmitRGB(buf, LOWPRIO);
                            }else if(strcmp(args[1], "MOTION") == 0){
                                char* response;
                                if(getBewegung(MOTIONPORT)==1){ response = "DETECTED";setLCDTextmitRGB("DANGER MOTION", HIGHPRIO);}
                                else {response = "NONE";}
                                snprintf(buf, sizeof buf, "<192.168.2.27> [MOTION: %s]\n", response);
                                send (fileDesc, buf, strlen(buf), 0);
                            }else if(strcmp(args[1], "SOUND") == 0){
                                snprintf(buf, sizeof buf, "<192.168.2.27> [SOUND: %d]\n", getGerausch(SOUNDPORT));
                                send (fileDesc, buf, strlen(buf), 0);
                                setLCDTextmitRGB(buf, LOWPRIO);
                            }else if(strcmp(args[1], "COLISION") == 0){
                                char* response;
                                if(getColision(COLISIONPORT)==1){ response = "DETECTED";setLCDTextmitRGB("DANGER COLISION", HIGHPRIO);}
                                else {response = "NONE";}
                                snprintf(buf, sizeof buf, "<192.168.2.27> [COLISION: %f]\n", response);
                                send (fileDesc, buf, strlen(buf), 0);
                            }else if(strcmp(args[1], "MOISTURE") == 0){
                                snprintf(buf, sizeof buf, "<192.168.2.27> [MOISTURE: %d]\n", getWasserkontakt(MOISTUREPORT));
                                send (fileDesc, buf, strlen(buf), 0);
                                setLCDTextmitRGB(buf, LOWPRIO);
                            }
                        } else if (strcmp(args[0], "E") == 0) {
                            snprintf(buf, sizeof buf, "R.I.P. in Piece, User %d\n", counter);
                            send (fileDesc, buf, strlen(buf), 0);
                            running = 0;
                        //Ist das 1. Wort unbekant gibt es einne Fehlerausgabe
                        } else {
                            char message [40]={0};
                            sprintf(message,"Fehlerhafte Eingabe.\n");
                            send (fileDesc, message, strlen(message), 0);
                        }
                    }
                    close(fileDesc); // wird die whileschleife verlassen wird die Verbindung aufgelöst
                    break;
=======
                    //Empfangen von Daten
                    char server_antwort[256];
                    recv(client_socket, &server_antwort, sizeof(server_antwort), 0);

                    //Ausgabe
                    printf("Dateien empfangen: %s\n", server_antwort);
                //Verbinde mit client via IP und PORT
                	--
                } else if (strcmp(args[0], "E") == 0) {
                    snprintf(buf, sizeof buf, "R.I.P. in Piece, User %d\n", counter);
                    send (fileDesc, buf, strlen(buf), 0);
                    running = 0;
                //Ist das 1. Wort unbekannt gibt es eine Fehlerausgabe
                } else {
                    char message [40]={0};
                    sprintf(message,"Fehlerhafte Eingabe.\n");
                    send (fileDesc, message, strlen(message), 0);
>>>>>>> 534c75913463a18764528337661d8576e693b838
                }
            }
            close(fileDesc2); // wird die whileschleife verlassen wird die Verbindung aufgelöst
//            break;
        }
    //close Funktion
    close(server_socket);
    return 0;
}
