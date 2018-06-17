//
// Created by Joel Mai on 17.06.2018.
//
//Include Libaries
#include<stdio.h>
#include<time.h>

int main(){
    //<255.255.255.255:1234> [ Sensor: Wert ]
    time_t now;
	  time(&now);
	  printf("Sekunden seit 01.01.1970 00:00:00 Uhr: %d\n", now);

    FILE *fp;
    int i;

    fp = fopen("abc.txt", "w");

    if(fp == NULL) {
    	printf("Datei konnte nicht geoeffnet werden.\n");
    }else {
    	for(i=0; i<26; i++) {
    		fputc(i+65, fp);
    	}
    	// Zeilenumbruch hinzufuegen
    	fputc(10, fp);
    	fclose(fp);
    }

    FILE *fp;
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
  return 0;
}
