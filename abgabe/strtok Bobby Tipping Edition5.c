#include <stdio.h>




 char * strtokBobbyTippingEdition(char input[], int num) {
 	
 	char word1[255];
 	char word2[1];
 	char word3[100];
 	 
 	 
 	int i=0;
 	int j=0;
 	
 	while (1) {
 		
 		word1[j]=input[i];
 		i++;
 		j++;
 		if (input[i]==32) {
 			i++;
 			j=0;
 			break;
		 }
	}
		 
	while (1) {
 	
 		word2[j]=input[i];
 		i++;
 		j++;
 		if (input[i]==32) {
 			i++;
 			j=0;
 			break;
		 }
 		
	 }
	 
	while (1) {
 	
 		word3[j]=input[i];
 			i++;
 			j++;
 		if (input[i]==32) {
 			break;
		 }
 		
	 }
	 
	 switch(num) {
	 	case 0: return word1;
	 	case 1: return word2;
	 	case 2: return word3;
	 	default: return "Für den zweiten Parameter ist nur 0-2 gültig";
	 }
 	
 	
 	
 }




int main() {
char in[255]= "CONNECT 123.132.123 1534";	

	printf ("Ja\n");
	printf ("Befehl: %s, IP: %s, Port: %s", strtokBobbyTippingEdition(in, 0), strtokBobbyTippingEdition(in, 1), strtokBobbyTippingEdition(in, 2));



return 0;
}
