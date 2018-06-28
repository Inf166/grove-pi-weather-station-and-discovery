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
//INCLUDE PI STUFF
#include "../grovepi.h"
//INCLUDE MESSAGEQUEUE STUFF
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
//INCLUDE MY SPAGGET
#include <sys/shm.h>
//INCLUDE SEMAPHORE
#include <sys/sem.h>
//Define In and Output
#define INPUT 0
#define OUTPUT 1
//Define Ports
#define LCDPORT 5
#define MOTIONPORT 6
#define COLISIONPORT 7
#define TEMPHUMPORT 8
#define SOUNDPORT 3
#define MOISTUREPORT 2
//Define Priority Codes
#define HIGHPRIO 1
#define MIDDLEPRIO 2
#define LOWPRIO 3

    //Struct Message
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
        int port = pport;
        //if(init()==-1)
        //    exit(1);
        pinMode(port,INPUT);
        if(digitalRead(port) == 0){
            return 1;
        }
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

    //Kreiert aus den feinsten Zutaten:
    char token_a[255]; //Tokens muessen ausserhalb der Funktion Deklariert werden, weil es mit "lokalem Speicher" nicht geht
    char token_b[255];
    char token_c[255];


    char * strtokBobbyTippingEdition(char input[], int num) {


        int t=0; //Fuer die Whileschleifen damit die Wissen welcher Token dran ist, 0=A, 1=B, 2=C, 3=Fertig
        int i=0; //Iterator fuer jeden char des Input Char Arrays
        int j=0; // Iterator fuer jeden Char des jeweiligen Tokens, wird zurueck auf 0 gesetzt wenn Token fertig ist

        printf("Input: %s\n", input); //gibt aus welchen Input die Funktion bekommen hat
        printf("Read and Set(A): "); //Die chars fuer Token A werden festgelegt
        while (t==0) {

            token_a[j]=input[i];
            printf("%c=%c, ", j, token_a[j], i, input[i] ); //Anzeige fuer jeweiligen Char
            i++;
            j++;
            if (input[i]==32) { //32 ist der ASCII Code fuer Leerzeichen, es wird \0 gesetzt und es geht weiter zum naechsten Token
                token_a[j]='\0';
                printf("\nToken A has detected Space and sets Zerobyte");
                i++;
                j=0;
                t++;
                printf("\nin Function: Token A: %s\n", token_a); //Das ist letztendlich Token A
            }
        }
        printf("Read and Set(B): ");
        while (t==1) {

            token_b[j]=input[i];
            printf("%c=%c, ", token_b[j], input[i] );
            i++;
            j++;
            if (input[i]==32) {
                token_b[j]='\0';
                printf("\nToken B has detected Space and sets Zerobyte");
                i++;
                j=0;
                t++;
                printf("\nin Function: Token B: %s\n", token_b);
            }
            else if (input[i]=='\0') { //falls es nur zwei Tokens gibt wird hier bereits das \0 gesetzt
                token_b[j]='\0';
                printf("\nToken B has detected Zerobyte and sets Zerobyte");
                t+=2;
                printf("\nin Function: Token B: %s\n", token_b);
            }

        }
        printf("Read and Set(C): ");
        while (t==2) {

            token_c[j]=input[i];
            printf("%c=%c, ", token_c[j], input[i] );
            i++;
            j++;
            if (input[i]=='\0') {
                token_c[j]='\0';
                printf("\nToken C has detected Zerobyte and sets Zerobyte");
                t++;
                printf("\nin Function: Token C: %s\n", token_c);
            }

        }

        switch(num) {
            case 0: printf("RETURN: Token A: %s\n\n", token_a); return token_a; break; //der Zweite Parameter num gibt an welcher Token returnt wird
            case 1: printf("RETURN: Token B: %s\n\n", token_b);return token_b; break;
            case 2: printf("RETURN: Token C: %s\n\n", token_c);return token_c; break;
            default: return "Fuer den zweiten Parameter ist nur 0-2 gueltig";
        }



    }



