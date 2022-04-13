#include<stdio.h>
#include<pthread.h>

//gcc filosofos.cpp -pthread

#define NUM_FILOSOFOS 5

void *filosofo(void *arg);

int variable=0;

int main(void){
	printf("FILOSOFOS\n");
	printf("Variable=%d/n", variable);

	pthread_t filosofos[NUM_FILOSOFOS];
	int ids[NUM_FILOSOFOS];
	
	for(int i=0; i<NUM_FILOSOFOS; i++){
		ids[i]=i+1;
		pthread_create(&filosofos[i], NULL, &filosofo, &ids[i]);
	}	
	for(int i=0; i<NUM_FILOSOFOS; i++){
		pthread_join(filosofos[i], NULL);
	}

	printf("Variable%d/n", variable);

	return 0;
}

void *filosofo(void *arg){
	int arg2=*((int*)arg);
	for(int i=0; i<5; i++){
		variable=variable+1;
		printf("Filosofo-%d estoy comiendo : %d. variable=%d \n", arg2, i, variable);
	}
	return NULL;
}


