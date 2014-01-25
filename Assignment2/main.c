
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Jake Christensen CS 322 Winter Quarter 2014
// This program handles the setup for a multicore approach to adding numbers.


//function to calculate 2 to the nth power.
int two_power(int power){
	int result = 1;
	for(int i = 0; i < power; i++){
		result = result * 2;
	}
	return result;
}

//Function finds the core with the lowest index (starting with 0) above a power of 2. So if
// there are 18 cores, this cuntion would return 16.
int getlowestnonpow2core(int numCores){
	int exp = 0;
	int result;
	while(result < numCores){
		result = two_power(exp++);
	}
	if(result == numCores){
		return -1;
	}else{
		return two_power(exp - 2);
	}
}
int main(int argc, char **argv){
	int n = atoi(argv[1]);
	int exp = 1;
	int entries = n;
	int currentcore = 0;
	//if there is an odd number of cores, we need to keep track of it.
	//and add the odd core at the end.
	int oddcore = -1;
	
	//find the core above the greatest power of two, this will also be added at the end
	//after all cores greater than it have been added in parallel.
	int nonpow2core = getlowestnonpow2core(n);
	
	printf("   ");
	for(int i = 0; i < n; i++){
		if(i >= 10){
			printf("%d  ", i);
		}else{
			printf("0%d  ", i);

		}
	}

	while(two_power(exp) <= n){
		if(exp > 9){
			printf("\n%d ", exp - 1);
		}else{
			printf("\n0%d ", exp - 1);

		}
		for(int i = 0; i < entries; i++){
			if(i % 2 == 0){
				int targetcore = currentcore +two_power(exp - 1);
				if(targetcore > n - 1){


					//if we try to read a core beyond the number of cores we have, then
					//we know we are beyond the power of two index, and should send all cores
					//after it to core zero once they've been added in parallel.
					if(currentcore - two_power(exp) >= nonpow2core){
						printf("S0%d ", nonpow2core);
					
					}else{
					printf("S0%d ", 0);
					}
				}else{
					if(targetcore >= 10){
						printf("R%d ", targetcore);
					}else{
						printf("R0%d ", targetcore);
					}
					if(exp > 1){
						for(int j = 0; j < two_power(exp - 1)-1; j++){
							printf("    ");
							currentcore++;
						}
					}
				}
			}else{

				int targetcore = currentcore - two_power(exp - 1);
				if(targetcore >= 10){
					printf("S%d ", targetcore);
				}else{
					printf("S0%d ", targetcore);

				}
				if(exp > 1){
					for(int j = 0; j < two_power(exp - 1)-1; j++){
						printf("    ");
						currentcore++;
					}
				}
			}
			currentcore++;
		}
		if(n % 2 > 0){
			oddcore = n - 1;
			n = n - 1;
		}
		exp++;
		currentcore = 0;
		entries = entries/2;

	}

	//add the non power cores at the end
	if(nonpow2core > 0){
		if(exp > 9){
			printf("\n%d ", exp++ - 1);
		}else{
			printf("\n0%d ", exp++ - 1);
		}
		if(nonpow2core >= 10){
			printf("R%d ", nonpow2core);


		}else{
			printf("R0%d ", nonpow2core);
		}
	}

	//add the odd core at the end
	if(oddcore > 0){
		if(exp > 9){
			printf("\n%d ", exp++ - 1);
		}else{
			printf("\n0%d ", exp++ - 1);
		}
		if(oddcore >= 10){
			printf("R%d \n", oddcore);


		}else{
			printf("R0%d \n", oddcore);
		}
	}

	getchar();

}