int main(){
// Just some Stuff for the Pi
    init();
    connectLCD();

// The Server Stuff
    // Erstelle Server Socket
        int server_socket;
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket <0) {
            perror("Error: unable to open a socket\n");
            setLCDTextmitRGB("FATAL ERROR :x",HIGHPRIO);
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
        if ((bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address))) <0) {
            perror("Error: unable to bind\n");          // Wenn bind fehlschlaegt gibt es eine Fehlermeldug.
            setLCDTextmitRGB("FATAL ERROR :(",HIGHPRIO);
        } else{ puts("Bind erfolgreich.\n");            // Sonst Bestaetigung
            // READY FOR CLIENTS
            setLCDTextmitRGB("I BIM READY :)",MIDDLEPRIO);
        }

    // LEGE LISTE VON CONNECTED-CLIENTS AN
        struct client connectedClients[35];
    // TODO - THIS SHOULD MAKE THE FIRST ENTRY TO OUR STRUCT... but it doesn't...
        printf("My IP address is: %s\n", inet_ntoa(server_address.sin_addr));
        snprintf(connectedClients[0].ipdesclient , sizeof(connectedClients[0].ipdesclient),(char*) inet_ntoa(server_address.sin_addr));
        connectedClients[0].isNotEmpty = 1;

    // listen to conections
        listen(server_socket, 5);
        puts("Warten auf verbindung...\n");

    // Lege Client Socket an
        struct sockaddr_in client_addr;
        socklen_t client_len=sizeof(struct sockaddr_in);

    // Lege Key fuer Shared Memory TODO MSG SCHLEIFE ALS SHARED MEMORY

      //InItIaLiZe ThE MeSsAgEqUeUe
          key_t key;
          int feeder;
          key = ftok("something", 65);
          feeder = msgget(key, IPC_CREAT);
      //Putin shared MEMORY
          key_t shkey = ftok("sollthing",65);
          int shid = shmget(shkey,sizeof(feeder),IPC_CREAT|0777);
          feeder = (int) shmat(shid,NULL,0); //kein plan

      //InItIaLiZe ThE lCdMeSsAgEqUeUe
          key_t key2;
          int lcdfeeder;
          key2 = ftok("mssomething", 65);
          lcdfeeder = msgget(key2, IPC_CREAT);
      //Putin shared LCDMEMORY
          key_t mskey = ftok("msoslllol",65);
          int shit = shmget(mskey,sizeof(lcdfeeder),IPC_CREAT|0777);
          lcdfeeder = (int) shmat(shit,NULL,0);//kein plan

      //CREATE SEMAPHORE
      int semID;
      struct sembuf sema;
      semID = semget(2404, 1, IPC_CREAT | 0666);
      //first message?!?!
          // int i = 0;
          // message neueNachricht;
          // neueNachricht.mstype = sizeof(connectedClients[i]);
          // neueNachricht.content = getTemperature;
          // msgsnd(feeder, &neueNachricht, sizeof(neueNachricht), 0);


