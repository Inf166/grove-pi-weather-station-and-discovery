/* Anlegen der nötigen Structs */
struct sensorwerte meineSensorwerte;
struct clientarray *p_meineSensorwerte;
p_meineSensorwerte = &meineclients;
/* Anlegen des Shared Memory */
key_t shkey = ftok("something", 65);
int shid = shmget(shkey, sizeof(meineSensorwerte), IPC_CREAT | 0777);
p_meineSensorwerte = (int) shmat(shid, NULL, 0);
/* Anlegen der Semaphore */
int semID;
struct sembuf sema;
semID = semget(2404, 1, IPC_CREAT | 0666);
/* Beginne Loop */
int pid;
if((pid1 = fork()) < 0)	//ERROR
{
  perror("Error: Fork Error - please restart the Programm\n");// Throw Error
}
if(pid1 > 0)		//PARENT PROCESS - Schreibe Struct alle 10 Sekunden
{
	while(1){
  	if (semID >= 0) {
  	/* Bereite die Semaphore vor und starte */
    sema.sem_num = 0;
    sema.sem_flg = SEM_UNDO;
    sema.sem_op = -1;
      if (-1 == semop(semID, &sema, 1)) {
          /* Fehler */
          perror("semop");
      }
  	meineSensorwerte.wasser = getWasserkontakt();
    meineSensorwerte.colision = getColision();
    meineSensorwerte.humidity = getHumidity();
    meineSensorwerte.temperature = getTempre();
    meineSensorwerte.sound = getGerausch();
    meineSensorwerte.bewegung = getBewegung();
    /* Gebe wieder frei*/
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
if(pid1 == 0)		//CHILD PROCESS - Gebe Struct alle 10 Sekunden neu aus
{
  while(1){
  	if (semID >= 0) {
  	/* Bereite die Semaphore vor und starte */
    sema.sem_num = 0;
    sema.sem_flg = SEM_UNDO;
    sema.sem_op = -1;
      if (-1 == semop(semID, &sema, 1)) {
          /* Fehler */
          perror("semop");
      }
      printf("Sensorwerte:\n");
      printf("Kontakt mit Wasser: %d\n", meineSensorwerte.wasser);
      printf("Zusammenstoss festgestellt: %d\n", meineSensorwerte.colision);
      printf("Bewegung festgestellt: %d\n", meineSensorwerte.bewegung);
      printf("Luftfeuchtigkeit: %f\n", meineSensorwerte.humidity);
  	  printf("Temperatur: %f\n", meineSensorwerte.temperature);
      printf("Dezibil: %d\n", meineSensorwerte.sound);
      
    /* Gebe wieder frei*/
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
