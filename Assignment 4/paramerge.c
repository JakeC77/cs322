#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>   
#include <limits.h> 

//Jake Christensen
//Runs a parallel merge sort using MPI.

void writeToFile(int type, int num_cores, double time, int loop ){
  FILE * out;
  out = fopen("Results_assig4.txt","a+");
  fprintf(out,"%d,%d,%f,%d\n",type,num_cores,time,loop);
  
}

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}


int* mergeAndSort(int *a, int *b, int alen, int blen){
	int * newList = (int *) malloc(sizeof(int) * (alen + blen));
	int i;
	for (i = 0; i < alen; i++){
		newList[i] = a[i];
	}

	
	for (i = 0; i < blen; i++){
		newList[alen + i] = b[i];
	}
	qsort(newList,(alen + blen),sizeof(int),compare);
	return newList;
}

int main (int argc, char * argv[]){


 
   int numProc,arg_n, myid, myNumber, numKeys;
   MPI_Status status;
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numProc);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);
   int min, max;
   unsigned int seed = myid;
   int timeStarted = 0;
  int * buf, * received;
  int rounds = 0;
  int num_loops;
  double dif_time;
  char* p;
  int loop_counter, size, remainder;
   errno = 0;
  long arg_loops = strtol(argv[1], &p, 10);
  int *vals;

  if (errno != 0 || *p != '\0' ) {
    printf("Please pass an int for number of loops as the only argument.");
    return -1;
  }else {
    num_loops = arg_loops;    
   }
   




  
  for(loop_counter = 0; loop_counter < num_loops; loop_counter++){
   unsigned int seed = myid;

   if(myid == 0){
     int i;
    	 arg_n = strtol(argv[2], &p, 10);
	 MPI_Bcast(&arg_n, 1,MPI_INT,0,MPI_COMM_WORLD);
	 remainder = arg_n % numProc;
	 printf("main remainder is %d",remainder);
	 numKeys = arg_n/numProc;
	 size = numKeys;
	 if(remainder > 0){
	  size = size + 1;
	 }
     buf = (int*)malloc(sizeof(int) * (remainder + (numKeys * numProc)));
	vals = (int*) malloc(sizeof(int) * numKeys + remainder);
	for(i = 0; i < size; i++){
     	vals[i] = rand_r(&seed) % 100;
  
     }
     qsort(vals,size,sizeof(int),compare);
     
   }else{


 
      
     MPI_Bcast(&myNumber, 1,MPI_INT,0,MPI_COMM_WORLD);
     
     numKeys = myNumber/numProc;
    	remainder = myNumber % numProc;
     size = numKeys;

	 if(remainder > myid){
	  size = size + 1;
	 }
     vals = (int*) malloc(sizeof(int) * numKeys);
     int i;
     for(i = 0; i < size; i++){
     	vals[i] = rand_r(&seed) % 100;
     }
     qsort(vals,size,sizeof(int),compare);

     
 


   }
	int i;
	  for(i = 0; i < size; i++){
     	printf("%d: %d ", i,vals[i]);
     	}   
     	    printf("\n");
     MPI_Gather( vals, numKeys, MPI_INT, buf, numKeys, MPI_INT, 0, MPI_COMM_WORLD); 
	int j;
	if(myid == 0){
	for(j = 0; j < numProc; j++){
		int k;
		for(k = 0; k < numKeys; k++){
		printf(" %d ", buf[(j*numKeys) + k]);
		}
	}
	}
	
	//Tree propagation
	int count = 1;
	if(myid == 0){
	dif_time = MPI_Wtime();
	}
	for(; count < numProc;){
	 	if(myid % (2 * count) == 0){
	 		if(myid + count < numProc){
	 		MPI_Recv(&myNumber,1,MPI_INT, myid+count,0,MPI_COMM_WORLD,&status);
	 		received = (int*) malloc(myNumber * sizeof(int));
	 		MPI_Recv(received,myNumber, MPI_INT,myid+count,0,MPI_COMM_WORLD,&status);
	 		vals = mergeAndSort(received, vals, myNumber, size);
	 		size = size + myNumber;
	 		}
	 	
	 	}else{
	 	
	 		int target =  myid - count;
	 		MPI_Send(&size,1,MPI_INT,target,0,MPI_COMM_WORLD);
	 		MPI_Send(vals,size,MPI_INT,target,0,MPI_COMM_WORLD);
	 		break;
	 	}
	 	count = count * 2;
    
    	}
    	if(myid == 0){
    		dif_time = MPI_Wtime() - dif_time;
   		 writeToFile(arg_n,numProc,dif_time,loop_counter);
    
    		printf("My size is %d\n" ,size);
    		int i;
    		printf("Final Array: \n");;
    		for(i = 0; i < size; i++){
    			printf("%d\n",vals[i]); 
    		}
    	
    	}
  }
  MPI_Finalize();

   
   

}
