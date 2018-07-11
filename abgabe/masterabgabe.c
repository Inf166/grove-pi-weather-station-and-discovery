//
// Created by Joel Mai & Robert Daniel Tipping on 11.07.2018.
//
#include "../grovepi.h"
#include "./functions/sensors.c"
#include "./functions/structs.c"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
/* Define Stuff */
#define INPUT 0
#define OUTPUT 1
#define MOISTUREPORT 6
#define SOUNDPORT 7
#define LCDPORT 5
#define MOTIONPORT 3
#define COLISIONPORT 5
#define TEMPHUMPORT 2
#define HIGHPRIO 1
#define MIDDLEPRIO 2
#define LOWPRIO 3
#define BUFSIZE 1024
/* Global Variablen */
int nextFreeEntryPlace = 0;
int  fileDesc;
/* Process - Zombies - kill */
void sig_handler(int signo)
{
    close(fileDesc);
    raise (SIGKILL);
}
/* Wahlteil Discovery */ //TODO WAHLTEIL DISCOVERY
static void broadcast(const char *nachricht)
{
    struct sockaddr_in bc;
//     memset(&bc, '\0', sizeof(struct sockaddr_in));
    bc.sin_family = AF_INET;
    bc.sin_port = htons(4567);
    bc.sin_addr.s_addr = inet_addr("192.168.2.255");

    int bc_socket;
    bc_socket = socket(AF_INET, SOCK_DGRAM, 0);
    int broadcastPermission = 1;
    if (setsockopt(bc_socket, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission,
                   sizeof(broadcastPermission)) < 0)
        printf("setsockopt() failed\n");


    printf("Lade Freunde via Broadcast ein\n\n");
    if(sendto(bc_socket, nachricht, strlen(nachricht), 0, (struct sockaddr *)&bc, sizeof(struct sockaddr_in)) < 0)
        perror("sendto");
    printf("Nachricht versandt.\n\n\n");
}

/* Main */
int main() {
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGUSR1\n");
// Sensors n Stuff
    init();
    connectLCD();
// Server Socket
    // Erstelle Server Socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error: unable to open a socket\n");
        setLCDTextmitRGB("FATAL ERROR :x", HIGHPRIO);
    }
    // Definiere Server Adress-Familie
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;            //Adress-Familie
    server_address.sin_port = htons(5678);          //Portnummer
    server_address.sin_addr.s_addr = INADDR_ANY;    //Vlt noch aendern in IP Adresse
    // Port Re-Use
    int option = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const void *) &option, sizeof(int));
    // bind den socket zu unser IP und port
    if ((bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address))) < 0) {
        perror("Error: unable to bind\n");          // Wenn bind fehlschlaegt gibt es eine Fehlermeldug.
        setLCDTextmitRGB("FATAL ERROR :(", HIGHPRIO);
    } else {
        puts("Bind erfolgreich.\n");            // Sonst Bestaetigung
        // READY FOR CLIENTS
        setLCDTextmitRGB("I BIM READY :)", MIDDLEPRIO);
        pi_sleep(1000);
    }
    // listen to conections
    listen(server_socket, 5);
    puts("Warten auf verbindung...\n");
    // Lege Client Socket an
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(struct sockaddr_in);
//InItIaLiZe ThE MeSsAgEqUeUe
    key_t key;
    int feeder;
    key = ftok("something", 65);
    feeder = msgget(key, IPC_CREAT);
    //Putin shared MEMORY
    key_t shkey = ftok("sollthing", 65);
    int shid = shmget(shkey, sizeof(feeder), IPC_CREAT | 0777);
    feeder = (int) shmat(shid, NULL, 0); //kein plan
    //InItIaLiZe ThE lCdMeSsAgEqUeUe
    key_t key2;
    int lcdfeeder;
    key2 = ftok("mssomething", 65);
    lcdfeeder = msgget(key2, IPC_CREAT);
    //Putin shared LCDMEMORY
    key_t mskey = ftok("msoslllol", 65);
    int shit = shmget(mskey, sizeof(lcdfeeder), IPC_CREAT | 0777);
    lcdfeeder = (int) shmat(shit, NULL, 0);//kein plan

//Shared Memory
    // LEGE LISTE VON CONNECTED-CLIENTS AN
    struct clientarray meineclients;
    struct clientarray *p_meineclients;
    p_meineclients = &meineclients;
    //THIS SHOULD MAKE THE FIRST ENTRY TO OUR STRUCT... but it doesn't...
    printf("My IP address is: %s\n", inet_ntoa(server_address.sin_addr));
    snprintf(meineclients.connectedClients[0].ipdesclient, sizeof(meineclients.connectedClients[0].ipdesclient),
             inet_ntoa(server_address.sin_addr));
    meineclients.connectedClients[0].isNotEmpty = 1;
    time_t now;
    now = time(0);
    snprintf(meineclients.connectedClients[0].connectionEstablished, sizeof(meineclients.connectedClients[0].connectionEstablished),ctime(&now));
    nextFreeEntryPlace++;
    key_t logkey = ftok("logmichan", 65);
    int shat = shmget(logkey, sizeof(meineclients.connectedClients), IPC_CREAT | 0777);
    p_meineclients = shmat(shat, (void *)0, 0);//kein plan
    printf("Memory shared since: %s\n", meineclients.connectedClients[0].connectionEstablished);
    //CREATE SEMAPHORE
    int semID;
    struct sembuf sema;
    semID = semget(2404, 1, IPC_CREAT | 0666);
    int counter = 0;
