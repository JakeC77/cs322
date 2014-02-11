#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <errno.h>   
#include <limits.h> 

void writeToFile(int type, int num_cores, double time, int loop ){
  FILE * out;
  out = fopen("Results.txt","a+");
  fprintf(out,"%d,%d,%f,%d\n",type,num_cores,time,loop);
  
}


int main (int argc, char*argv[]){
  int com_size, my_rank, my_number;
     MPI_Status status;

  double dif_time;
  char* p;
     
  int min, max, num_loops = 3, loop_counter;
  
    
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
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD,&com_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  
  for(loop_counter = 0; loop_counter < num_loops; loop_counter++){
   unsigned int seed = my_rank;
    
      my_number = rand_r(&seed) % 100;
	//printf("Process %d number is %d args is %d\n",my_rank,my_number,argc);
	
  if(my_rank == 0){
    printf("Starting the ring from 0\n");
    min = max = my_number;
    
    
    dif_time = MPI_Wtime();
    MPI_Send(&min, 1,MPI_INT,1,1,MPI_COMM_WORLD);
    MPI_Send(&max, 1,MPI_INT,1,0,MPI_COMM_WORLD);

    MPI_Recv(&min, 1,MPI_INT,com_size - 1,1,MPI_COMM_WORLD,&status);
    MPI_Recv(&max, 1,MPI_INT,com_size - 1,0,MPI_COMM_WORLD,&status);
    dif_time = MPI_Wtime() - dif_time;
    writeToFile(3,com_size,dif_time,loop_counter);
   printf("Process %d Received min: %d max: %d from process %d in loop %d\n", my_rank,min, max, com_size -1, loop_counter); 
  }else{
	int destination = (my_rank + 1) % com_size;
        MPI_Recv (&min , 1, MPI_INT, my_rank - 1, 1, MPI_COMM_WORLD, &status );
	MPI_Recv (&max , 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, &status );
//	printf("Process %d number is %d\n",my_rank,my_number);
	if(my_number > max){
	 max = my_number; 
	}
	if(my_number < min){
	 min = my_number; 
	}
	//printf("Process %d Received %d from process %d\n", my_rank, my_number, my_rank - 1); 
        MPI_Send ( &min, 1, MPI_INT, destination,   1, MPI_COMM_WORLD );
	MPI_Send ( &max, 1, MPI_INT, destination,   0, MPI_COMM_WORLD );
//	printf("Process %d Received min: %d max: %d from process %d\n", my_rank,min, max, my_rank - 1); 
  }
  }
  MPI_Finalize();

}



