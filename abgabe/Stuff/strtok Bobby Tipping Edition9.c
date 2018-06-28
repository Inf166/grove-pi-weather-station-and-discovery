#include <stdio.h>


	char token_a[255]; //Tokens m�ssen au�erhalb der Funktion Deklariert werden, weil es mit "lokalem Speicher" nicht geht
	char token_b[255];
 	char token_c[255];


 char * strtokBobbyTippingEdition(char input[], int num) {
 	
	  
 	int t=0; //F�r die Whileschleifen damit die Wissen welcher Token dran ist, 0=A, 1=B, 2=C, 3=Fertig
 	int i=0; //Iterator f�r jeden char des Input Char Arrays
 	int j=0; // Iterator f�r jeden Char des jeweiligen Tokens, wird z�r�ck auf 0 gesetzt wenn Token fertig ist
 	
 	printf("Input: %s\n", input); //gibt aus welchen Input die Funktion bekommen hat
 	printf("Read and Set(A): "); //Die chars f�r Token A werden festgelegt
 	while (t==0) { 
 		
 		token_a[j]=input[i];
 		printf("%c=%c, ", j, token_a[j], i, input[i] ); //Anzeige f�r jeweiligen Char
 		i++;
 		j++;
 		if (input[i]==32) { //32 ist der ASCII Code f�r Leerzeichen, es wird \0 gesetzt und es geht weiter zum n�chsten Token
 			token_a[j]='\0';
 			printf("\nToken A has detected Space and sets Zerobyte");
 			i++;
 			j=0;
 			t++;
 			printf("\nin Function: Token A: %s\n", token_a); //Das ist letztendlich Token A
		 }
	}
	printf("Read and Set(B): ");
	while (t==1) {
 	
 		token_b[j]=input[i];
 		printf("%c=%c, ", token_b[j], input[i] );
 		i++;
 		j++;
 		if (input[i]==32) {
 			token_b[j]='\0';
 			printf("\nToken B has detected Space and sets Zerobyte");
 			i++;
 			j=0;
 			t++;
 			printf("\nin Function: Token B: %s\n", token_b);
		 }
		else if (input[i]=='\0') { //falls es nur zwei Tokens gibt wird hier bereits das \0 gesetzt
			token_b[j]='\0';
			printf("\nToken B has detected Zerobyte and sets Zerobyte");
			t+=2;
			printf("\nin Function: Token B: %s\n", token_b);
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
 			printf("\nToken C has detected Zerobyte and sets Zerobyte");
 			t++;
 			printf("\nin Function: Token C: %s\n", token_c);
		 }
 		
	 }
	 
	 switch(num) {
	 	case 0: printf("RETURN: Token A: %s\n\n", token_a); return token_a; break; //der Zweite Parameter num gibt an welcher Token returnt wird
	 	case 1: printf("RETURN: Token B: %s\n\n", token_b);return token_b; break;
	 	case 2: printf("RETURN: Token C: %s\n\n", token_c);return token_c; break;
	 	default: return "F�r den zweiten Parameter ist nur 0-2 g�ltig";
	 }
 	
 	
 	
 }




int main() {
char input_test[255]= "CONNECT 123.132.123 1534";
char in[255];

printf("Befehl eingeben: "); //Eingabe
scanf("%255[^\n]", &in); //das 255 gibt die Maximalgr��e des Strings an, [^\n] macht das der String erst fertig ist wenn Enter gedr�ckt wird, damit Leerzeichen auch drin bleiben
printf("\n");
	
	printf("in Main: Token A: %s\n\n", strtokBobbyTippingEdition(in, 0)); //hier wird ausgegeben ob in der Main Funktion die richtigen Tokens da sind
	printf("in Main: Token B: %s\n\n", strtokBobbyTippingEdition(in, 1));
	printf("in Main: Token C: %s\n\n", strtokBobbyTippingEdition(in, 2));
	
	printf ("Befehl: %s, IP: %s, Port: %s", strtokBobbyTippingEdition(in, 0), strtokBobbyTippingEdition(in, 1), strtokBobbyTippingEdition(in, 2)); //die letztliche Ausgabe

return 0;
}
