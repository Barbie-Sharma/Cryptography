#include <stdio.h>

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

int efficient_exp(int base, int exponent, int modulus) {
	long long result = 1;
    	base %= modulus;

    	while (exponent > 0) {
        	if (exponent & 1) {
            	result = (result * base) % modulus;
        	}
        	base = (base * base) % modulus;
        	exponent >>= 1;
    	}

    	return (int)result;
}

int extended_gcd(int a, int b, int *x, int *y) {
	if (a == 0) {
        	*x = 0;
        	*y = 1;
        	return b;
    	}

    	int x1, y1;
    	int gcd = extended_gcd(b % a, a, &x1, &y1);
    	*x = y1 - (b / a) * x1;
    	*y = x1;
    	return gcd;
}

int mod_inverse(int a, int m) {
	int x, y;
    	extended_gcd(a, m, &x, &y);
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
