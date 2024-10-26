#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    // Variables for time measurement
    clock_t start_time, end_time;
    double cpu_time_used;
    
    if (argc != 2) {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }

    // Start timing
    start_time = clock();

    // Get the upper limit from command line
    int n = atoi(argv[1]);
    printf("Finding prime numbers up to %d:\n", n);

    // Create and initialize array
    // 1 means prime, 0 means not prime
    int* isPrime = (int*)malloc((n + 1) * sizeof(int));
    
    // Initially assume all numbers are prime
    for (int i = 0; i <= n; i++) {
        isPrime[i] = 1;
    }
    
    // 0 and 1 are not prime
    isPrime[0] = isPrime[1] = 0;

    // Apply Sieve of Eratosthenes
    for (int i = 2; i * i <= n; i++) {
        if (isPrime[i]) {
            // Mark all multiples of i as not prime
            for (int j = i * i; j <= n; j += i) {
                isPrime[j] = 0;
            }
        }
    }

    // Count prime numbers
    int count = 0;
    printf("Prime numbers: ");
    for (int i = 2; i <= n; i++) {
        if (isPrime[i]) {
            printf("%d ", i);
            count++;
        }
    }
    printf("\n");
    printf("Total number of primes found: %d\n", count);

    // Calculate and print execution time
    end_time = clock();
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", cpu_time_used);

    // Free allocated memory
    free(isPrime);
    return 0;
}