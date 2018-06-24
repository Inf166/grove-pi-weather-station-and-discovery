#include <stdio.h>


	char token_a[255];
	char token_b[255];
 	char token_c[255];


 char * strtokBobbyTippingEdition(char input[], int num) {
 	
	  
 	int t=0;
 	int i=0;
 	int j=0;
 	
 	printf("Input: %s\n", input);
 	printf("Read and Set(A): ");
 	while (t==0) {
 		
 		token_a[j]=input[i];
 		printf("(%i)%c=(%i)%c, ", j, token_a[j], i, input[i] );
 		i++;
 		j++;
 		if (input[i]==32) {
 			token_a[j]='\0';
 			printf("\nToken A has detected Space and sets Zerobyte");
 			i++;
 			j=0;
 			t++;
 			printf("\nin Function: Token A: %s\n", token_a);
		 }
	}
	printf("Read and Set(B): ");
	while (t==1) {
 	
 		token_b[j]=input[i];
 		printf("%c=%c, ", token_b[j], input[i] );
 		i++;
 		j++;
 		if (input[i]==' ') {
 			token_b[j]='\0';
 			printf("\nToken B has detected Space and sets Zerobyte");
 			i++;
 			j=0;
 			t++;
 			printf("\nin Function: Token B: %s\n", token_b);
		 }
		else if (input[i]=='\0') {
			token_b[j]='\0';
			printf("\nToken B has detected Zerobyte and sets Zerobyte");
			t+=2;
			printf("in Function: Token B: %s\n", token_b);
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
 			("\n Token B has detected Zerobyte and sets Zerobyte");
 			t++;
 			printf("\nin Function: Token C: %s\n", token_c);
		 }
 		
	 }
	 
	 switch(num) {
	 	case 0: printf("RETURN: Token A: %s\n\n", token_a); return token_a; break;
	 	case 1: printf("RETURN: Token B: %s\n\n", token_b);return token_b; break;
	 	case 2: printf("RETURN: Token C: %s\n\n", token_c);return token_c; break;
	 	default: return "Für den zweiten Parameter ist nur 0-2 gültig";
	 }
 	
 	
 	
 }




int main() {
char input_test[255]= "CONNECT 123.132.123 1534";
char in[255];

printf("Befehl eingeben: ");
scanf("%s", &in);
printf("\n");
	
	printf("in Main: Token A: %s\n", strtokBobbyTippingEdition(in, 0));
	printf("in Main: Token B: %s\n", strtokBobbyTippingEdition(in, 1));
	printf("in Main: Token C: %s\n", strtokBobbyTippingEdition(in, 2));
	
	printf ("Befehl: %s, IP: %s, Port: %s", strtokBobbyTippingEdition(in, 0), strtokBobbyTippingEdition(in, 1), strtokBobbyTippingEdition(in, 2));

return 0;
}
