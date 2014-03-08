#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

//Jake Christensen
// CS 322 Semaphore Produce consumer implementation.



int no_cycles = 2;
int no_consumers = 4;
int counter = 0;
int exitcounter = 0;
sem_t mutex, exit_mutex;
sem_t *con_sems;
sem_t produce,cons_done, prod_done;
int totalReads = 0;
int totalWrites = 0;
char* buffer = "\0";
char* input ="hello";
int out_count = 0;

void writeToFile(int type, int num_cores, double time, int loop ){
  FILE * out;
  out = fopen("Results_5_Raw.txt","a+");
  fprintf(out,"%d,%d,%f,%d\n",type,num_cores,time,loop);
  
}




void *producer(void *arg) {
	int i;
	int j;
	printf("producer:\n");
		for (i = 0; i < no_cycles; i++) {
			sem_wait(&produce);
			sem_wait(&mutex);
			printf("Writing to buffer.\n");
			buffer = input;
			totalWrites++;
			sem_post(&mutex);
			for(j = 0; j < no_consumers; j++){
				printf("post sem %d\n",j);
				sem_post(&con_sems[j]);
			}	
			}
			
		sem_post(&prod_done);
		}
void *consumer(void *arg) {
	int i;
		for (i = 0; i < no_cycles; i++) {
			sem_wait(&con_sems[*((int*)arg)]);
			sem_wait(&mutex);
			char* tmp = buffer;
			totalReads++;
			counter++;
			if(counter == no_consumers){
				buffer = '\0';
				counter = 0;
				sem_post(&produce);
			}
	
			printf("Consumer %d read %s\n", *((int*)arg),tmp);
			sem_post(&mutex);

		}
		out_count--;
		if(out_count == 0){
		sem_post(&cons_done);
		}
	}

int main(int argc, char **argv){
	char* p;
	sem_init(&mutex,0,1);
	sem_init(&produce,0,1);
	sem_init(&cons_done,0,0);
	sem_init(&prod_done,0,0);
	
	if(argc >= 4){
  		no_cycles = strtol(argv[1], &p, 10);
  		 out_count = no_consumers = strtol(argv[2],&p,10);
  		printf("%d    %d",no_cycles, no_consumers);
  		printf("%s is arg 3 argc %d",argv[3],argc);
  		input = argv[3];
  	}else{
  		printf("Please pass three args: number of cycles, number of threads, and a string to write\n");
  		return -1;
  	}
	pthread_t producert;
	pthread_t consumers[no_consumers];
	
	int i = 0;
	con_sems = malloc(sizeof(sem_t) * no_consumers);
	pthread_create(&producert,NULL,producer,NULL);
	for(i = 0; i <= no_consumers ; i++){
		int *arg = malloc(sizeof(*arg));
		*arg = i;
		pthread_create(&consumers[i],NULL,consumer,arg);
		sem_init(&con_sems[i],0,0);
	
	}
	
	
	sem_wait(&prod_done);
	sem_wait(&cons_done);
	printf("Total writes: %d Total Reads: %d",totalWrites,totalReads);
	
}
