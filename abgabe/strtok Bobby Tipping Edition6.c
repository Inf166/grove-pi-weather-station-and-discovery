#include <stdio.h>





 char * strtokBobbyTippingEdition(char *input[], int num) {
 	
 	
 	char worda[8];
 	char wordb[8];
 	char wordc[8];
 
 	 
 	 
 	int i=0;
 	int j=0;
 	
 	while (1) {
 		
 		worda[j]=input[i];
 		i++;
 		j++;
 		if (input[i]==' ') {
 			i++;
 			j=0;
 			break;
		 }
	}
		 
	while (1) {
 	
 		wordb[j]=input[i];
 		i++;
 		j++;
 		if (input[i]==' ') {
 			i++;
 			j=0;
 			break;
		 }
 		
	 }
	 
	while (1) {
 	
 		wordc[j]=input[i];
 			i++;
 			j++;
 		if (input[i]=='\0') {
 			break;
		 }
 		
	 }
	 
	 switch(num) {
	 	case 0: return worda;
	 	case 1: return wordb;
	 	case 2: return wordc;
	 	default: return "F�r den zweiten Parameter ist nur 0-2 g�ltig";
	 }
 	
 	
 	
 }




int main() {
char in[32]= "CONNECT 123.132.123 1534";	

	printf ("Befehl: %s, IP: %s, Port: %s", strtokBobbyTippingEdition(in, 0), strtokBobbyTippingEdition(in, 1), strtokBobbyTippingEdition(in, 2));


return 0;
}
