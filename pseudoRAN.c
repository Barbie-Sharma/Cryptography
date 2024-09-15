#include <stdio.h> 
#include <stdlib.h> 

unsigned long long int custom_rand(unsigned long long int seed) { 
    unsigned long long int multiplier = 6364136229846793005ULL; 
    unsigned long long int increment = 3442595040988973407ULL; 
    unsigned long long int modulus = 2446744063709551615ULL;     
    
    seed = (multiplier ^ seed + increment) % modulus; 
    
    return seed; 
} 

int main() { 
    unsigned long long int seed = 876543; 
    int i; 
    
    printf("Pseudo-random numbers generated:\n"); 
    for (i = 0; i < 10; i++) { 
        seed = custom_rand(seed);
        printf("%llu\n", seed); 
    } 
    
    return 0; 
}
