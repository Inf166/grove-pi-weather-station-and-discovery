/* Include Libaries */
#include "../grovepi.h"
  #include <arpa/inet.h>
  #include <errno.h>
  #include <netinet/in.h>
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
  #define MOISTUREPORT 2
  #define SOUNDPORT 3
  #define LCDPORT 5
  #define MOTIONPORT 6
  #define COLISIONPORT 7
  #define TEMPHUMPORT 8
  #define HIGHPRIO 1
  #define MIDDLEPRIO 2
  #define LOWPRIO 3
/* Structs */
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
     char connectionEstablished[255];
     struct sensorwerte meineSensorwerte;
   };
   int nextFreeEntryPlace = 0;
/* Functions */
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
/* main-Function */
  int main(){
  //TODO SOCKETS()
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
          // listen to conections
              listen(server_socket, 5);
              puts("Warten auf verbindung...\n");
          // Lege Client Socket an
              struct sockaddr_in client_addr;
              socklen_t client_len=sizeof(struct sockaddr_in);
  //TODO SENSOREN()
        init();
        connectLCD();
  //TODO SHARED MEMORY AND MESSAGE QUEUES
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
    //Putin shared LogMEMORY
    // LEGE LISTE VON CONNECTED-CLIENTS AN
        struct client connectedClients[35];
    // TODO - THIS SHOULD MAKE THE FIRST ENTRY TO OUR STRUCT... but it doesn't...
        printf("My IP address is: %s\n", inet_ntoa(server_address.sin_addr));
        snprintf(connectedClients[0].ipdesclient , sizeof(connectedClients[0].ipdesclient),(char*) inet_ntoa(server_address.sin_addr));
        connectedClients[0].isNotEmpty = 1;
        connectedClients[0].connectionEstablished = ctime(time(time_t t));
        nextFreeEntryPlace++;
        key_t logkey = ftok("logmichan",65);
        int shat = shmget(logkey,sizeof(connectedClients),IPC_CREAT|0777);
        connectedClients = (int) shmat(shit,NULL,0);//kein plan
    //CREATE SEMAPHORE
    int semID;
    struct sembuf sema;
    semID = semget(2404, 1, IPC_CREAT | 0666);
    // First Fork:
      int pid;  // pid equals Process ID
      // CREATE FORK
          if ((pid = fork()) <0)      // ERROR OF FORK - PLEASE THROW ERROR
          {
              // Throw Error
              perror("Error: Fork Error - please restart the Programm\n");
          }
          else if (pid > 0)           // PARENT PROCESS - SERVER STUFF
          {
            // Second Fork:
              int pid2;  // pid equals Process ID
              // CREATE FORK
                  if ((pid2 = fork()) <0)      // ERROR OF FORK - PLEASE THROW ERROR
                  {
                      // Throw Error
                      perror("Error: Fork Error - please restart the Programm\n");
                  }
                  else if (pid2 > 0)           // PARENT PROCESS - MAIN FLOW
                  {
                    // Third Fork:
                      int pid3;  // pid equals Process ID
                      // CREATE FORK
                          if ((pid3 = fork()) <0)      // ERROR OF FORK - PLEASE THROW ERROR
                          {
                              // Throw Error
                              perror("Error: Fork Error - please restart the Programm\n");
                          }
                          else if (pid3 > 0)           // PARENT PROCESS - SERVER INITIALIZE
                          {
                            //TODO INTIALIZE THE Server
                            // Do the Loop
                            int pid5, fileDesc;
                            while (1)
                            {
                                // Verbindung wird Serverseitig angenommen
                                fileDesc = accept(server_socket, (struct sockaddr *) &client_addr, &client_len);
                                // Gebe Erfolg auf Konsole aus
                                puts("Verbindung akzeptiert\n");
                                // Zeige verbundene IP-Adresse an - for DEBUG
                                printf("IP address is: %s\n", inet_ntoa(client_addr.sin_addr));

                            // IF ACCEPT DANN FORK
                            // Fifth Fork:


                              // Initialize Counter for Sons
                              static int counter=0;
                              // CREATE FORK
                                  if ((pid5 = fork()) <0)      // ERROR OF FORK - PLEASE THROW ERROR
                                  {
                                      // Throw Error
                                      perror("Error: Fork Error - please restart the Programm\n");
                                      // ONLY SON Continues to Communicate with Clients
                                      close(fileDesc);
                                      // Continue the While
                                      continue;
                                  }
                                  else if (pid5 > 0)           // PARENT PROCESS - ONLY SERVER MAIN FLOW
                                  {
                                    // ONLY SON Continues to Communicate with Clients
                                    close(fileDesc);
                                    // ADD +1 to Counter-Var
                                    counter++;
                                  }
                                  else if (pid5 == 0)          // CHILD PROCESS - ALL CLIENTS ARE HERE
                                  {
                                    // Sixth Fork:
                                      int pid6;  // pid equals Process ID
                                      // ADD +1 to Counter-Var
                                      counter++;
                                      // CREATE FORK
                                          if ((pid6 = fork()) <0)      // ERROR OF FORK - PLEASE THROW ERROR
                                          {
                                              // Throw Error
                                              perror("Error: Fork Error - please restart the Programm\n");
                                          }
                                          else if (pid6 > 0)           // PARENT PROCESS - CLIENT SERVER COMMUNICATION?
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
                                                        snprintf(buf, sizeof buf, "Temperatur: %f\n", getTempre(TEMPHUMPORT));
                                                        send (fileDesc, buf, strlen(buf), 0);
                                                    } else if(strcmp(args[1], "HUMIDITYURE") == 0){
                                                        snprintf(buf, sizeof buf, "Humidity: %f\n", getTempre(TEMPHUMPORT));
                                                        send (fileDesc, buf, strlen(buf), 0);
                                                    }else if(strcmp(args[1], "MOTIONONURE") == 0){
                                                        char* response;
                                                        if(getBewegung(MOTIONPORT)==1){ response = "DETECTED";}
                                                        else {response = "NONE";}
                                                        snprintf(buf, sizeof buf, "Motiondetected: %s\n", response);
                                                        send (fileDesc, buf, strlen(buf), 0);
                                                    }else if(strcmp(args[1], "SOUNDNONURE") == 0){
                                                        snprintf(buf, sizeof buf, "Sound: %f\n", getGerausch(SOUNDPORT));
                                                        send (fileDesc, buf, strlen(buf), 0);
                                                    }else if(strcmp(args[1], "COLISIONURE") == 0){
                                                        char* response;
                                                        if(getColision(COLISIONPORT)==1){ response = "DETECTED";}
                                                        else {response = "NONE";}
                                                        snprintf(buf, sizeof buf, "Colisiondetected: %s\n", response);
                                                        send (fileDesc, buf, strlen(buf), 0);
                                                    }else if(strcmp(args[1], "MOISTUREURE") == 0){
                                                        snprintf(buf, sizeof buf, "Moisture %d\n", getWasserkontakt(MOISTUREPORT));
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
                                            } else if (strcmp(args[0], "PEERS") == 0) {
                                                  int i=1;
                                                  while(connectedClients[i].isNotEmpty){
                                                    printf("<%s> [Connect since: %s]\n",connectedClients[i].ipdesclient,connectedClients[i].connectionEstablished);

                                                    printf("<%s> [Min-Temp: %f]\n",connectedClients[i].ipdesclient,connectedClients[i].minTemp);
                                                    printf("<%s> [ak-Temp: %f]\n",connectedClients[i].ipdesclient,connectedClients[i].akTemp);
                                                    printf("<%s> [Max-Temp: %f]\n",connectedClients[i].ipdesclient,connectedClients[i].maxTemp);

                                                    printf("<%s> [Min-Hum: %f]\n",connectedClients[i].ipdesclient,connectedClients[i].minHum);
                                                    printf("<%s> [ak-Hum: %f]\n",connectedClients[i].ipdesclient,connectedClients[i].akHum);
                                                    printf("<%s> [Max-Hum: %f]\n",connectedClients[i].ipdesclient,connectedClients[i].maxHum);

                                                    printf("<%s> [Min-DB: %f]\n",connectedClients[i].ipdesclient,connectedClients[i].minDB);
                                                    printf("<%s> [ak-DB: %f]\n",connectedClients[i].ipdesclient,connectedClients[i].akDB);
                                                    printf("<%s> [Max-DB: %f]\n",connectedClients[i].ipdesclient,connectedClients[i].maxDB);

                                                    printf("<%s> [Min-Water: %d]\n",connectedClients[i].ipdesclient,connectedClients[i].minWater);
                                                    printf("<%s> [ak-Water: %d]\n",connectedClients[i].ipdesclient,connectedClients[i].akWater);
                                                    printf("<%s> [Max-Water: %d]\n",connectedClients[i].ipdesclient,connectedClients[i].maxWater);

                                                    printf("<%s> [Motiondetected: %d]\n",connectedClients[i].ipdesclient,connectedClients[i].motion);
                                                    printf("<%s> [Colisiondetected: %d]\n\n",connectedClients[i].ipdesclient,connectedClients[i].colision);
                                                    i++;
                                                  }
                                                  continue;
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
                                          else if (pid6 == 0)          // CHILD PROCESS - CHECKING FOR UPDATES
                                          {
                                            //TODO SEMAPHORE REED FEEDER FOR MY TYPE OF CLIENT
                                            //NOTIFY MAJOR EVENTS
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
                          else if (pid3 == 0)          // CHILD PROCESS - LCD INTIALIZE
                          {
                            //TODO LOOP FOR OUTPUT TO LCD (PRIORIZED)

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
                  else if (pid2 == 0)          // CHILD PROCESS - CLIENT INTIALIZE
                  {
                    //TODO MAKE SERVER TO CLIENT IF CONNECTION IS SUCCSESSFUL
                    int cougar;
                    while(1){
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
                            connectedClients[nextFreeEntryPlace].isNotEmpty = 1;
                            connectedClients[nextFreeEntryPlace].ipdesclient = inet_ntoa(client_addr.sin_addr);
                            connectedClients[nextFreeEntryPlace].connectionEstablished = ctime(time(time_t t));
                            cougar = nextFreeEntryPlace;
                            nextFreeEntryPlace++;
                    // Fourth Fork:
                      int pid4;  // pid equals Process ID
                      // CREATE FORK
                          if ((pid4 = fork()) <0)      // ERROR OF FORK - PLEASE THROW ERROR
                          {
                              // Throw Error
                              perror("Error: Fork Error - please restart the Programm\n");
                          }
                          else if (pid4 > 0)           // PARENT PROCESS - NORMAL CLIENT STUFF
                          {
                            //TODO NOTIFY? IF MASSIVE CHANGE WAS NOTICED?
                            //TODO WRITE LOG
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
                                int tag = 1;

                                struct message nachricht;
                                while(tag>0){

                                  tag=msgrcv(feeder,&nachricht,sizeof(&nachricht),sizeof(connectedClients[cougar]),0);
                                  if (tag < 0) {
                                    perror( strerror(errno) );
                                    printf("msgrcv failed, rc=%d\n", tag);
                                    exit(1);
                                  }
                                  printf("%s\n", nachricht.content);
                                  send (client_socket, nachricht.content, strlen(nachricht.content), 0);

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
                          else if (pid4 == 0)          // CHILD PROCESS - AUTOMIZED CLIENT STUFF
                          {
                            //TODO ASK ALL 10 Sek. for New Sensordata
                            //TODO LOOP AND RECV MESSAGE AND MAKE SENSE FROM IT
                            //TODO WRITE LOG
                            while(1){
                              if (semID >= 0) {
                                /* Bereite die Semaphore vor und starte */
                                sema.sem_num = 0;
                                sema.sem_flg = SEM_UNDO;
                                sema.sem_op  = -1;
                                if (-1==semop(semID, &sema, 1)) {
                                    /* Fehler */
                                    perror("semop");
                                }
                              char buff[256];
                              snprintf(buff, sizeof buff, "GET TEMPERATURE \n");
                              send (client_socket, buff, strlen(buff), 0);
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
                              char buff[256];
                              snprintf(buff, sizeof buff, "GET HUMIDITYURE \n");
                              send (client_socket, buff, strlen(buff), 0);
                              //Empfangen von Daten
                              char server_antwort[256];
                              recv(client_socket, &server_antwort, sizeof(server_antwort), 0);
                              char *informationen[1024];
                              strtoken(server_antwort, informationen,2);
                              else if(informationen[0]=="Humidity"){
                                connectedClients[runner].meineSensorwerte.akHum = atof(informationen[1]);
                                if(connectedClients[runner].meineSensorwerte.minHum>atof(informationen[1]))
                                  connectedClients[runner].meineSensorwerte.minHum = atof(informationen[1]);
                                else if(connectedClients[runner].meineSensorwerte.maxHum<atof(informationen[1]))
                                  connectedClients[runner].meineSensorwerte.maxHum = atof(informationen[1]);
                              }
                              char buff[256];
                              snprintf(buff, sizeof buff, "GET SOUNDNONURE \n");
                              send (client_socket, buff, strlen(buff), 0);
                              //Empfangen von Daten
                              char server_antwort[256];
                              recv(client_socket, &server_antwort, sizeof(server_antwort), 0);
                              char *informationen[1024];
                              strtoken(server_antwort, informationen,2);
                              else if(informationen[0]=="Sound"){
                                connectedClients[runner].meineSensorwerte.akDB = atof(informationen[1]);
                                if(connectedClients[runner].meineSensorwerte.minDB>atof(informationen[1]))
                                  connectedClients[runner].meineSensorwerte.minDB = atof(informationen[1]);
                                else if(connectedClients[runner].meineSensorwerte.maxDB<atof(informationen[1]))
                                  connectedClients[runner].meineSensorwerte.maxDB = atof(informationen[1]);
                              }
                              char buff[256];
                              snprintf(buff, sizeof buff, "GET MOISTUREURE \n");
                              send (client_socket, buff, strlen(buff), 0);
                              //Empfangen von Daten
                              char server_antwort[256];
                              recv(client_socket, &server_antwort, sizeof(server_antwort), 0);
                              char *informationen[1024];
                              strtoken(server_antwort, informationen,2);
                              else if(informationen[0]=="Moisture"){
                                connectedClients[runner].meineSensorwerte.akWater =  atoi(informationen[1]);
                                if(connectedClients[runner].meineSensorwerte.minWater>atoi(informationen[1]))
                                  connectedClients[runner].meineSensorwerte.minWater = atoi(informationen[1]);
                                else if(connectedClients[runner].meineSensorwerte.maxWater<atoi(informationen[1]))
                                  connectedClients[runner].meineSensorwerte.maxWater = atoi(informationen[1]);
                              }
                              char buff[256];
                              snprintf(buff, sizeof buff, "GET MOTIONONURE \n");
                              send (client_socket, buff, strlen(buff), 0);
                              //Empfangen von Daten
                              char server_antwort[256];
                              recv(client_socket, &server_antwort, sizeof(server_antwort), 0);
                              char *informationen[1024];
                              strtoken(server_antwort, informationen,2);
                              else if(informationen[0]=="Motiondetected"){
                                connectedClients[runner].meineSensorwerte.motion = atoi(informationen[1]);
                              }
                              char buff[256];
                              snprintf(buff, sizeof buff, "GET COLISIONURE \n");
                              send (client_socket, buff, strlen(buff), 0);
                              //Empfangen von Daten
                              char server_antwort[256];
                              recv(client_socket, &server_antwort, sizeof(server_antwort), 0);
                              char *informationen[1024];
                              strtoken(server_antwort, informationen,2);
                              else if(informationen[0]=="Colisiondetected"){
                                connectedClients[runner].meineSensorwerte.colision = atoi(informationen[1]);
                              }
                              else {
                                //do nothing at all
                              }
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
                }
          }
          else if (pid == 0)          // CHILD PROCESS - FEEDER INITIALIZE
          {
            //TODO  LOOP FILL FEEDER AND LCD QUEUE make types
            //Bewegungsmelder, Erschütterung, Temperaturschwankung --> LCD UND PEERS benachrichtigen
            while(1){
              if (semID >= 0) {
              sema.sem_num = 0;
              sema.sem_flg = SEM_UNDO;
              sema.sem_op  = -1;
              if (-1==semop(semID, &sema, 1)) {
                  /* Fehler */
                  perror("semop");
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
                neueLCDNachricht.mstype = (long) MIDDLEPRIO;
                sprintf(neueLCDNachricht.content,"Sound: %f\n",getGerausch(SOUNDPORT));
                msgsnd(lcdfeeder, &neueLCDNachricht, sizeof(neueLCDNachricht), 0);
              }
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
