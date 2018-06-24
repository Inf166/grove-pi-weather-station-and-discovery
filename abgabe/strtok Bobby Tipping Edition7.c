#include <stdio.h>





 char * strtokBobbyTippingEdition(char input[], int num) {
 	
 	
 	char worda[8];
 	char wordb[8];
 	char wordc[8];
 
 	 
 	int s=0
 	int i=0;
 	int j=0;
 	
 	while (s==0) {
 		
 		worda[j]=input[i];
 		i++;
 		j++;
 		if (input[i]==' ') {
 			i++;
 			j=0;
 			s++;
		 }
	}
		 
	while (s==1) {
 	
 		wordb[j]=input[i];
 		i++;
 		j++;
 		if (input[i]==' ') {
 			i++;
 			j=0;
 			s++;
		 }
 		
	 }
	 
	while (s==2) {
 	
 		wordc[j]=input[i];
 			i++;
 			j++;
 		if (input[i]=='\0') {
 			s++;
		 }
 		
	 }
	 
	 switch(num) {
	 	case 0: return worda;
	 	case 1: return wordb;
	 	case 2: return wordc;
	 	default: return "Für den zweiten Parameter ist nur 0-2 gültig";
	 }
 	
 	
 	
 }




int main() {
char in[32]= "CONNECT 123.132.123 1534";	

	printf ("Befehl: %s, IP: %s, Port: %s", strtokBobbyTippingEdition(in, 0), strtokBobbyTippingEdition(in, 1), strtokBobbyTippingEdition(in, 2));


return 0;
}
