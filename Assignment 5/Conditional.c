#include <stdio.h>
#include <pthread.h>

//Jake Christensen CS 322
// Conditioncal Variables

pthread_mutex_t mutex;
pthread_cond_t empty, full, done;


int no_cycles = 2;
int no_consumers = 4;
int counter = 0;
char *input = "Hello!";
char* buffer = '\0';
int totalReads = 0;
int totalWrites = 0;


void writeToFile(int type, int num_cores, double time, int loop ){
  FILE * out;
  out = fopen("Results_5_Raw.txt","a+");
  fprintf(out,"%d,%d,%f,%d\n",type,num_cores,time,loop);
  
}




void *producer(void *arg) {
	int i;
	

		for (i = 0; i < no_cycles; i++) {
			pthread_mutex_lock(&mutex);
			// p1
			while (buffer != '\0'){
				pthread_cond_wait(&empty, &mutex);
			}
			buffer = input;
			printf("Writing new buffer.\n");
			totalWrites++;
			pthread_cond_signal(&full);
			pthread_mutex_unlock(&mutex);

			}
		}
void *consumer(void *arg) {
	int i;
		for (i = 0; i < no_cycles; i++) {
			pthread_mutex_lock(&mutex);
			while (buffer == '\0'){
				pthread_cond_wait(&full, &mutex);
			}
		char* tmp = buffer;
		totalReads++;
		counter++;
		if(counter == no_consumers){
		buffer = '\0';
		counter = 0;
		}
		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&mutex);
		printf("consumer read: %s\n", tmp);
		}
	}

int main(int argc, char **argv){
	char* p;
	if(argc >= 4){
  	no_cycles = strtol(argv[1], &p, 10);
  	no_consumers = strtol(argv[2],&p,10);
  	printf("%d    %d",no_cycles, no_consumers);
  	printf("%s is arg 3 argc %d",argv[3],argc);
  	input = argv[3];
  	}else{
  	printf("Please pass three args: number of cycles, number of threads, and a string to write\n");
  	return -1;
  	}
  	if(argc >= 2){

  	}
  	
	pthread_t producert;
	pthread_t consumers[no_consumers];
	
	int i = 0;
	
	pthread_cond_init(&empty,NULL);
	pthread_cond_init(&full, NULL);
	pthread_cond_init(&done, NULL);
	pthread_mutex_init(&mutex,NULL);
	
	
	pthread_create(&producert,NULL,producer,NULL);
	for(i = 0; i < no_consumers ; i++){
		pthread_create(&consumers[i],NULL,consumer,NULL);
		pthread_join(consumers[i],NULL);
	
	}
	pthread_join(producert,NULL);

	printf("Total Reads: %d  Total Writes: %d\n",totalReads,totalWrites);
}
