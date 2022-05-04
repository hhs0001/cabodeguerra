#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <semaphore.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#define NUMCONS		2
#define NUMPROD		2
#define BUFFERSIZE	1000

//int sorteio(int MIN, int MAX){
//	srand48(time(0));
//	int n = (int) (drand48()*(MAX - MIN + 1)) + MIN;
//	return n;
//}

pthread_t cons[NUMCONS];
pthread_t prod[NUMPROD];
int buffer[BUFFERSIZE];
int currentidx;
pthread_mutex_t buffer_mutex;
sem_t buffer_full, buffer_empty;
void *consumidor(void *arg);
void *produtor(void *arg);

//declaração das variaveis globais
int SEU_RA = 202010655;
char primiraLetra[1] = "H";
char segundaLetra[1] = "H";
char terceiraLetra[1] = "S";

char cabodeguerra[87][1];

int primeiraLetraPOS = 42;
int ultimaLetraPOS = 44;
int numerocontrole = 43;

//função que verifica se a letra esta na primeira posição ou se a letra esta na ultima posição

int verificador(){
	if (numerocontrole == 1){
		return 1;
	}
	if (numerocontrole == 86){
		return 1;
	}
	return 0;
}



//limpar tela conio.h não funciona

void clrscr()
{
   system("@cls||clear");
}

//andar para a esquerda com o cabodeguerra

int andarParaEsquerda() {
	strcpy(cabodeguerra[numerocontrole], terceiraLetra);
	strcpy(cabodeguerra[numerocontrole + 1], "_");
	strcpy(cabodeguerra[numerocontrole - 1], segundaLetra);
	strcpy(cabodeguerra[numerocontrole - 2], primiraLetra);
	numerocontrole--;
	return 0;
}

//andar para a direita com o cabodeguerra

int andarParaDireita() {
	strcpy(cabodeguerra[numerocontrole], primiraLetra);
	strcpy(cabodeguerra[numerocontrole + 1], segundaLetra);
	strcpy(cabodeguerra[numerocontrole + 2], terceiraLetra);
	strcpy(cabodeguerra[numerocontrole - 1], "_");
	numerocontrole++;
	return 0;
}

int main(int argc, char **argv) {
	for(int contador = 0; contador <= 87; contador++)
  	{
		if (contador == 42)
		{
			strcpy(cabodeguerra[contador], primiraLetra);
		}
		else if (contador == 43)
		{
			strcpy(cabodeguerra[contador], segundaLetra);
		}
		else if (contador == 44)
		{
			strcpy(cabodeguerra[contador], terceiraLetra);
		}
		else{
			strcpy(cabodeguerra[contador], "_");
		}
	}
	int i; 
	time_t t;
	srand48(time(&t));
	currentidx = 0;
	pthread_mutex_init(&buffer_mutex, NULL);
	sem_init(&buffer_full, 0, 0);
	sem_init(&buffer_empty, 0, 0);
	for(i = 0; i < NUMCONS; i++) {
		pthread_create(&(cons[i]), NULL, consumidor, NULL);
	}
	for(i = 0; i < NUMPROD; i++) {
		pthread_create(&(prod[i]), NULL, produtor, NULL);
	}
	for(i = 0; i < NUMCONS; i++) {
		pthread_join(cons[i], NULL);
	}
	for(i = 0; i < NUMPROD; i++) {
		pthread_join(prod[i], NULL);
	}
}

void *produtor(void *arg) {
	int n;
	while(1) {
		n = (int) (drand48() * 1000.0);
		pthread_mutex_lock(&buffer_mutex);
		if(currentidx == BUFFERSIZE) {
			pthread_mutex_unlock(&buffer_mutex);
			sem_wait(&buffer_full);
		} else {
			buffer[currentidx++] = n;
			if(currentidx == 1) sem_post(&buffer_empty);
			pthread_mutex_unlock(&buffer_mutex);
			//printf("Produzindo numero %d\n", n);
			if (verificador() == 0) {
				//printf("%s\n", *cabodeguerra);
				printf("sou o produtor: %d\n", numerocontrole);
				andarParaDireita();
			}
			else {
				printf("Os vencedores foram os consumidores\n");
			}
		}
		usleep((long) (drand48() * SEU_RA/89.0));
		//clrscr();
	}
}


void *consumidor(void *arg) {
	int n;
	while(1) {
		pthread_mutex_lock(&buffer_mutex);
		if(currentidx > 0) {
			n = buffer[--currentidx];
			if(currentidx == (BUFFERSIZE - 1))
				sem_post(&buffer_full);
			pthread_mutex_unlock(&buffer_mutex);
			//printf("Consumindo numero %d\n", n);
			if (verificador() == 0) {
				//printf("%s", *cabodeguerra);
				printf("sou o consumidor: %d\n", numerocontrole);
				andarParaEsquerda();
			} else
			{
				printf("Os vencedores foram os consumidores\n");
			}
			
		} else {
			pthread_mutex_unlock(&buffer_mutex);
			sem_wait(&buffer_empty);
		}
		usleep((long) (drand48() * SEU_RA/89.0));
		//clrscr();
	}
}
