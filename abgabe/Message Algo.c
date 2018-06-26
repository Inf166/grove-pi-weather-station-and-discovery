#include <stdio.h>

int main() {
	
	
	char kleinerals=60;
    char groesserals=62;
    char leerzeichen=32;
    char klammerauf=91;
	char klammerzu=93;
	char doppelpunkt=58;
	
	int tempmin= 0;
	int tempderz= 18;
	int tempmax= 26; 
	
	char ausgabe[255];
	
	char kleinera[1]="<";
	char groessera[1]=">";
	
	char ip[11]="125.125.125";
	char sensor[11]=" [ Sensor: ";
	char min[5]="min: ";
	char derzeitig[13]=", derzeitig: ";
	char max[7]=", max: ";
	char klammerz[2]= " ]";
	
	char temperatur[12]="TEMPERATUR: "; 
	
	sprintf(ausgabe, "<%s> [ Sensor: %s: min: %d, derzeitig: %d, max: %d ]", ip, temperatur, tempmin, tempderz, tempmax);
	printf(ausgabe);
	
	
	
	//<255.255.255> [ Sensor: TEMP: min: xx, derzeitig: xx, max: xx ]
	
	
	return 0;
}
