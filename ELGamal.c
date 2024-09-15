#include <stdio.h>
#include <math.h>

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

int extended_euclid_solve(int a, int b, int r, int t1, int t2) {
    int q = a / b;
    r = a % b;
    if (r == 0)
        return t2;
    int t = t1 - (t2 * q);
    return extended_euclid_solve(b, r, a % b, t2, t);
}

int extended_euclid(int b, int n) {
    int ans = extended_euclid_solve(n, b, 0, 0, 1);
    if (ans < 0)
        ans = n + ans;
    return ans;
}

int generator(int p) {
    for (int i = 2; i < p; i++) {
        int isPrimitiveRoot = 1;
        for (int j = 1; j < p - 1; j++) {
            int g = efficient_exp(i, j, p);
            if (g == 1) {
                isPrimitiveRoot = 0;
                break;
            }
        }
        if (isPrimitiveRoot) {
            return i;
        }
    }
    return -1; 
}

int is_prime(int n) {
    if (n <= 1) {
        return 0;
    }
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            return 0; 
        }
    }
    return 1;
}

int main() {
    int p = 43, m = 14, x = 7, r = 26, c1, c2;
    int g = generator(p);
    printf("Generator g: %d",g);
    int y = efficient_exp(g, x, p); // y = (g^x)mod(p)
    c1 = efficient_exp(g, r, p);    //c1 = (g^r)mod(p)
    printf("\nc1 : %d",c1);
    c2 = (efficient_exp(y, r, p) * m) % p; //c2 = ((y^r)*m)mod(p)
    printf("\nc2: %d",c2);
    printf("\nEncryption:\nc = (%d, %d)\n", c1, c2);
    int plaintext = (c2 * extended_euclid(efficient_exp(c1, x, p), p)) % p; //m = (c2/(c1^x))mod(p)
    printf("\nPlaintext : %d", plaintext);
    printf("\nDecryption:\nm = %d\n", plaintext);

    return 0;
}

