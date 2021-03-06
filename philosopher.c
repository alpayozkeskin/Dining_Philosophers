/*
 * philosopher.c
 *
 *  Created on: May 22, 2017
 *      Author: lpy
 */

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

sem_t mutex;
int C[5] = {0};
sem_t s[5];
int id[5] = {0,1,2,3,4};

void eat(int p) {
	printf("Philosopher %d is eating\n", p);
	sleep(2);
}
void think(int p) {
	printf("Philosopher %d is thinking\n", p);
}

void hungry(int p) {
	printf("Philosopher %d is hungry\n", p);
}
//test function to see if a neighbour can start eating
void test(int p) {
	if(C[(p + 5 - 1) % 5] != 2 && C[(p + 1) % 5] != 2 && C[p] == 1) {
		C[p] = 2;
		eat(p);
		sem_post(&s[p]);
	}
}

void * dine(void * arg) {
	while(1) {
		int * w = (int *) arg;
		sleep(1);
		sem_wait(&mutex);	//critical section
			C[*w] = 1;
			hungry(*w);
			test(*w);
		sem_post(&mutex);	//end of critical section
		sem_wait(&s[*w]);	//waits until &s[*w] is posted
		sleep(1);
		sleep(0);
		sem_wait(&mutex);	//critical section
			C[*w] = 0;
			think(*w);
			test((*w + 1) % 5);
			test((*w + 5 - 1) % 5);
		sem_post(&mutex);	//end of critical section
	}
}

int main(int argc, char **argv) {
	sem_init(&mutex, 0, 1); //mutex initialization to 1
	pthread_t t[5];			//5 threads representing philosophers
	int i;
	for(i = 0;i < sizeof(C) / sizeof(C[0]);i++) {
		sem_init(&s[i], 0, 0);
	}
	for(i = 0;i < sizeof(C) / sizeof(C[0]);i++) {
		pthread_create(&t[i], NULL, dine, &id[i]);
		printf("Philosopher %d has sat at the table\n", i);
	}
	for(i = 0;i < sizeof(C) / sizeof(C[0]);i++) {
		pthread_join(t[i], NULL);
	}
}
