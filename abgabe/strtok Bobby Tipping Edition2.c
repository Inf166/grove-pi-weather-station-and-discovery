#include <stdio.h>


char in[255]= "CONNECT 255.255.255 1234";

 char * strtokBobbyTippingEdition(char input[], int num) {
 	
 	char word1[255];
 	char word2[255];
 	char word3[255];
 	 
 	 
 	int i;
 	
 	while (1) {
 		i++;
 		word1[i]=input[i];
 		if (input[i+1]=" ") {
 			i++;
 			break;
		 }
		 
	while (1) {
 		i++;
 		word2[i]=input[i];
 		if (input[i+1]=" ") {
 			i++;
 			break;
		 }
 		
	 }
	 
	while (1) {
 		i++;
 		word3[i]=input[i];
 		if (input[i+1]=" ") {
 			break;
		 }
 		
	 }
	 
	 switch(num) {
	 	case 0: return word1;
	 	case 1: return word2;
	 	case 2: return word3;
	 	default: "Für den zweiten Parameter ist nur 0-2 gültig";
	 }
 	
 	
 	
 }




int main() {
	
	printf ("Befehl: %s, IP: %s, Port: %s", strtokBobbyTippingEdition(in, 0), strtokBobbyTippingEdition(in, 1), strtokBobbyTippingEdition(in, 2));



return 0;
}
