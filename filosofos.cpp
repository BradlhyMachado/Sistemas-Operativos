#include<stdio.h>
#include<pthread.h>

//gcc filosofos.cpp -pthread

void *filosofo(void *arg);

int main(void){
	printf("FILOSOFOS\n");

	char fil1[20]= "Platon";
	char fil2[20]= "Pitagoras";

	pthread_t platon, pitagoras;

	pthread_create(&platon, NULL, &filosofo, &fil1);
	pthread_create(&pitagoras, NULL, &filosofo, &fil2);	
	
	pthread_join(platon, NULL);
	pthread_join(pitagoras, NULL);

	return 0;
}

void *filosofo(void *arg){
	char *arg2=(char*)arg;
	for(int i=0; true; i++){
		printf("%s estoy comiendo : %d \n", arg2, i);
	}
	return NULL;
} 
