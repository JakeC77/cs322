#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>   
#include <limits.h> 


void writeToFile(int type, int num_cores, double time, int loop ){
  FILE * out;
  out = fopen("Results1.txt","a+");
  fprintf(out,"%d,%d,%f,%d\n",type,num_cores,time,loop);
  
}

int main (int argc, char * argv[]){


 
   int numProc, myid, myNumber;
   MPI_Status status;
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numProc);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);
   int min, max;
   unsigned int seed = myid;
   int timeStarted = 0;
      
  int rounds = 0;
  int num_loops;
  double dif_time;
  char* p;
  int loop_counter;
   errno = 0;
  long arg_loops = strtol(argv[1], &p, 10);

  // Check for errors: e.g., the string does not represent an integer
  // or the integer is larger than int
  if (errno != 0 || *p != '\0' || arg_loops > INT_MAX) {
    printf("Please pass an int for number of loops as the only argument.");
    return -1;
  }else {
    num_loops = arg_loops;    
   }

  
  for(loop_counter = 0; loop_counter < num_loops; loop_counter++){
   unsigned int seed = myid;

      myNumber = rand_r(&seed) % 100;
	//printf("Process %d number is %d args is %d\n",my_rank,my_number,argc);

   if(myid > 0){

     MPI_Send(&myNumber, 1,MPI_INT,0,1,MPI_COMM_WORLD);
     
   }else{
     int i = 1;
     min = max = myNumber;
     dif_time = MPI_Wtime();
     while(i < numProc){
      MPI_Recv(&myNumber,1,MPI_INT,i,1,MPI_COMM_WORLD,&status);
      printf("Got %d from processor %d\n", myNumber,i);
         i++;
      if(min > myNumber){
	min = myNumber;
      }
      
      if(max < myNumber){
	max = myNumber;
      }
	 
	 
     }
         dif_time = MPI_Wtime() - dif_time;
	 writeToFile(1,numProc,dif_time,loop_counter);

           printf("Minimum: %d  Maximum: %d\n", min,max);
	  
   }
  }
  MPI_Finalize();

   
   

}