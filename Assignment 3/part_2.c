#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>   
#include <limits.h> 

void writeToFile(int type, int num_cores, double time, int loop ){
  FILE * out;
  out = fopen("Results2.txt","a+");
  fprintf(out,"%d,%d,%f,%d\n",type,num_cores,time,loop);
  
}

int main (int argc, char * argv[]){


 
   int numProc, myid, myNumber;
   double dif_time;
   MPI_Status status;
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numProc);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);
   int min, max;
   int timerStarted = 0;
   unsigned int seed = myid;
   int totalMessages = 0;
      
  int rounds = 0;
  int total_rounds;
  
  
  
    char* p;
   errno = 0;
  long arg_loops = strtol(argv[1], &p, 10);

  // Check for errors: e.g., the string does not represent an integer
  // or the integer is larger than int
    if (errno != 0 || *p != '\0' || arg_loops > INT_MAX) {
      printf("Please pass an int for number of loops as the only argument.");
      return -1;
    }else {
      total_rounds = arg_loops;    
    }
    
  
  
   while(rounds < total_rounds){
       rounds++;
       myNumber = rand_r(&seed) % 100;
       usleep(myNumber * 1000);
       timerStarted = 0;
      for(int i = 0; i < numProc; i ++){
      
	  if(i == myid){


	  }else{
	    totalMessages++;
	    if(myid == 0 && timerStarted == 0){
	      
	    dif_time = MPI_Wtime();
	    timerStarted = 1;
	    }
	    MPI_Send(&myNumber, 1,MPI_INT,i,1,MPI_COMM_WORLD);
	 //   printf("Process %d sends %d\n", myid , myNumber);
	  }

      }
   min = max = myNumber;
   for(int i = 0; i < numProc; i ++){
	  if(i == myid){


	  }else{
	    MPI_Recv(&myNumber, 1,MPI_INT,i,1,MPI_COMM_WORLD,&status);
	  //  printf("Process %d reads %d from %d\n", myid , myNumber, i);
	    if(min > myNumber){
	     min = myNumber; 
	    }
	    if(max < myNumber){
	     max = myNumber; 
	    }

	  }

      }
          if(myid == 0){
	       dif_time = MPI_Wtime() - dif_time;
	    
	   // printf("Stopping timer");
	    writeToFile(2,numProc,dif_time,rounds);
	  }
      // printf("Process: %d | Minimum: %d | Maximum: %d | Sent_Messages: %d\n", myid,min,max, totalMessages);
   }
   
  
   MPI_Finalize();

   
   

}