//TODO WAHLTEILDISCOVERY
    char nachricht[256];
    snprintf(nachricht, sizeof(nachricht), "192.168.2.28 5678");
    broadcast(nachricht);

    meineclients.connectedClients[0].meineSensorwerte.akWater = getWasserkontakt(MOISTUREPORT);
    meineclients.connectedClients[0].meineSensorwerte.motion = getBewegung(MOTIONPORT);
    meineclients.connectedClients[0].meineSensorwerte.colision = getColision(COLISIONPORT);
    meineclients.connectedClients[0].meineSensorwerte.akDB = getGerausch(SOUNDPORT);
    meineclients.connectedClients[0].meineSensorwerte.akTemp = getTempre(TEMPHUMPORT);
    meineclients.connectedClients[0].meineSensorwerte.akHum = getFeuchtigkeit(TEMPHUMPORT);

// Forks and Stuff
    int pid1;
    if((pid1 = fork()) < 0)//ERROR
    {
        perror("Error: Fork Error - please restart the Programm\n");// Throw Error
    }
    if(pid1 > 0)//PARENT PROCESS - Mainflow Server
    {
        int pid10;
        if((pid10 = fork()) < 0)//ERROR
        {
            perror("Error: Fork Error - please restart the Programm\n");// Throw Error
        }
        if(pid10 == 0)//child LCD
        {
            //LCD AUSGABE
            //NOTIFY MAJOR EVENTS
            setLCDTextmitRGB("LCD is ready :)", LOWPRIO);
            pi_sleep(100);
            while (1) {
                if (semID >= 0) {
                    /* Bereite die Semaphore vor und starte */
                    sema.sem_num = 0;
                    sema.sem_flg = SEM_UNDO;
                    sema.sem_op = -1;
                    if (-1 == semop(semID, &sema, 1)) {
                        /* Fehler */
                        perror("semop");
                    }
                    /* Code ab hier */
                    int tag;
                    struct message nachricht;
                    while (tag > 0) {
                        tag = msgrcv(feeder, &nachricht, sizeof(&nachricht),
                                     (long) HIGHPRIO, 0);
                        if (tag < 0) {
                            perror(strerror(errno));
                            printf("msgrcv failed, rc=%d\n", tag);
                            exit(1);
                        }
                        printf("%s\n", nachricht.content);
                        setLCDTextmitRGB(nachricht.content, HIGHPRIO);
                        pi_sleep(1000);
                    }
                    if (tag > 0) {
                        tag = msgrcv(feeder, &nachricht, sizeof(&nachricht),
                                     (long) MIDDLEPRIO, 0);
                        if (tag < 0) {
                            perror(strerror(errno));
                            printf("msgrcv failed, rc=%d\n", tag);
                            exit(1);
                        }
                        printf("%s\n", nachricht.content);
                        setLCDTextmitRGB(nachricht.content, MIDDLEPRIO);
                        pi_sleep(1000);
                    }
                    if (tag > 0) {
                        tag = msgrcv(feeder, &nachricht, sizeof(&nachricht),
                                     (long) LOWPRIO, 0);
                        if (tag < 0) {
                            perror(strerror(errno));
                            printf("msgrcv failed, rc=%d\n", tag);
                            exit(1);
                        }
                        printf("%s\n", nachricht.content);
                        setLCDTextmitRGB(nachricht.content, LOWPRIO);
                        pi_sleep(1000);
                    }
                    /* Code bis hier */
                    sema.sem_op = 1;
                    if (-1 == semop(semID, &sema, 1)) {
                        /* Fehler */
                        perror("semop");
                    }

                } else {
                    perror("semget");
                }
            }
        }
        if(pid10 > 0)//PARENT PROCESS - Mainflow Server
        {
            int pid2;
            if((pid2 = fork()) < 0)//ERROR
            {
                perror("Error: Fork Error - please restart the Programm\n");// Throw Error
            }
            if(pid2 > 0)//PARENT PROCESS - Mainflow Server
            {
                int pid4, fileDesc;
                while (1) {
                    // Verbindung wird Serverseitig angenommen
                    fileDesc = accept(server_socket, (struct sockaddr *) &client_addr, &client_len);
                    // Gebe Erfolg auf Konsole aus
                    puts("Verbindung akzeptiert\n");
                    // Zeige verbundene IP-Adresse an - for DEBUG
                    printf("IP address is: %s\n", inet_ntoa(client_addr.sin_addr));
                    if((pid4 = fork()) < 0)//ERROR
                    {
                        perror("Error: Fork Error - please restart the Programm\n");// Throw Error
                        close(fileDesc);
                        continue;
                    }
                    if(pid4 > 0)//PARENT PROCESS - Mainflow Server
                    {
                        close(fileDesc);
                        counter++;
                        continue;
                    }
                    if(pid4 == 0)//CHILD PROCESS - Typ1
                    {
                        counter++;
                        int pid5;
                        if((pid5 = fork()) < 0)//ERROR
                        {
                            perror("Error: Fork Error - please restart the Programm\n");// Throw Error
                        }
                        if(pid5 > 0)//PARENT PROCESS - Typ1 Mainflow
                        {
                            // Create a Buffer-Var
                            char buf[100];
                            // SEND Welcome
                            snprintf(buf, sizeof buf, "Hallo User %d\n", counter);
                            send(fileDesc, buf, strlen(buf), 0);
                            int pid6;
                            if((pid6 = fork()) < 0)//ERROR
                            {
                                perror("Error: Fork Error - please restart the Programm\n");// Throw Error
                            }
                            if(pid6 > 0)//PARENT PROCESS - Check if still there
                            {

                            }
                            if(pid6 == 0) {
                                // Abbruchvariable: running als Abbruchsbedingung fuer die Kommunikation - Wird running geaendert ist die Server-Client-Kommunikation abgeschlossen
                                int running = 1;
                                while (running) {
                                    // RESERVIERE VARIABLEN
                                    char client_cmd[256];
                                    char *args[1024];
                                    // RECEIVE NICE VIBES
                                    recv(fileDesc, &client_cmd, sizeof(client_cmd), 0);
                                    printf("User: %s\n", client_cmd);
                                    // teilt die Client-Nachricht in 2 Teile an den Leerstellen WICHTIG!! Das 2. Wort muss mit einem Leerzeichen enden!!
                                    strtoken(client_cmd, args, 2);
                                    // CHECK SENSORS FROM CONNECTED CLIENT
                                    if (strcmp(args[0], "GET") == 0) {
                                        if (strcmp(args[1], "TEMPERATURE") == 0) {
                                            snprintf(buf, sizeof buf, "Temperatur: %f\n", getTempre(TEMPHUMPORT));
                                            send(fileDesc, buf, strlen(buf), 0);
                                        } else if (strcmp(args[1], "HUMIDITYURE") == 0) {
                                            snprintf(buf, sizeof buf, "Humidity: %f\n", getTempre(TEMPHUMPORT));
                                            send(fileDesc, buf, strlen(buf), 0);
                                        } else if (strcmp(args[1], "MOTIONONURE") == 0) {
                                            char *response;
                                            if (getBewegung(MOTIONPORT) == 1) { response = "DETECTED"; }
                                            else { response = "NONE"; }
                                            snprintf(buf, sizeof buf, "Motiondetected: %s\n", response);
                                            send(fileDesc, buf, strlen(buf), 0);
                                        } else if (strcmp(args[1], "SOUNDNONURE") == 0) {
                                            snprintf(buf, sizeof buf, "Sound: %d\n", getGerausch(SOUNDPORT));
                                            send(fileDesc, buf, strlen(buf), 0);
                                        } else if (strcmp(args[1], "COLISIONURE") == 0) {
                                            char *response;
                                            if (getColision(COLISIONPORT) == 1) { response = "DETECTED"; }
                                            else { response = "NONE"; }
                                            snprintf(buf, sizeof buf, "Colisiondetected: %s\n", response);
                                            send(fileDesc, buf, strlen(buf), 0);
                                        } else if (strcmp(args[1], "MOISTUREURE") == 0) {
                                            snprintf(buf, sizeof buf, "Moisture %d\n", getWasserkontakt(MOISTUREPORT));
                                            send(fileDesc, buf, strlen(buf), 0);
                                        }
                                            //TODO WAHLTEIL Überwachung
                                        else if(strcmp(args[0], "ECHO") == 0) {
                                            snprintf(buf, sizeof buf, "REPLY \n");
                                            send(fileDesc, buf, strlen(buf), 0);
                                        }
                                        // PRESS E AND SEND TO BREAK WHILE
                                    } else if (strcmp(args[0], "E") == 0) {
                                        snprintf(buf, sizeof buf, "BYE BYE, User %d\n", counter);
                                        send(fileDesc, buf, strlen(buf), 0);
                                        running = 0;
                                        // IF THE CHECKER-WHILE BREAKS - THROW END OF INNER WHILE
                                        close(fileDesc);
                                        printf("VERBINDUNG GESCHLOSSEN");
                                        break;
                                        // THROW ERROR IF FIRST WORD IS UNKNOWN
                                    } else {
                                        char errorlol[40] = {0};
                                        sprintf(errorlol, "Fehlerhafte Eingabe.\n");
                                        send(fileDesc, errorlol, strlen(errorlol), 0);
                                    }
                                }
                            }
                        }
                        if(pid5 == 0)//CHILD PROCESS - Sideflow Typ1
                        {
                            //NOTIFY MAJOR EVENTS
                            while (1) {
                                if (semID >= 0) {
                                    /* Bereite die Semaphore vor und starte */
                                    sema.sem_num = 0;
                                    sema.sem_flg = SEM_UNDO;
                                    sema.sem_op = -1;
                                    if (-1 == semop(semID, &sema, 1)) {
                                        /* Fehler */
                                        perror("semop");
                                    }
                                    /* Code ab hier */
                                    int tag;
                                    struct message nachricht;
                                    while (tag > 0) {

                                        tag = msgrcv(feeder, &nachricht, sizeof(&nachricht),
                                                     sizeof(meineclients.connectedClients[counter]), 0);
                                        if (tag < 0) {
                                            perror(strerror(errno));
                                            printf("msgrcv failed, rc=%d\n", tag);
                                            exit(1);
                                        }
                                        printf("%s\n", nachricht.content);
                                        send(fileDesc, nachricht.content, strlen(nachricht.content), 0);
                                    }
                                    /* Code bis hier */
                                    sema.sem_op = 1;
                                    if (-1 == semop(semID, &sema, 1)) {
                                        /* Fehler */
                                        perror("semop");
                                    }

                                } else {
                                    perror("semget");
                                }
                            }
                        }
                    }
                }
            }
            if(pid2 == 0)//CHILD PROCESS - Client Modus Enabled
            {
                int cougar;
                    while (1) {
                        char eingabe[256];
                        char *args[1024];
                        int pid8;
                        if((pid8 = fork()) < 0)//ERROR
                        {
                            perror("Error: Fork Error - please restart the Programm\n");// Throw Error
                        }
                        if(pid8 > 0)//PARENT PROCESS - Mainflow Server
                        {
                        printf("<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>\n");
                        printf("<   Mögliche Eingaben:          > \n");
                        printf("<   CONNECT xxx.xxx.x.xx xxxx   >\n");
                        printf("<   PEERS                       >\n");
                        printf("<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>\n\n\n");
                        scanf("%s", &eingabe);
                        strtoken(eingabe, args, 3);
                        if (strcmp(args[0], "CONNECT") == 0) {
                            // Wir machen einen Socket:
                            int client_socket; // ADRESSE ODER PROTOKOLLFAMILE / SOCKET TYP / PROTOKOLL (TCP)
                            client_socket = socket(AF_INET, SOCK_STREAM, 0);
                            printf("Lege neuen Socket für andere Server an\n");
                            int gefunden = 0;
                            int i = 0;
                            while (meineclients.connectedClients[i].isNotEmpty) {
                                if (meineclients.connectedClients[i].ipdesclient == args[1]) {
                                    gefunden = 1;
                                    break;
                                }
                                i++;
                            }
                            if (gefunden == 0) {
                                //Genauere Adresse fuer den Socket
                                struct sockaddr_in serv_ad;
                                serv_ad.sin_family = AF_INET;           //Adress-Familie
                                serv_ad.sin_port = htons(atoi(args[2]));//Portnummer
                                char *buffer = args[1];
                                serv_ad.sin_addr.s_addr = inet_addr(buffer);//Vlt noch ändern in IP Adresse
                                //     Typ              Cast zur Adresse              Länge der Adresse
                                int conect_status = connect(client_socket, (struct sockaddr *) &serv_ad, sizeof(serv_ad));
                                //Auffangen von Connection error
                                if (conect_status == -1) {
                                    printf("Verbindung fehlgeschlagen...\n\n");
                                }
                                if (conect_status == 0) {
                                    printf("Verbindung hat geklappt...\n\n");
                                    meineclients.connectedClients[nextFreeEntryPlace].isNotEmpty = 1;
                                    snprintf(meineclients.connectedClients[nextFreeEntryPlace].ipdesclient,
                                             sizeof(meineclients.connectedClients[nextFreeEntryPlace].ipdesclient),
                                             (char *) inet_ntoa(client_addr.sin_addr));
                                    time_t now;
                                    now = time(0);
                                    snprintf(meineclients.connectedClients[nextFreeEntryPlace].connectionEstablished,
                                             sizeof(meineclients.connectedClients[nextFreeEntryPlace].connectionEstablished),
                                             ctime(&now));
                                    cougar = nextFreeEntryPlace;
                                    nextFreeEntryPlace++;
                                    int pid3;
                                    if ((pid3 = fork()) < 0)//ERROR
                                    {
                                        perror("Error: Fork Error - please restart the Programm\n");// Throw Error
                                    }
                                    if (pid3 > 0)//PARENT PROCESS - Mainflow von Client
                                    {
                                        while (1) {
                                            if (semID >= 0) {
                                                /* Bereite die Semaphore vor und starte */
                                                sema.sem_num = 0;
                                                sema.sem_flg = SEM_UNDO;
                                                sema.sem_op = -1;
                                                if (-1 == semop(semID, &sema, 1)) {
                                                    /* Fehler */
                                                    perror("semop");
                                                }
                                                /* Code ab hier */
                                                int tag = 1;
                                                struct message nachricht;
                                                while (tag > 0) {
                                                    tag = msgrcv(feeder, &nachricht, sizeof(&nachricht),
                                                                 sizeof(meineclients.connectedClients[cougar]), 0);
                                                    if (tag < 0) {
                                                        perror(strerror(errno));
                                                        printf("msgrcv failed, rc=%d\n", tag);
                                                        exit(1);
                                                    }
                                                    printf("%s\n", nachricht.content);
                                                    send(client_socket, nachricht.content, strlen(nachricht.content), 0);
                                                }
                                                /* Code bis hier */
                                                sema.sem_op = 1;
                                                if (-1 == semop(semID, &sema, 1)) {
                                                    /* Fehler */
                                                    perror("semop");
                                                }
                                            } else {
                                                perror("semget");
                                            }
                                        }
                                    }
                                    if (pid3 == 0)//CHILD PROCESS - Sideflow von Client
                                    {
                                        int runboyrun = 1;
                                        while (runboyrun) {
                                            if (semID >= 0) {
                                                /* Bereite die Semaphore vor und starte */
                                                sema.sem_num = 0;
                                                sema.sem_flg = SEM_UNDO;
                                                sema.sem_op = -1;
                                                if (-1 == semop(semID, &sema, 1)) {
                                                    /* Fehler */
                                                    perror("semop");
                                                }
                                                /* Code ab hier */
                                                char buff[256];
                                                //TODO WAHLTEIL
                                                snprintf(buff, sizeof buff, "ECHO \n");
                                                send(client_socket, buff, strlen(buff), 0);
                                                char server_antwort[256];
                                                recv(client_socket, &server_antwort, sizeof(server_antwort), 0);
                                                char *informationen[1024];
                                                strtoken(server_antwort, informationen, 2);
                                                if (strcmp(informationen[0], "REPLY") == 0) {

                                                    snprintf(buff, sizeof buff, "GET TEMPERATURE \n");
                                                    send(client_socket, buff, strlen(buff), 0);
                                                    //Empfangen von Daten
                                                    recv(client_socket, &server_antwort, sizeof(server_antwort), 0);
                                                    strtoken(server_antwort, informationen, 2);
                                                    if (strcmp(informationen[0], "Temperatur") == 0) {
                                                        meineclients.connectedClients[cougar].meineSensorwerte.akTemp = atof(
                                                                informationen[1]);
                                                        if (meineclients.connectedClients[cougar].meineSensorwerte.minTemp >
                                                            atof(informationen[1]))
                                                            meineclients.connectedClients[cougar].meineSensorwerte.minTemp = atof(
                                                                    informationen[1]);
                                                        else if (
                                                                meineclients.connectedClients[cougar].meineSensorwerte.maxTemp <
                                                                atof(informationen[1]))
                                                            meineclients.connectedClients[cougar].meineSensorwerte.maxTemp = atof(
                                                                    informationen[1]);

                                                    }

                                                    snprintf(buff, sizeof buff, "GET HUMIDITYURE \n");
                                                    send(client_socket, buff, strlen(buff), 0);
                                                    //Empfangen von Daten

                                                    recv(client_socket, &server_antwort, sizeof(server_antwort), 0);

                                                    strtoken(server_antwort, informationen, 2);
                                                    if (strcmp(informationen[0], "Humidity") == 0) {
                                                        meineclients.connectedClients[cougar].meineSensorwerte.akHum = atof(
                                                                informationen[1]);
                                                        if (meineclients.connectedClients[cougar].meineSensorwerte.minHum >
                                                            atof(informationen[1]))
                                                            meineclients.connectedClients[cougar].meineSensorwerte.minHum = atof(
                                                                    informationen[1]);
                                                        else if (
                                                                meineclients.connectedClients[cougar].meineSensorwerte.maxHum <
                                                                atof(informationen[1]))
                                                            meineclients.connectedClients[cougar].meineSensorwerte.maxHum = atof(
                                                                    informationen[1]);
                                                    }

                                                    snprintf(buff, sizeof buff, "GET SOUNDNONURE \n");
                                                    send(client_socket, buff, strlen(buff), 0);
                                                    //Empfangen von Daten

                                                    recv(client_socket, &server_antwort, sizeof(server_antwort), 0);

                                                    strtoken(server_antwort, informationen, 2);
                                                    if (strcmp(informationen[0], "Sound") == 0) {
                                                        meineclients.connectedClients[cougar].meineSensorwerte.akDB = atof(
                                                                informationen[1]);
                                                        if (meineclients.connectedClients[cougar].meineSensorwerte.minDB >
                                                            atof(informationen[1]))
                                                            meineclients.connectedClients[cougar].meineSensorwerte.minDB = atof(
                                                                    informationen[1]);
                                                        else if (
                                                                meineclients.connectedClients[cougar].meineSensorwerte.maxDB <
                                                                atof(informationen[1]))
                                                            meineclients.connectedClients[cougar].meineSensorwerte.maxDB = atof(
                                                                    informationen[1]);
                                                    }

                                                    snprintf(buff, sizeof buff, "GET MOISTUREURE \n");
                                                    send(client_socket, buff, strlen(buff), 0);
                                                    //Empfangen von Daten

                                                    recv(client_socket, &server_antwort, sizeof(server_antwort), 0);

                                                    strtoken(server_antwort, informationen, 2);
                                                    if (strcmp(informationen[0], "Moisture") == 0) {
                                                        meineclients.connectedClients[cougar].meineSensorwerte.akWater = atoi(
                                                                informationen[1]);
                                                        if (meineclients.connectedClients[cougar].meineSensorwerte.minWater >
                                                            atoi(informationen[1]))
                                                            meineclients.connectedClients[cougar].meineSensorwerte.minWater = atoi(
                                                                    informationen[1]);
                                                        else if (
                                                                meineclients.connectedClients[cougar].meineSensorwerte.maxWater <
                                                                atoi(informationen[1]))
                                                            meineclients.connectedClients[cougar].meineSensorwerte.maxWater = atoi(
                                                                    informationen[1]);
                                                    }

                                                    snprintf(buff, sizeof buff, "GET MOTIONONURE \n");
                                                    send(client_socket, buff, strlen(buff), 0);
                                                    //Empfangen von Daten

                                                    recv(client_socket, &server_antwort, sizeof(server_antwort), 0);

                                                    strtoken(server_antwort, informationen, 2);
                                                    if (strcmp(informationen[0], "Motiondetected") == 0) {
                                                        meineclients.connectedClients[cougar].meineSensorwerte.motion = 1;
                                                    }

                                                    snprintf(buff, sizeof buff, "GET COLISIONURE \n");
                                                    send(client_socket, buff, strlen(buff), 0);
                                                    //Empfangen von Daten

                                                    recv(client_socket, &server_antwort, sizeof(server_antwort), 0);

                                                    strtoken(server_antwort, informationen, 2);
                                                    if (strcmp(informationen[0], "Colisiondetected") == 0) {
                                                        meineclients.connectedClients[cougar].meineSensorwerte.colision = atoi(
                                                                informationen[1]);
                                                    }
                                                } else {//TODO WAHLTEIL Überwachung
                                                    printf("Connection Lost: %s\n",
                                                           meineclients.connectedClients[cougar].ipdesclient);
                                                    runboyrun = 0;
                                                }

                                                /* Code bis hier */
                                                sema.sem_op = 1;
                                                if (-1 == semop(semID, &sema, 1)) {
                                                    /* Fehler */
                                                    perror("semop");
                                                }
                                                pi_sleep(10000);
                                            } else {
                                                perror("semget");
                                            }
                                        }
                                    }
                                }
                            }
                        } else if (strcmp(args[0], "PEERS") == 0) {
                            int i = 0;
                            printf("Meine Peers: \n");
                            while (meineclients.connectedClients[i].isNotEmpty) {
                                printf("<%s> [Connect since: %s]\n", meineclients.connectedClients[i].ipdesclient,
                                       meineclients.connectedClients[i].connectionEstablished);

                                printf("<%s> [Min-Temp: %f]\n", meineclients.connectedClients[i].ipdesclient,
                                       meineclients.connectedClients[i].meineSensorwerte.minTemp);
                                printf("<%s> [ak-Temp: %f]\n", meineclients.connectedClients[i].ipdesclient,
                                       meineclients.connectedClients[i].meineSensorwerte.akTemp);
                                printf("<%s> [Max-Temp: %f]\n", meineclients.connectedClients[i].ipdesclient,
                                       meineclients.connectedClients[i].meineSensorwerte.maxTemp);

                                printf("<%s> [Min-Hum: %f]\n", meineclients.connectedClients[i].ipdesclient,
                                       meineclients.connectedClients[i].meineSensorwerte.minHum);
                                printf("<%s> [ak-Hum: %f]\n", meineclients.connectedClients[i].ipdesclient,
                                       meineclients.connectedClients[i].meineSensorwerte.akHum);
                                printf("<%s> [Max-Hum: %f]\n", meineclients.connectedClients[i].ipdesclient,
                                       meineclients.connectedClients[i].meineSensorwerte.maxHum);

                                printf("<%s> [Min-DB: %f]\n", meineclients.connectedClients[i].ipdesclient,
                                       meineclients.connectedClients[i].meineSensorwerte.minDB);
                                printf("<%s> [ak-DB: %f]\n", meineclients.connectedClients[i].ipdesclient,
                                       meineclients.connectedClients[i].meineSensorwerte.akDB);
                                printf("<%s> [Max-DB: %f]\n", meineclients.connectedClients[i].ipdesclient,
                                       meineclients.connectedClients[i].meineSensorwerte.maxDB);

                                printf("<%s> [Min-Water: %d]\n", meineclients.connectedClients[i].ipdesclient,
                                       meineclients.connectedClients[i].meineSensorwerte.minWater);
                                printf("<%s> [ak-Water: %d]\n", meineclients.connectedClients[i].ipdesclient,
                                       meineclients.connectedClients[i].meineSensorwerte.akWater);
                                printf("<%s> [Max-Water: %d]\n", meineclients.connectedClients[i].ipdesclient,
                                       meineclients.connectedClients[i].meineSensorwerte.maxWater);

                                printf("<%s> [Motiondetected: %d]\n", meineclients.connectedClients[i].ipdesclient,
                                       meineclients.connectedClients[i].meineSensorwerte.motion);
                                printf("<%s> [Colisiondetected: %d]\n\n", meineclients.connectedClients[i].ipdesclient,
                                       meineclients.connectedClients[i].meineSensorwerte.colision);
                                i++;
                            }
                        }
                    }
                        if(pid8 == 0)//CHILD PROCESS - Typ1
                        {//TODO WAHLTEIL DISCOVERY
//                            printf("Lausche nach anderen Clients\n");
                            int sockfd;		/* socket */
                            sockfd = socket(AF_INET, SOCK_DGRAM, 0);
                            if (sockfd < 0)
                                perror("ERROR opening socket");
                            int portno = 4567;		/* port to listen on */
                            int clientlen;		/* byte size of client's address */
                            struct sockaddr_in serveraddr;	/* server's addr */
                            struct sockaddr_in clientaddr;	/* client addr */
                            struct hostent *hostp;	/* client host info */
                            char *buf;		/* message buf */
                            char *hostaddrp;	/* dotted decimal host addr string */
                            int optval;		/* flag value for setsockopt */
                            optval = 1;
                            setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
                                       (const void *)&optval, sizeof(int));
                            int n;			/* message byte size */
//                    zero((char *)&serveraddr, sizeof(serveraddr));
                            serveraddr.sin_family = AF_INET;
                            serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
                            serveraddr.sin_port = htons(4567);
                            if (bind(sockfd, (struct sockaddr *)&serveraddr,
                                     sizeof(serveraddr)) < 0)
                                perror("ERROR on binding");
                            clientlen = sizeof(clientaddr);
                            while (1) {

                                /*
                                 * recvfrom: receive a UDP datagram from a client
                                 */
                                buf = malloc(BUFSIZE);
                                n = recvfrom(sockfd, buf, BUFSIZE, 0,
                                             (struct sockaddr *)&clientaddr, &clientlen);
                                if (n < 0)
                                    perror("ERROR in recvfrom");
                                printf("%s\n",buf);
                                char *args[1024];
                                strtoken(buf, args, 2);
                                // Wir machen einen Socket:
                                int client_socket; // ADRESSE ODER PROTOKOLLFAMILE / SOCKET TYP / PROTOKOLL (TCP)
                                client_socket = socket(AF_INET, SOCK_STREAM, 0);
                                printf("Lege neuen Socket für andere Server an\n");
                                //Genauere Adresse fuer den Socket
                                struct sockaddr_in serv_ad;
                                serv_ad.sin_family = AF_INET;           //Adress-Familie
                                serv_ad.sin_port = htons((int) atof(args[1]));         //Portnummer
                                serv_ad.sin_addr.s_addr = inet_addr(args[0]);   //Vlt noch ändern in IP Adresse
                                //     Typ              Cast zur Adresse              Länge der Adresse
                                int conect_status = connect(client_socket, (struct sockaddr *) &serv_ad, sizeof(serv_ad));
                                //Auffangen von Connection error
                                if (conect_status == -1) {
                                    printf("Verbindung fehlgeschlagen...\n\n");
                                }
                                if (conect_status == 0) {
                                    printf("Verbindung hat geklappt...\n\n");
                                    meineclients.connectedClients[nextFreeEntryPlace].isNotEmpty = 1;
                                    snprintf(meineclients.connectedClients[nextFreeEntryPlace].ipdesclient,
                                             sizeof(meineclients.connectedClients[nextFreeEntryPlace].ipdesclient),
                                             (char *) inet_ntoa(client_addr.sin_addr));
                                    time_t now;
                                    now = time(0);
                                    snprintf(meineclients.connectedClients[nextFreeEntryPlace].connectionEstablished,
                                             sizeof(meineclients.connectedClients[nextFreeEntryPlace].connectionEstablished), ctime(&now));
                                    cougar = nextFreeEntryPlace;
                                    nextFreeEntryPlace++;
                                    int pid3;
                                    if ((pid3 = fork()) < 0)//ERROR
                                    {
                                        perror("Error: Fork Error - please restart the Programm\n");// Throw Error
                                    }
                                    if (pid3 > 0)//PARENT PROCESS - Mainflow von Client
                                    {
                                        while (1) {
                                            if (semID >= 0) {
                                                /* Bereite die Semaphore vor und starte */
                                                sema.sem_num = 0;
                                                sema.sem_flg = SEM_UNDO;
                                                sema.sem_op = -1;
                                                if (-1 == semop(semID, &sema, 1)) {
                                                    /* Fehler */
                                                    perror("semop");
                                                }
                                                /* Code ab hier */
                                                int tag = 1;
                                                struct message nachricht;
                                                while (tag > 0) {
                                                    tag = msgrcv(feeder, &nachricht, sizeof(&nachricht),
                                                                 sizeof(meineclients.connectedClients[cougar]), 0);
                                                    if (tag < 0) {
                                                        perror(strerror(errno));
                                                        printf("msgrcv failed, rc=%d\n", tag);
                                                        exit(1);
                                                    }
                                                    printf("%s\n", nachricht.content);
                                                    send(client_socket, nachricht.content, strlen(nachricht.content), 0);
                                                }
                                                /* Code bis hier */
                                                sema.sem_op = 1;
                                                if (-1 == semop(semID, &sema, 1)) {
                                                    /* Fehler */
                                                    perror("semop");
                                                }
                                            } else {
                                                perror("semget");
                                            }
                                        }
                                    }
                                    if (pid3 == 0)//CHILD PROCESS - Sideflow von Client
                                    {
                                        while (1) {
                                            if (semID >= 0) {
                                                /* Bereite die Semaphore vor und starte */
                                                sema.sem_num = 0;
                                                sema.sem_flg = SEM_UNDO;
                                                sema.sem_op = -1;
                                                if (-1 == semop(semID, &sema, 1)) {
                                                    /* Fehler */
                                                    perror("semop");
                                                }
                                                /* Code ab hier */
                                                char buff[256];
                                                snprintf(buff, sizeof buff, "GET TEMPERATURE \n");
                                                send(client_socket, buff, strlen(buff), 0);
                                                //Empfangen von Daten
                                                char server_antwort[256];
                                                recv(client_socket, &server_antwort, sizeof(server_antwort), 0);
                                                char *informationen[1024];
                                                strtoken(server_antwort, informationen, 2);
                                                if (informationen[0] == "Temperatur") {
                                                    meineclients.connectedClients[cougar].meineSensorwerte.akTemp = atof(informationen[1]);
                                                    if (meineclients.connectedClients[cougar].meineSensorwerte.minTemp >
                                                        atof(informationen[1]))
                                                        meineclients.connectedClients[cougar].meineSensorwerte.minTemp = atof(
                                                                informationen[1]);
                                                    else if (meineclients.connectedClients[cougar].meineSensorwerte.maxTemp <
                                                             atof(informationen[1]))
                                                        meineclients.connectedClients[cougar].meineSensorwerte.maxTemp = atof(
                                                                informationen[1]);

                                                }

                                                snprintf(buff, sizeof buff, "GET HUMIDITYURE \n");
                                                send(client_socket, buff, strlen(buff), 0);
                                                //Empfangen von Daten

                                                recv(client_socket, &server_antwort, sizeof(server_antwort), 0);

                                                strtoken(server_antwort, informationen, 2);
                                                if (informationen[0] == "Humidity") {
                                                    meineclients.connectedClients[cougar].meineSensorwerte.akHum = atof(informationen[1]);
                                                    if (meineclients.connectedClients[cougar].meineSensorwerte.minHum > atof(informationen[1]))
                                                        meineclients.connectedClients[cougar].meineSensorwerte.minHum = atof(informationen[1]);
                                                    else if (meineclients.connectedClients[cougar].meineSensorwerte.maxHum <
                                                             atof(informationen[1]))
                                                        meineclients.connectedClients[cougar].meineSensorwerte.maxHum = atof(informationen[1]);
                                                }

                                                snprintf(buff, sizeof buff, "GET SOUNDNONURE \n");
                                                send(client_socket, buff, strlen(buff), 0);
                                                //Empfangen von Daten

                                                recv(client_socket, &server_antwort, sizeof(server_antwort), 0);

                                                strtoken(server_antwort, informationen, 2);
                                                if (informationen[0] == "Sound") {
                                                    meineclients.connectedClients[cougar].meineSensorwerte.akDB = atof(informationen[1]);
                                                    if (meineclients.connectedClients[cougar].meineSensorwerte.minDB > atof(informationen[1]))
                                                        meineclients.connectedClients[cougar].meineSensorwerte.minDB = atof(informationen[1]);
                                                    else if (meineclients.connectedClients[cougar].meineSensorwerte.maxDB <
                                                             atof(informationen[1]))
                                                        meineclients.connectedClients[cougar].meineSensorwerte.maxDB = atof(informationen[1]);
                                                }

                                                snprintf(buff, sizeof buff, "GET MOISTUREURE \n");
                                                send(client_socket, buff, strlen(buff), 0);
                                                //Empfangen von Daten

                                                recv(client_socket, &server_antwort, sizeof(server_antwort), 0);

                                                strtoken(server_antwort, informationen, 2);
                                                if (informationen[0] == "Moisture") {
                                                    meineclients.connectedClients[cougar].meineSensorwerte.akWater = atoi(informationen[1]);
                                                    if (meineclients.connectedClients[cougar].meineSensorwerte.minWater >
                                                        atoi(informationen[1]))
                                                        meineclients.connectedClients[cougar].meineSensorwerte.minWater = atoi(
                                                                informationen[1]);
                                                    else if (meineclients.connectedClients[cougar].meineSensorwerte.maxWater <
                                                             atoi(informationen[1]))
                                                        meineclients.connectedClients[cougar].meineSensorwerte.maxWater = atoi(
                                                                informationen[1]);
                                                }

                                                snprintf(buff, sizeof buff, "GET MOTIONONURE \n");
                                                send(client_socket, buff, strlen(buff), 0);
                                                //Empfangen von Daten

                                                recv(client_socket, &server_antwort, sizeof(server_antwort), 0);

                                                strtoken(server_antwort, informationen, 2);
                                                if (informationen[0] == "Motiondetected") {
                                                    meineclients.connectedClients[cougar].meineSensorwerte.motion = 1;
                                                }

                                                snprintf(buff, sizeof buff, "GET COLISIONURE \n");
                                                send(client_socket, buff, strlen(buff), 0);
                                                //Empfangen von Daten

                                                recv(client_socket, &server_antwort, sizeof(server_antwort), 0);

                                                strtoken(server_antwort, informationen, 2);
                                                if (informationen[0] == "Colisiondetected") {
                                                    meineclients.connectedClients[cougar].meineSensorwerte.colision = atoi(informationen[1]);
                                                }
                                                /* Code bis hier */
                                                sema.sem_op = 1;
                                                if (-1 == semop(semID, &sema, 1)) {
                                                    /* Fehler */
                                                    perror("semop");
                                                }
                                                pi_sleep(10000);
                                            } else {
                                                perror("semget");
                                            }
                                        }
                                    }
                                }
                            }
                        }
                }
            }
        }
    }
    if(pid1 == 0)//CHILD PROCESS - Feeder
    {

        while (1) {
            if (semID >= 0) {
                /* Bereite die Semaphore vor und starte */
                sema.sem_num = 0;
                sema.sem_flg = SEM_UNDO;
                sema.sem_op = -1;
                if (-1 == semop(semID, &sema, 1)) {
                    /* Fehler */
                    perror("semop");
                }
                /* Code ab hier */
                setLCDTextmitRGB("LOLIBärchen", HIGHPRIO);pi_sleep(5000);
                if (meineclients.connectedClients[0].meineSensorwerte.akWater != getWasserkontakt(MOISTUREPORT)) {
                    int i = 0;
                    while (meineclients.connectedClients[i].isNotEmpty) {
                        struct message neueNachricht;
                        neueNachricht.mstype = sizeof(meineclients.connectedClients[i]);
                        sprintf(neueNachricht.content, "Waterdetected: %d\n",
                                getWasserkontakt(MOISTUREPORT));
                        msgsnd(feeder, &neueNachricht, sizeof(neueNachricht), 0);
                        i++;
                    }
                    struct message neueLCDNachricht;
                    neueLCDNachricht.mstype = (long) HIGHPRIO;
                    sprintf(neueLCDNachricht.content, "Waterdetected: %d\n",
                            getWasserkontakt(MOISTUREPORT));
                    msgsnd(lcdfeeder, &neueLCDNachricht, sizeof(neueLCDNachricht), 0);
                }

                if (meineclients.connectedClients[0].meineSensorwerte.motion != getBewegung(MOTIONPORT)) {
                    int i = 0;
                    while (meineclients.connectedClients[i].isNotEmpty) {
                        struct message neueNachricht;
                        neueNachricht.mstype = sizeof(meineclients.connectedClients[i]);
                        sprintf(neueNachricht.content, "Motiondetected: %d\n", getBewegung(MOTIONPORT));
                        msgsnd(feeder, &neueNachricht, sizeof(neueNachricht), 0);
                        i++;
                    }
                    struct message neueLCDNachricht;
                    neueLCDNachricht.mstype = (long) MIDDLEPRIO;
                    sprintf(neueLCDNachricht.content, "Motiondetected: %d\n", getBewegung(MOTIONPORT));
                    msgsnd(lcdfeeder, &neueLCDNachricht, sizeof(neueLCDNachricht), 0);
                }
                if (meineclients.connectedClients[0].meineSensorwerte.akDB != getGerausch(SOUNDPORT)) {
                    int i = 0;
                    while (meineclients.connectedClients[i].isNotEmpty) {
                        struct message neueNachricht;
                        neueNachricht.mstype = sizeof(meineclients.connectedClients[i]);
                        sprintf(neueNachricht.content, "Sound: %d\n", getGerausch(SOUNDPORT));
                        msgsnd(feeder, &neueNachricht, sizeof(neueNachricht), 0);
                        i++;
                    }
                    struct message neueLCDNachricht;
                    neueLCDNachricht.mstype = (long) MIDDLEPRIO;
                    sprintf(neueLCDNachricht.content, "Sound: %d\n", getGerausch(SOUNDPORT));
                    msgsnd(lcdfeeder, &neueLCDNachricht, sizeof(neueLCDNachricht), 0);
                }
                if (meineclients.connectedClients[0].meineSensorwerte.akTemp != getTempre(TEMPHUMPORT)) {
                    int i = 0;
                    while (meineclients.connectedClients[i].isNotEmpty) {
                        struct message neueNachricht;
                        neueNachricht.mstype = sizeof(meineclients.connectedClients[i]);
                        sprintf(neueNachricht.content, "Temperatur %f\n", getTempre(TEMPHUMPORT));
                        msgsnd(feeder, &neueNachricht, sizeof(neueNachricht), 0);
                        i++;
                    }
                    struct message neueLCDNachricht;
                    neueLCDNachricht.mstype = (long) LOWPRIO;
                    sprintf(neueLCDNachricht.content, "Temperatur %f\n", getTempre(TEMPHUMPORT));
                    msgsnd(lcdfeeder, &neueLCDNachricht, sizeof(neueLCDNachricht), 0);
                }
                if (meineclients.connectedClients[0].meineSensorwerte.akHum != getFeuchtigkeit(TEMPHUMPORT)) {
                    int i = 0;
                    while (meineclients.connectedClients[i].isNotEmpty) {
                        struct message neueNachricht;
                        neueNachricht.mstype = sizeof(meineclients.connectedClients[i]);
                        sprintf(neueNachricht.content, "Humidity %f\n", getFeuchtigkeit(TEMPHUMPORT));
                        msgsnd(feeder, &neueNachricht, sizeof(neueNachricht), 0);
                        i++;
                    }
                    struct message neueLCDNachricht;
                    neueLCDNachricht.mstype = (long) LOWPRIO;
                    sprintf(neueLCDNachricht.content, "Humidity: %f\n", getFeuchtigkeit(TEMPHUMPORT));
                    msgsnd(lcdfeeder, &neueLCDNachricht, sizeof(neueLCDNachricht), 0);
                }
                /* Code bis hier*/
                sema.sem_op = 1;
                if (-1 == semop(semID, &sema, 1)) {
                    /* Fehler */
                    perror("semop");
                }
            } else {
                perror("semget");
            }
        }
    }
}