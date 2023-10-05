#include "stdio.h"
#include "stdbool.h"
#include "math.h"


bool is_prime(int num);

int main(int argc, char *argv[]) {

    int n_primes, primes_found = 0, i = 0;
    printf("How many prime numbers? ");
    scanf("%d", &n_primes);
    
    do{
	if (is_prime(i)){
	    printf("%d\n", i);
    	    primes_found++;	    
	}
	i++;
    }while (primes_found != n_primes);

    return 0;
}


bool is_prime(int num) {
    double squareroot = sqrt(num);
    for(int i = 2; i <= squareroot; i++){
	if(num % i == 0){
	    return false;
	}
    }
}