// First Fork: Mainflow and Sensorenwarteschlange
    // pid equals Process ID Filedesc indicates Process
        int pid;
        // CREATE FORK
            if ((pid = fork()) <0)      // ERROR OF FORK - PLEASE THROW ERROR
            {
                // Throw Error
                perror("Error: Fork Error - please restart the Programm\n");
                setLCDTextmitRGB("Fork Error :(",HIGHPRIO);
            }
            else if (pid > 0)           // PARENT PROCESS - CONTINUE SERVER STUFF
            {
            // Second Fork: Server should accept and Client should Connect
                // pid equals Process ID Filedesc indicates Process
                int pid2;
                // CREATE FORK
                if ((pid2 = fork()) <0)      // ERROR OF FORK - PLEASE THROW ERROR
                {
                    // Throw Error
                    perror("Error: Fork Error - please restart the Programm\n");
                    setLCDTextmitRGB("Fork Error :(",HIGHPRIO);
                }
                else if (pid2 > 0)           // PARENT PROCESS - CONTINUE SERVER STUFF
                {
                // Fourth Fork: LCD Stuff
                    // pid equals Process ID Filedesc indicates Process
                    int pid4;
                    // CREATE FORK
                    if ((pid4 = fork()) <0)      // ERROR OF FORK - PLEASE THROW ERROR
                    {
                        // Throw Error
                        perror("Error: Fork Error - please restart the Programm\n");
                        setLCDTextmitRGB("Fork Error :(",HIGHPRIO);
                    }
                    else if (pid4 > 0)           // PARENT PROCESS - MAINFLOW
                    {
                    // Fifth Fork: Server should accept and Client should Connect
                        // pid equals Process ID Filedesc indicates Process
                        int pid5, fileDesc;

                        // Initialize Counter for Sons
                        static int counter=0;

                        // Do the Loop
                        while (1)
                        {
                            // Verbindung wird Serverseitig angenommen
                            fileDesc = accept(server_socket, (struct sockaddr *) &client_addr, &client_len);
                            // Gebe Erfolg auf Konsole aus
                            puts("Verbindung akzeptiert\n");
                            // Zeige verbundene IP-Adresse an - for DEBUG
                            printf("IP address is: %s\n", inet_ntoa(client_addr.sin_addr));
                            if ((pid5 = fork()) <0)      // ERROR OF FORK - PLEASE THROW ERROR
                            {
                                // Throw Error
                                perror("Error: Fork Error - please restart the Programm\n");
                                setLCDTextmitRGB("Fork Error :(",HIGHPRIO);
                                // ONLY SON Continues to Communicate with Clients
                                close(fileDesc);
                                // Continue the While
                                continue;
                            }
                            else if (pid5 > 0)           // PARENT PROCESS - ACCEPT INCOMING CLIENTS
                            {
                                // ONLY SON Continues to Communicate with Clients
                                close(fileDesc);
                                // ADD +1 to Counter-Var
                                counter++;
                            }
                            else if (pid5 == 0)          // CHILD PROCESS - COMMUNICATE WITH CLIENT
                            {
                            // Sixth Fork: CONNECT BEFEHL UND
                                // pid equals Process ID Filedesc indicates Process
                                int pid6;
                                // ADD +1 to Counter-Var
                                counter++;
                                // CREATE FORK
                                if ((pid6 = fork()) <0)      // ERROR OF FORK - PLEASE THROW ERROR
                                {
                                    // Throw Error
                                    perror("Error: Fork Error - please restart the Programm\n");
                                    setLCDTextmitRGB("Fork Error :(",HIGHPRIO);
                                }
                                else if (pid6 > 0)           // PARENT PROCESS - SERVER RESPONSE
                                {
                                    // Create a Buffer-Var
                                    char buf[100];
                                    // SEND Welcome
                                    snprintf(buf, sizeof buf, "Hallo User %d\n", counter);
                                    send (fileDesc, buf, strlen(buf), 0);
                                    // Abbruchvariable: running als Abbruchsbedingung fuer die Kommunikation - Wird running geaendert ist die Server-Client-Kommunikation abgeschlossen
                                    int running=1;
                                    while(running) {
                                        // RESERVIERE VARIABLEN
                                        char client_cmd[256];
                                        char *args[1024];
                                        // RECEIVE NICE VIBES
                                        recv(fileDesc, &client_cmd, sizeof(client_cmd), 0);
                                        printf("User: %s\n", client_cmd);
                                        // teilt die Client-Nachricht in 2 Teile an den Leerstellen WICHTIG!! Das 2. Wort muss mit einem Leerzeichen enden!!
                                        strtoken(client_cmd, args, 2);
                                        // CHECK SENSORS FROM CONNECTED CLIENT
                                        if(strcmp(args[0], "GET") == 0){
                                            if(strcmp(args[1], "TEMPERATURE") == 0){
                                                snprintf(buf, sizeof buf, "<192.168.2.27> [TEMPERATURE: %f]\n", getTempre(TEMPHUMPORT));
                                                send (fileDesc, buf, strlen(buf), 0);
                                            } else if(strcmp(args[1], "HUMIDITYURE") == 0){
                                                snprintf(buf, sizeof buf, "<192.168.2.27> [HUMIDITY: %f]\n", getTempre(TEMPHUMPORT));
                                                send (fileDesc, buf, strlen(buf), 0);
                                            }else if(strcmp(args[1], "MOTIONONURE") == 0){
                                                char* response;
                                                if(getBewegung(MOTIONPORT)==1){ response = "DETECTED";}
                                                else {response = "NONE";}
                                                snprintf(buf, sizeof buf, "<192.168.2.27> [MOTION: %s]\n", response);
                                                send (fileDesc, buf, strlen(buf), 0);
                                            }else if(strcmp(args[1], "SOUNDNONURE") == 0){
                                                snprintf(buf, sizeof buf, "<192.168.2.27> [SOUND: %d]\n", getGerausch(SOUNDPORT));
                                                send (fileDesc, buf, strlen(buf), 0);
                                            }else if(strcmp(args[1], "COLISIONURE") == 0){
                                                char* response;
                                                if(getColision(COLISIONPORT)==1){ response = "DETECTED";}
                                                else {response = "NONE";}
                                                snprintf(buf, sizeof buf, "<192.168.2.27> [COLISION: %f]\n", response);
                                                send (fileDesc, buf, strlen(buf), 0);
                                            }else if(strcmp(args[1], "MOISTUREURE") == 0){
                                                snprintf(buf, sizeof buf, "<192.168.2.27> [MOISTURE: %d]\n", getWasserkontakt(MOISTUREPORT));
                                                send (fileDesc, buf, strlen(buf), 0);
                                            }
                                        // PRESS E AND SEND TO BREAK WHILE
                                        } else if (strcmp(args[0], "E") == 0) {
                                            snprintf(buf, sizeof buf, "BYE BYE, User %d\n", counter);
                                            send (fileDesc, buf, strlen(buf), 0);
                                            running = 0;
                                            // IF THE CHECKER-WHILE BREAKS - THROW END OF INNER WHILE
                                            close(fileDesc);
                                            printf("VERBINDUNG GESCHLOSSEN");
                                            break;
                                        // THROW ERROR IF FIRST WORD IS UNKNOWN
                                        } else {
                                            char errorlol [40]={0};
                                            sprintf(errorlol,"Fehlerhafte Eingabe.\n");
                                            send (fileDesc, errorlol, strlen(errorlol), 0);
                                        }
                                    }
                                    // IF THE CHECKER-WHILE BREAKS - THROW END OF INNER WHILE
                                    close(fileDesc);
                                    printf("VERBINDUNG GESCHLOSSEN\n\n");
                                    // IF INNER-WHILE BREAKS - TROW OUTER BREAK
                                    break;
                                }
                                else if (pid6 == 0)          // CHILD PROCESS - MESSAGE SENDEN AN CLIENT
                                {
                                  while (1) {
                                    if (semID >= 0) {
                                      /* Bereite die Semaphore vor und starte */
                                      sema.sem_num = 0;
                                      sema.sem_flg = SEM_UNDO;
                                      sema.sem_op  = -1;
                                      if (-1==semop(semID, &sema, 1)) {
                                          /* Fehler */
                                          perror("semop");
                                      }
                                      int tag;
                                      struct message nachricht;
                                      while(tag>0){

                                        tag=msgrcv(feeder,&nachricht,sizeof(&nachricht),sizeof(connectedClients[counter]),0);
                                        if (tag < 0) {
                                          perror( strerror(errno) );
                                          printf("msgrcv failed, rc=%d\n", tag);
                                          exit(1);
                                        }
                                        printf("%s\n", nachricht.content);
                                        send (fileDesc, nachricht.content, strlen(nachricht.content), 0);
                                      }
                                      /* Gebe Sem wieder frei */
                                      sema.sem_op  = 1;
                                      if (-1==semop(semID, &sema, 1)) {
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
                    else if (pid4 == 0)          // CHILD PROCESS - NACHRICHTENWARTESCHLANGE FUER LCD
                    {
                        while (1) {
                          if (semID >= 0) {
                            /* Bereite die Semaphore vor und starte */
                            sema.sem_num = 0;
                            sema.sem_flg = SEM_UNDO;
                            sema.sem_op  = -1;
                            if (-1==semop(semID, &sema, 1)) {
                                /* Fehler */
                                perror("semop");
                            }
                            int tag;
                            struct message nachricht;
                            while(tag > 0){
                            tag=msgrcv(lcdfeeder,&nachricht,sizeof(&nachricht),(long)HIGHPRIO,0);
                            if (tag < 0) {
                              perror( strerror(errno) );
                              printf("msgrcv failed, rc=%d\n", tag);
                              exit(1);
                            }
                            printf("%s\n", nachricht.content);
                            setLCDTextmitRGB(nachricht.content,(int)nachricht.mstype);
                            pi_sleep(1000); //Alternative für die Lesbarkeit
                            }
                            while(tag > 0){
                            tag=msgrcv(lcdfeeder,&nachricht,sizeof(&nachricht),(long)MIDDLEPRIO,0);
                            if (tag < 0) {
                              perror( strerror(errno) );
                              printf("msgrcv failed, rc=%d\n", tag);
                              exit(1);
                            }
                            printf("%s\n", nachricht.content);
                            setLCDTextmitRGB(nachricht.content,(int)nachricht.mstype);
                            pi_sleep(1000); //Alternative für die Lesbarkeit
                            }
                            while(tag > 0){
                            tag=msgrcv(lcdfeeder,&nachricht,sizeof(&nachricht),(long)LOWPRIO,0);
                            if (tag < 0) {
                              perror( strerror(errno) );
                              printf("msgrcv failed, rc=%d\n", tag);
                              exit(1);
                            }
                            printf("%s\n", nachricht.content);
                            setLCDTextmitRGB(nachricht.content,(int)nachricht.mstype);
                            pi_sleep(1000); //Alternative für die Lesbarkeit
                            }
                            /* Gebe Sem wieder frei */
                            sema.sem_op  = 1;
                            if (-1==semop(semID, &sema, 1)) {
                                /* Fehler */
                                perror("semop");
                            }
                          } else {
                              perror("semget");
                          }
                        }
                    }
                }
                else if (pid2 == 0)          // CHILD PROCESS - SERVER-TURNS_TO_BE_CLIENT SIDE
                {
                // Third Fork: CONNECT BEFEHL UND
                    //TODO  CONNECT BEFEHL & IF SUCESSFUL MAKE A NEW FORK
                    char eingabe[256];
                    char *args[1024];
                    printf("Bitte EINGABE: CONNECT XXX.XXX.XXX.XXX XXXX\n");
                    scanf("%s", &eingabe);
                    strtoken(eingabe,args,3);
                    if(strcmp(args[0], "CONNECT") == 0){

                        // Wir machen einen Socket:
                        int client_socket; // ADRESSE ODER PROTOKOLLFAMILE / SOCKET TYP / PROTOKOLL (TCP)
                        client_socket = socket(AF_INET, SOCK_STREAM, 0);
                        printf("Lege neuen Socket für andere Server an\n");
                        //Genauere Adresse fuer den Socket
                        struct sockaddr_in serv_ad;
                        serv_ad.sin_family = AF_INET;           //Adress-Familie
                        serv_ad.sin_port = htons((int)args[2]);         //Portnummer
                        serv_ad.sin_addr.s_addr = inet_addr(args[1]);   //Vlt noch ändern in IP Adresse
                        //     Typ              Cast zur Adresse              Länge der Adresse
                        int conect_status = connect(client_socket, (struct sockaddr *) &serv_ad, sizeof(serv_ad));
                        //Auffangen von Connection error
                        if(conect_status == -1){
                            printf("Verbindung fehlgeschlagen...\n\n");
                        }
                        if(conect_status == 0){
                            printf("Verbindung hat geklappt...\n\n");
                            int runner = 0;
                            while(connectedClients[runner].isNotEmpty){runner++;}//TODO var shared memory anlegen - damit nicht endlos
                            connectedClients[runner].isNotEmpty =1;
                            strcpy(connectedClients[runner].ipdesclient , args[1]);
                            // pid equals Process ID Filedesc indicates Process
                            int pid3;
                            // CREATE FORK
                            if ((pid3 = fork()) <0)      // ERROR OF FORK - PLEASE THROW ERROR
                            {
                                // Throw Error
                                perror("Error: Fork Error - please restart the Programm\n");
                                setLCDTextmitRGB("Fork Error :(",HIGHPRIO);
                            }
                            else if (pid3 > 0)           // PARENT PROCESS - CONNECT BEFEHL
                            {
                              char unsercmd[256];
                              scanf("%s\n", unsercmd);
                              send (client_socket, unsercmd, strlen(unsercmd), 0);
                              //Empfangen von Daten
                              char server_antwort[256];
                              recv(client_socket, &server_antwort, sizeof(server_antwort), 0);
                              //Ausgabe
                              printf("Dateien empfangen: %s\n", server_antwort);
                            }
                            else if (pid3 == 0)          // CHILD PROCESS - FUER JEDEN NEUEN VERBUNDENEN SERVER ERSTELLE DIESEN FORK
                            {
                                while(1){
                                  //Empfangen von Daten
                                  char server_antwort[256];
                                  recv(client_socket, &server_antwort, sizeof(server_antwort), 0);
                                  char *informationen[1024];
                                  strtoken(server_antwort, informationen,2);
                                  if(informationen[0]=="Temperatur"){
                                    connectedClients[runner].meineSensorwerte.akTemp = atof(informationen[1]);
                                    if(connectedClients[runner].meineSensorwerte.minTemp>   atof(informationen[1]))
                                      connectedClients[runner].meineSensorwerte.minTemp = atof(informationen[1]);
                                    else if(connectedClients[runner].meineSensorwerte.maxTemp< atof(informationen[1]))
                                      connectedClients[runner].meineSensorwerte.maxTemp = atof(informationen[1]);

                                  }
                                  else if(informationen[0]=="Humidity"){
                                    connectedClients[runner].meineSensorwerte.akHum = atof(informationen[1]);
                                    if(connectedClients[runner].meineSensorwerte.minHum>atof(informationen[1]))
                                      connectedClients[runner].meineSensorwerte.minHum = atof(informationen[1]);
                                    else if(connectedClients[runner].meineSensorwerte.maxHum<atof(informationen[1]))
                                      connectedClients[runner].meineSensorwerte.maxHum = atof(informationen[1]);
                                  }
                                  else if(informationen[0]=="Sound"){
                                    connectedClients[runner].meineSensorwerte.akDB = atof(informationen[1]);
                                    if(connectedClients[runner].meineSensorwerte.minDB>atof(informationen[1]))
                                      connectedClients[runner].meineSensorwerte.minDB = atof(informationen[1]);
                                    else if(connectedClients[runner].meineSensorwerte.maxDB<atof(informationen[1]))
                                      connectedClients[runner].meineSensorwerte.maxDB = atof(informationen[1]);
                                  }
                                  else if(informationen[0]=="Waterdetected"){
                                    connectedClients[runner].meineSensorwerte.akWater =  atoi(informationen[1]);
                                    if(connectedClients[runner].meineSensorwerte.minWater>atoi(informationen[1]))
                                      connectedClients[runner].meineSensorwerte.minWater = atoi(informationen[1]);
                                    else if(connectedClients[runner].meineSensorwerte.maxWater<atoi(informationen[1]))
                                      connectedClients[runner].meineSensorwerte.maxWater = atoi(informationen[1]);
                                  }
                                  else if(informationen[0]=="Motiondetected"){
                                    connectedClients[runner].meineSensorwerte.motion = atoi(informationen[1]);
                                  }
                                  else if(informationen[0]=="Colisiondetected"){
                                    connectedClients[runner].meineSensorwerte.colision = atoi(informationen[1]);
                                  }
                                  else {
                                    //do nothing at all
                                  }
                                }
                            }
                        }
                    }
                }
            }
            else if (pid == 0)          // CHILD PROCESS - FEED THE QUEUE WITH SENSOR DATA FOR EACH CONNECTED CLIENT
            {
                //Ausgangswerte einmalig festlegen
                connectedClients[0].meineSensorwerte.minTemp=getTempre(TEMPHUMPORT);
                connectedClients[0].meineSensorwerte.akTemp=getTempre(TEMPHUMPORT);
                connectedClients[0].meineSensorwerte.maxTemp=getTempre(TEMPHUMPORT);

                connectedClients[0].meineSensorwerte.minHum=getFeuchtigkeit(TEMPHUMPORT);
                connectedClients[0].meineSensorwerte.akHum=getFeuchtigkeit(TEMPHUMPORT);
                connectedClients[0].meineSensorwerte.maxHum=getFeuchtigkeit(TEMPHUMPORT);

                connectedClients[0].meineSensorwerte.minDB=getGerausch(SOUNDPORT);
                connectedClients[0].meineSensorwerte.akDB=getGerausch(SOUNDPORT);
                connectedClients[0].meineSensorwerte.maxDB=getGerausch(SOUNDPORT);

                connectedClients[0].meineSensorwerte.minWater=getWasserkontakt(MOISTUREPORT);
                connectedClients[0].meineSensorwerte.akWater=getWasserkontakt(MOISTUREPORT);
                connectedClients[0].meineSensorwerte.maxWater=getWasserkontakt(MOISTUREPORT);

                connectedClients[0].meineSensorwerte.motion=getBewegung(MOTIONPORT);

                connectedClients[0].meineSensorwerte.colision=getColision(COLISIONPORT);

                while (1) {
                  if (semID >= 0) {
                    /* Bereite die Semaphore vor und starte */
                    sema.sem_num = 0;
                    sema.sem_flg = SEM_UNDO;
                    sema.sem_op  = -1;
                    if (-1==semop(semID, &sema, 1)) {
                        /* Fehler */
                        perror("semop");
                    }
                    printf("neue Werte :>>>>");
                    if(connectedClients[0].meineSensorwerte.akTemp != getTempre(TEMPHUMPORT)){
                      int i = 0;
                      while(connectedClients[i].isNotEmpty){
                        struct message neueNachricht;
                        neueNachricht.mstype = sizeof(connectedClients[i]);
                        sprintf(neueNachricht.content,"Temperatur %f\n",getTempre(TEMPHUMPORT));
                        msgsnd(feeder, &neueNachricht, sizeof(neueNachricht), 0);
                        i++;
                      }
                        struct message neueLCDNachricht;
                      neueLCDNachricht.mstype = (long) LOWPRIO;
                      sprintf(neueLCDNachricht.content,"Temperatur %f\n",getTempre(TEMPHUMPORT));
                      msgsnd(lcdfeeder, &neueLCDNachricht, sizeof(neueLCDNachricht), 0);
                    }
                    if(connectedClients[0].meineSensorwerte.akHum != getFeuchtigkeit(TEMPHUMPORT)){
                      int i = 0;
                      while(connectedClients[i].isNotEmpty){
                          struct message neueNachricht;
                        neueNachricht.mstype = sizeof(connectedClients[i]);
                        sprintf(neueNachricht.content,"Humidity %f\n",getFeuchtigkeit(TEMPHUMPORT));
                        msgsnd(feeder, &neueNachricht, sizeof(neueNachricht), 0);
                        i++;
                      }
                        struct message neueLCDNachricht;
                      neueLCDNachricht.mstype = (long) LOWPRIO;
                      sprintf(neueLCDNachricht.content,"Humidity: %f\n",getFeuchtigkeit(TEMPHUMPORT));
                      msgsnd(lcdfeeder, &neueLCDNachricht, sizeof(neueLCDNachricht), 0);
                    }
                    if(connectedClients[0].meineSensorwerte.akDB != getGerausch(SOUNDPORT)){
                      int i = 0;
                      while(connectedClients[i].isNotEmpty){
                          struct message neueNachricht;
                        neueNachricht.mstype = sizeof(connectedClients[i]);
                        sprintf(neueNachricht.content,"Sound: %f\n",getGerausch(SOUNDPORT));
                        msgsnd(feeder, &neueNachricht, sizeof(neueNachricht), 0);
                        i++;
                      }
                        struct message neueLCDNachricht;
                      neueLCDNachricht.mstype = (long) LOWPRIO;
                      sprintf(neueLCDNachricht.content,"Sound: %f\n",getGerausch(SOUNDPORT));
                      msgsnd(lcdfeeder, &neueLCDNachricht, sizeof(neueLCDNachricht), 0);
                    }
                    if(connectedClients[0].meineSensorwerte.akWater != getWasserkontakt(MOISTUREPORT)){
                      int i = 0;
                      while(connectedClients[i].isNotEmpty){
                          struct message neueNachricht;
                        neueNachricht.mstype = sizeof(connectedClients[i]);
                        sprintf(neueNachricht.content,"Waterdetected: %d\n",getWasserkontakt(MOISTUREPORT));
                        msgsnd(feeder, &neueNachricht, sizeof(neueNachricht), 0);
                        i++;
                      }
                        struct message neueLCDNachricht;
                      neueLCDNachricht.mstype = (long) HIGHPRIO;
                      sprintf(neueLCDNachricht.content,"Waterdetected: %d\n",getWasserkontakt(MOISTUREPORT));
                      msgsnd(lcdfeeder, &neueLCDNachricht, sizeof(neueLCDNachricht), 0);
                    }
                    if(connectedClients[0].meineSensorwerte.colision != getColision(COLISIONPORT)){
                      int i = 0;
                      while(connectedClients[i].isNotEmpty){
                          struct message neueNachricht;
                        neueNachricht.mstype = sizeof(connectedClients[i]);
                        sprintf(neueNachricht.content,"Colisiondetected: %d\n",getColision(COLISIONPORT));
                        msgsnd(feeder, &neueNachricht, sizeof(neueNachricht), 0);
                        i++;
                      }
                        struct  message neueLCDNachricht;
                      neueLCDNachricht.mstype = (long) LOWPRIO;
                      sprintf(neueLCDNachricht.content,"Colisiondetected: %d\n",getColision(COLISIONPORT));
                      msgsnd(lcdfeeder, &neueLCDNachricht, sizeof(neueLCDNachricht), 0);
                    }
                    if(connectedClients[0].meineSensorwerte.motion != getBewegung(MOTIONPORT)){
                      int i = 0;
                      while(connectedClients[i].isNotEmpty){
                          struct message neueNachricht;
                        neueNachricht.mstype = sizeof(connectedClients[i]);
                        sprintf(neueNachricht.content,"Motiondetected: %d\n",getBewegung(MOTIONPORT));
                        msgsnd(feeder, &neueNachricht, sizeof(neueNachricht), 0);
                        i++;
                      }
                        struct message neueLCDNachricht;
                      neueLCDNachricht.mstype = (long) MIDDLEPRIO;
                      sprintf(neueLCDNachricht.content,"Motiondetected: %d\n",getBewegung(MOTIONPORT));
                      msgsnd(lcdfeeder, &neueLCDNachricht, sizeof(neueLCDNachricht), 0);
                    }
                    /* Gebe Sem wieder frei */
                    sema.sem_op  = 1;
                    if (-1==semop(semID, &sema, 1)) {
                        /* Fehler */
                        perror("semop");
                    }
                  pi_sleep(10000);
                  } else {
                      perror("semget");
                  }

                }
            }

    // close Funktion
        close(server_socket);
    // Return 0 for the lols
        return 0;
}
