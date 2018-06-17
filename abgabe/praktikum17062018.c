//
// Created by Joel Mai on 17.06.2018.
//
//Include Libaries
#include<stdio.h>
#include<time.h>

int main(){

	/*char kleinerAls = 60;
    char groesserAls = 62;

    char punkt = 46 ;
    char doppelpunkt = 58;
    char klammerAuf = 91 ;
    char klammerZu = 93;
    char leerzeichen = 32 ;



    char Colision[255] = "COLLISION";
    char Temperatur[255] = "TEMPERATUR";
    char Feuchtigkeit[255] = "FEUCHTIGKEIT";
    char Geraeusch[255] = "GERAEUSCH";
    char Bewegung[255] = "BEWEGUNG";
    char Wasserkontakt[255] = "WASSERKONTAKT";

    char Max[255] = "Max";
    char Min[255] = "Min: ";
    char Current[255] = "Derzeitig: ";*/

    //<255.255.255.255:1234> [ Sensor: Wert ]

    int getColision = 1;

		int getTempre = 20;
    int getTempreMax = 25;
    int getTempreMin = 15;

    int getFeuchtigkeit = 60;
    int getFeuchtigkeitMax = 70;
    int getFeuchtigkeitMin = 40;

    int getGerausch = 50;
    int getGerauschMax = 80;
    int getGerauschMin =0;

    int getBewegung = 1;

    int getWasserkontakt = 5;
    int getWasserkontaktMax = 9;
    int getWasserkontaktMin = 1;

    char IPColision[255] = "255.255.255.255:1234";
    char IPTempre[255] = "255.255.255.255:1234";
		char IPFeuchtigkeit[255] = "255.255.255.255:1234";
		char IPGerausch[255] = "255.255.255.255:1234";
    char IPBewegung[255] = "255.255.255.255:1234";
    char IPWasserkontakt[255] = "255.255.255.255:1234";


   /* time_t now;
	  time(&now);
	  printf("Sekunden seit 01.01.1970 00:00:00 Uhr: %d\n", now);*/

    FILE *fp;

    fp = fopen("Sensordaten.txt", "w");

    if(fp == NULL) {
    	printf(" Datei? Was fuer 'ne Datei?'.\n");
    }else {
    	//<255.255.255.255:1234> [ Sensor: min: xx; derzeitig: xx, max: xx ]

		fprintf(fp, "<%s> [ COLLISION: derzeitig: %d ]\n", IPColision, getColision );
		fprintf(fp, "<%s> [ TEMPERATUR: min: %d, derzeitig: %d, max: %d ]\n", IPTempre, getTempreMin, getTempre, getTempreMax );
		fprintf(fp, "<%s> [ FEUCHTIGKEIT: min: %d, derzeitig: %d, max: %d ]\n", IPFeuchtigkeit, getFeuchtigkeitMin, getFeuchtigkeit, getFeuchtigkeitMax );
		fprintf(fp, "<%s> [ GERAEUSCH: min: %d, derzeitig: %d, max: %d ]\n", IPGerausch, getGerauschMin, getGerausch, getGerauschMax );
		fprintf(fp, "<%s> [ BEWEGUNG: derzeitig: %d ]\n", IPBewegung, getBewegung );
		fprintf(fp, "<%s> [ WASSER: min: %d, derzeitig: %d, max: %d ]\n", IPWasserkontakt, getWasserkontaktMin, getWasserkontakt, getWasserkontaktMax );

	printf("*DING* Tabelle is' feddich.\n");
	fclose(fp);


    }


}

	 /* FILE *fp;
    int i, temp;

    fp = fopen("abc.txt", "r");

    if(fp == NULL) {
    	printf("Datei konnte nicht geoeffnet werden.\n");
    }else {
    	// komplette Datei zeichenweise ausgeben
    	while((temp = fgetc(fp))!=EOF) {
    		printf("%c ", temp);
    	}
    	fclose(fp);
    }

    printf("\n");
  return 0;*/

	/*	fputc(kleinerAls, fp);  					//<

		for(i=0; i<255; i++) {						//255.255.255.255:123
		if (IPColision[i]== "\0") {
    			break;
    		fputc(IPColision[i], fp);

			}
    	}
    	fputc(groesserAls, fp);  					//>
    	fputc(leerzeichen, fp);  					//leerzeichen halt
    	fputc(klammerAuf, fp);  					//[
    	fputc(leerzeichen, fp);						//leerzeichen halt

    	for(i=0; i<255; i++) {						//COLLISON:
    	if (Colision[i]== "\0") {
    			break;
			}
    		fputc(Colision[i], fp);

    	}

    	for(i=0; i<255; i++) {						//Min:
    	if (Colision[i]== "\0") {
    			break;
			}
    		fputc(Colision[i], fp);

    	}

    	// Zeilenumbruch hinzufuegen
    	fputc(10, fp);
    	fclose(fp);*/

    	//<255.255.255.255:1234> [ Sensor: Wert ]
