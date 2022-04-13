#include<stdio.h>

void filosofo(char *arg);

int main(void){
	printf("FILOSOFOS\n");
	char fil1[20]= "Platon";
	char fil2[20]= "Pitagoras";

	filosofo(fil1);
	filosofo(fil2);

	return 0;
}

void filosofo(char *arg){
	for(int i=0; i<5; i++){
		printf("%s estoy comiendo : %d \n", arg, i);
	}
}
