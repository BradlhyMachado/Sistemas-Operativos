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

int COMIDA=50000;				// ----2. RECURSO COMIDA 2147483647----
pthread_t filos[NUMFIL];        // Hilos que representan a los filósofos
sem_t mutex;                    // Semáforo para la sección crítica
sem_t s[NUMFIL];                // Semáforos para los filósofos
int estado[NUMFIL];             //estado actual de cada filósosfo
int estomagos[NUMFIL];			// ----3. ESTOMAGOS PARA CADA FILOSOFO----
int estMax=20000;				// ----3. Rango cercano a lo máximo del estomago----
int contadorComida=0;			// ----2. Registro de restauracion de comida----
int totalComido=0;				// ----7. Registro de recurso Comida----
int pensando=0;					// ----7. Registro de total pensado----
int comido=0;					// ----7. Registro de total comido----

//El filosofo i pensará
void pensar(int i){
  int gasto=rand() % 2001;
  estomagos[i]=estomagos[i]-gasto;			// ----6. DESGASTO DEL RECURSO COMIDA AL PENSAR----
  pensando=pensando+gasto;
  printf(" Filosofo %d pensando %d, estomago en %d \n", i, gasto, estomagos[i]);
  gasto=0;
  estado[i]=PENSANDO;
  sleep(2);
}

//El filosofo i, comerá
void comer(int i){
  bool estadoint=true;
  int xComer=(rand() % 10001);
  
  //////////////////////
  //printf("i=%d ",nom);
  //////////////////////
  
  //Pensar por que el filosofo comio mas de lo que puede
  if(estomagos[i]>=estMax){
    printf(" ----Filosofo %d comio %d, mas de lo que debe %d---- \n",i , estomagos[i],estMax);
    estadoint=false;
    pensar(i);
  }
  if(estadoint==true){
    int ii=i; ////////////////////////////////////
	if(xComer<COMIDA){
      estomagos[i]=estomagos[i]+xComer;
      COMIDA=COMIDA-xComer;
      totalComido=totalComido+xComer;
      printf(" Filosofo %i comio %d, estomago en %d \n"),ii , xComer, estomagos[i];
    }
    else{
      xComer=COMIDA;
      estomagos[i]=estomagos[i]+xComer;
      COMIDA=COMIDA-xComer;
      totalComido=totalComido+xComer;
      printf(" Filosofo %i comio %d, estomago en %d \n",ii , xComer, estomagos[i]);
      COMIDA=2147483647;		//Restaurando COMIDA
      contadorComida++;			//Registro de restauracion de comida
      printf("----Recurso Comida restaurada %d---- \n", contadorComida);
      // ----7. Demostrar Registo----
      printf("----Total Comido %d---- \n", totalComido);
      printf("----Total Pensado %d---- \n", pensando);
      printf("----Total Estómagos %d---- \n", estomagos[0]+estomagos[1]+estomagos[2]);  
    }
  }
  xComer=0; 
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
  int i, f, g;
  srand (time(NULL));
  // ----INICIA LOS REQUERIMIENROS DE LOS FILÓSOFOS----
  for(i=0; i<NUMFIL; i++){
    sem_init (&s[i], 0, 1);
    estado[i]=PENSANDO;			 //Filosofos inician pensando
    estomagos[i]=0;				 //Estomagos inician en 0
  }
  // ----5. CONSIDERAR DEAD LOCKS----
  sem_init (&mutex, 0, 1);

  // Filósofos iniciados, con hilos
  for (f=0; f<NUMFIL; f++){
    pthread_create(&filos[f], NULL, (void*)filosofos, (void*)f);
  }
 
  // Los filosofos esperan a que termine otros para iniciar recursos
  for (g=0; g<NUMFIL; g++){
    pthread_join(filos[g],NULL);
  }
  return 0;
}
