#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

//Jake Christensen
// CS 322 An openMP parallel implementation of solving a triangular matrix Ax = b.





void writeToFile(int type, int num_cores, double time, int loop ){
  FILE * out;
  out = fopen("Results_5_Raw.txt","a+");
  fprintf(out,"%d,%d,%f,%d\n",type,num_cores,time,loop);
  
}

void printXvalues(float* x, int n, int ** a, int * b){
	int i,j;
	for(i=0;i<n;i++){
		for(j=0; j < n; j++){
			printf("%d(x%d) +", a[i][j],j);
		}
		printf(" = %d \n", b[i]);
	}
	
	printf("X: ");
	for(i=0; i < n; i++){
		printf( "  %d: %F", i, x[i]);
	}
	printf("\n");

}

void loadArrays(int * b, int **A, int n){
	int i,j;
	
	for(i = 0; i < n; i++){
		b[i] = 5 + i;
		for(j = 0; j < n; j++){
			A[i][j] = i + j + 1; 
		}
	}

}

void checkXVals(int *b, int ** A, int n, float * x){
	int i,j;
	float total = 0;
	for(i = 0; i < n; i++){
		
		
		for(j=0; j < n; j++){
		
		//	printf("Mulitplying %F by %d\n", x[j], A[j][i]);
			
			total = total + (x[j] * A[j][i]);
		}
		
	//	printf("Numerical value for b%d: %F\n", i,total);
	//	printf("Set value for b%d: %d\n", i,b[i]);
		total = 0;
	}

}




int main(int argc, char **argv){
	clock_t begin,end;
	int n_val;
	int row,col,i;
	char *p;
	int ** A;
	int * b;
	float * x;
	if(argc >= 2){
	  	n_val = strtol(argv[1], &p, 10);
		printf("Building %d x %d matrix\n",n_val,n_val);
  	}
	A = (int **) malloc(n_val * sizeof(int*));
	b = (int *) malloc(n_val * sizeof(int));
	x = (float *) malloc(n_val * sizeof(float));
	
	

	for(i = 0; i < n_val; i++){
		A[i] = (int *) malloc(n_val * sizeof(int));
	}
	loadArrays(b,A,n_val);
	begin = clock();
	for (row = 0; row < n_val; row++)
		x[row] = b[row];
		#pragma omp parallel for
		for (col = n_val-1; col >= 0; col--) {
			x[col] /= A[col][col];
			for (row = 0; row < col; row++){
				x[row] -= A[row][col]*x[col];
			}
		}
	end = clock();
	double time = (double)(end - begin) / CLOCKS_PER_SEC;
	
	//printXvalues(x,n_val,A,b);
	printf("\n\n");
	//checkXVals(b,A,n_val,x);
	printf("Runtime: %f\n",time);
}



