#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#define  NUMFIL 5

//gcc filosofos.cpp -pthread

pthread_mutex_t cubiertos[5];
int aux=0;
void tomarCubierto(int p1, int p2);
void soltarCubierto(int p1, int p2);
void comer(int arg);
void *filosofo(void *a);

int main(void){
	printf("FILOSOFOS\n");
	pthread_t filosofos[NUMFIL];
	int ids[NUMFIL];
	printf("Auxiliar = %d\n", aux);
	for(int i=0; i<NUMFIL; i++){
		pthread_mutex_init(&cubiertos[i], NULL);
	}
	for(int i=0; i<NUMFIL; i++){
		ids[i]=i+1;
		pthread_create(&filosofos[i], NULL, &filosofo, &ids[i]);
	}	
	for(int i=0; i<NUMFIL; i++){
		pthread_join(filosofos[i], NULL);
	}
	printf("Auxiliar = %d\n");
	return 0;
}

void soltarCubierto(int p1, int p2){
	pthread_mutex_unlock(&cubiertos[p1]);
	pthread_mutex_unlock(&cubiertos[p2]);
}

void tomarCubierto(int p1, int p2){
	pthread_mutex_lock(&cubiertos[p2]);
	printf("Filosofo %d tomo cubierto %d\n", p1, p2);
}

void comer(int a){
	int cubierto1=a+1;
	int cubierto2=a-2;
	if(cubierto2==-1){
		cubierto2=4;
	}
	tomarCubierto(a, cubierto1);
	tomarCubierto(a, cubierto2);
	printf("Filosofo %d comiendo \n", a);
	int random = rand()%5;
	sleep(random);
	soltarCubierto(cubierto1, cubierto2);
}

void *filosofo(void *arg){
	int arg2=*((int*)arg);
	for(int i=0; true; i++){
		aux=aux+1;
		comer(arg2);
	}
	return NULL;
}
