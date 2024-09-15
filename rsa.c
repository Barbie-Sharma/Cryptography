#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int gcd(int a, int b){
	if(b == 0){
		return a;
	}
	return gcd(b, a%b);
}

int public_key(int p){
	int key = 2;
	while (key < p) {
        	if (gcd(key, p) == 1){
                	return key;
		}
		key++;	
	}
	return key;	  
}

int efficient_exp(int number, int exponent, int modulus) {
	int noOfBits = (int)ceil(log2(exponent));
	
	int numberInBit[noOfBits];
	int exponents[noOfBits];
	exponents[0] = number;
	int result = 1;

	for(int i = noOfBits-1; i>=0; i--){
		numberInBit[i] = exponent%2;
		exponent /= 2;
	}

	for(int i = 1; i<noOfBits; i++){
		exponents[i] = (int)ceil(pow(exponents[i-1], 2))%modulus;
	}
	
	for(int i = 0; i<noOfBits; i++){
		if(numberInBit[i] == 1){
			result = (exponents[(noOfBits-1) -i]*result)%modulus;
		}
	}
	return result;
}

void extended_euclidean(int A, int B, int *x, int *y) {
	if (B == 0) {
        	* x = 1;
        	* y = 0;
        	return;
    	}

    	int x1, y1;
    	extended_euclidean(B, A % B, &x1, &y1);
    	* x = y1;
    	* y = x1 - (A / B) * y1;
}

int mod_inverse(int a, int m) {
	int x, y;
	extended_euclidean(a, m, &x, &y);
    	return (x % m + m) % m;
}

int main() {
	int p = 17, q = 197;
    	int n = p * q;
    	int phi = (p - 1) * (q - 1);

    	int m;
    	printf("Enter message: ");
    	scanf("%d", &m);

    	int e = public_key(phi);
    	int d = mod_inverse(e, phi);

    	int cipher = efficient_exp(m, e, n);
    	printf("Cipher: %d\n", cipher);

    	int decrypt = efficient_exp(cipher, d, n);
    	printf("Plaintext: %d\n", decrypt);

    	return 0;
}
