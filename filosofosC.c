#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define NUMFIL 3				// ----1. "N FILÓSOFOS"----
#define IZQ (i-1)%NUMFIL        // Vecino izquierdo de i
#define DER (i+1)%NUMFIL        // Vecino derecho de i
#define PENSANDO 0
#define CON_HAMBRE 1
#define COME 2

int COMIDA=50000;              
pthread_t filos[NUMFIL];        // Hilos que representan a los filósofos
sem_t mutex;                    // Semáforo para la sección crítica
sem_t s[NUMFIL];                // Semáforos para los filósofos
int estado [NUMFIL];            //estado actual de cada filósosfo

//El filosofo i pensará
void pensar(int i){
  printf("Filosofo %d pensando \n", i);
  estado[i]=PENSANDO;
  sleep(2);
}

//El filosofo i, comerá
void comer(int i){
  printf("Filosofo %d comiendo \n", i);
  estado[i]=COME;	
  sleep(2);
}

//Verifica que pueda tomar ambos tenedores
void verifica(int i){
  if( estado[i]==CON_HAMBRE && estado[IZQ]!=COME && estado[DER]!=COME){
    estado[i] = COME;
    printf(" Filósofo %d puede comer\n", i) ;
    sem_post(&s[i]);
  }
}

//El filosofo i intenta tomar los  tenedores
void toma_tndrs(int i){
  sem_wait(&mutex);             // Entra a la sección crítica, hace uso del semaforo
  estado[i]=CON_HAMBRE;         // El estado del filósofo cambia a hambriento
  verifica(i);                  // Verifica que pueda tomar los tenedores
  sem_post(&mutex);             // Sale de la sección crítica y el sem. puede permitir la entrada a alguien más
  sem_wait(&s[i]);              // Bloquea si no tiene los tenedores
}

//El filosofo i dejará los tenedores
void deja_tndrs(int i){
  sem_wait(&mutex);             // de nuevo entra a la sección critica
  estado[i] = PENSANDO;         //deja de comer y se pone a pensar
  verifica(IZQ);        
  verifica(DER);
  sem_post(&mutex);
}

void *filosofos(int i){
  for(;;){
    pensar(i);                
    toma_tndrs(i);
    comer(i) ;
    deja_tndrs(i) ;
  }
}

main(){
  int i;
  srand (time(NULL));
  // ----INICIA LOS REQUERIMIENROS DE LOS FILÓSOFOS----
  for(i=0; i<NUMFIL; i++){
    sem_init (&s[i], 0, 1);
    estado[i]=PENSANDO;       //Filosofos inician pensando 
  }

  sem_init (&mutex, 0, 1);

  // Filósofos iniciados, con hilos
  for (i=0; i<NUMFIL; i++){
    pthread_create(&filos[i], NULL, (void*)filosofos, (void *)i);
  }
 
  // Los filosofos esperan a que termine otros para iniciar recursos
  for (i=0; i<NUMFIL; i++){
    pthread_join(filos[i],NULL);
  }
  return 0;
}
