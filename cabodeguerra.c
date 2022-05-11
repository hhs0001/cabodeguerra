#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

int sorteio(int MIN, int MAX){
	srand48(time(0));
	int n = (int) (drand48()*(MAX - MIN + 1)) + MIN;
	return n;
}

int currentidx;
pthread_mutex_t buffer_mutex;
void *consumidor(void *arg);
void *produtor(void *arg);

//declaração das variaveis globais
int SEU_RA = 202010655;
char primiraLetra = 'H';
char segundaLetra = 'H';
char terceiraLetra = 'S';
int loop = 1;
char cabodeguerra[87];

int primeiraLetraPOS = 42;
int ultimaLetraPOS = 44;
int numerocontrole = 43;

//função que verifica se a letra esta na primeira posição ou se a letra esta na ultima posição

int verificador(){
	if (numerocontrole == 1){
		return 1;
	}
	if (numerocontrole == 86){
		return 2;
	}
	return 0;
}



//limpar tela conio.h não funciona

void clrscr()
{
   system("clear");
}

//andar para a esquerda com o cabodeguerra

int andarParaEsquerda() {
	cabodeguerra[numerocontrole] = terceiraLetra;
	cabodeguerra[numerocontrole - 1] = segundaLetra;
	cabodeguerra[numerocontrole - 2] = primiraLetra;
	cabodeguerra[numerocontrole + 1] = '_';
	numerocontrole--;
	return 0;
}

//andar para a direita com o cabodeguerra

int andarParaDireita() {
	cabodeguerra[numerocontrole] = primiraLetra;
	cabodeguerra[numerocontrole + 1] = segundaLetra;
	cabodeguerra[numerocontrole + 2] = terceiraLetra;
	cabodeguerra[numerocontrole - 1] = '_';
	numerocontrole++;
	return 0;
}

int main(int argc, char **argv) {
	int numeroConsumidores = sorteio(7, 22);
	int numeroProdutores = sorteio(7, 22);
	while (numeroConsumidores == numeroProdutores) {
		numeroConsumidores = sorteio(7, 22);
		numeroProdutores = sorteio(7, 22);
	}
	for(int contador = 0; contador <= 86; contador++)
  	{
		if (contador == 42)
		{
			cabodeguerra[contador] = primiraLetra;
		}
		else if (contador == 43)
		{
			cabodeguerra[contador] = segundaLetra;
		}
		else if (contador == 44)
		{
			cabodeguerra[contador] = terceiraLetra;
		}
		else{
			cabodeguerra[contador] = '_';
		}
	}
	cabodeguerra[87] = '\0';
	int i; 
	time_t t;
	srand48(time(&t));
	currentidx = 0;
	pthread_mutex_init(&buffer_mutex, NULL);
	for(i = 0; i < numeroConsumidores; i++) {
		pthread_t cons;
		pthread_create(&cons, NULL, consumidor, NULL);
	}
	for(i = 0; i < numeroProdutores; i++) {
		pthread_t prod;
		pthread_create(&prod, NULL, produtor, NULL);
	}
	while(loop == 1) {
		clrscr();
		printf("%s\n", cabodeguerra);
		if (verificador() != 0) {
			if (verificador() == 1) {
				printf("%s\n", "Os consumidores ganharam o jogo!");
				printf("O numero de consumidores foi: ");
				printf("%d\n", numeroConsumidores);
				printf("O numero de produtores foi: ");
				printf("%d\n", numeroProdutores);
			}
			if (verificador() == 2) {
				printf("%s\n", "Os produtores ganharam o jogo!");
				printf("O numero de consumidores foi: ");
				printf("%d\n", numeroConsumidores);
				printf("O numero de produtores foi: ");
				printf("%d\n", numeroProdutores);
			}
			loop = 0;
		}
	}
}

void *produtor(void *arg) {
	while(1) {
		if(verificador() == 0) {
			pthread_mutex_lock(&buffer_mutex);
			andarParaDireita();
			pthread_mutex_unlock(&buffer_mutex);
			usleep((long) (drand48() * SEU_RA/89.0));
		}
	}
}


void *consumidor(void *arg) {
	while(1) {
		if(verificador() == 0) {
			pthread_mutex_lock(&buffer_mutex);
			andarParaEsquerda();
			pthread_mutex_unlock(&buffer_mutex);
			usleep((long) (drand48() * SEU_RA/89.0));
		}
	}
}
