#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char K1[9];
char K2[9];	
char cipher[9];

int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
int P8[] = {6, 3, 7, 4, 8, 5, 10, 9};
int IP[] = {2, 6, 3, 1, 4, 8, 5, 7};
int IPinv[] = {4, 1, 3, 5, 7, 2, 8, 6};
int EP[] = {4, 1, 2, 3, 2, 3, 4, 1};
int P4[] = {2, 4, 3, 1};
int SW[] = {5, 6, 7, 8, 1, 2, 3, 4};

int S0[4][4] = {{1, 0, 3, 2}, {3, 2, 1, 0}, {0, 2, 1, 3}, {3, 1, 3, 2}};
int S1[4][4] = {{0, 1, 2, 3}, {2, 0, 1, 3}, {3, 0, 1, 0}, {2, 1, 0, 3}};

void binarize(char input, char bin[11]) {
    	int i;
    	for (i = 0; i < 10; i++) {
        	bin[i] = (input & (1 << (9 - i))) ? '1' : '0';
    	}
    	bin[10] = '\0';
}

void leftShift(char array[6]) {
    	char temp = array[0];
    	for (int i = 0; i < 4; i++) {
        	array[i] = 
array[i + 1];
    	}
    	array[4] = temp;
}


void concat(char left[], char right[], char result[], int half) {
	strncpy(result, left, half);
	strncpy((result + half), right, half);
	result[2*half] = '\0';
}

void xorBlock(const char block1[], const char block2[], char result[], int len) {
    for (int j = 0; j < len; j++) {
        result[j] = (block1[j] ^ block2[j]) + '0';
    }
    result[len] = '\0';
}


void ApplyPermutation(char X[], int permutation[], char result[], int len) {
	for (int j = 0; j < len; j++) {
		result[j] = X[permutation[j] - 1];	
	}
	result[len] = '\0';
}

void ApplySBox(char X[6], int SBox[4][4], char output[2]) {
    int r, c, o;

    r = 2 * (X[0] - '0') + (X[3] - '0');
    c = 2 * (X[1] - '0') + (X[2] - '0');
    o = SBox[r][c];

    output[0] = (o & 2) ? '1' : '0';
    output[1] = (o & 1) ? '1' : '0';
    output[2] = '\0';
}

void SDESKeySchedule(char bin[11]){
	char temp[11];

	ApplyPermutation(bin, P10, temp, 10);
	
    	char left[6];
    	left[5] = '\0';

    	char right[6];
    	right[5] = '\0';

    	for(int i = 0; i < 5; i++){
        	left[i] = temp[i];
        	right[i] = temp[i + 5];
    	}

    	leftShift(left);
    	leftShift(right);

    	concat(left, right, temp, 5);

	ApplyPermutation(temp, P8, K1, 8);

    	leftShift(left);
    	leftShift(right);

 	concat(left, right, temp, 5);

    	ApplyPermutation(temp, P8, K2, 8);
}

void genKeys(){
	char input;
	printf("Enter a character to generate key: ");
	scanf("%c", &input);
	
	char temp[11];
	binarize(input, temp);

	SDESKeySchedule(temp);

	printf("K1: %s\n", K1);
	printf("K2: %s\n", K2);
}

void f_K(char input[9], char K[9], char output[9]){
	char left[5], right[5];
	char expRight[9], xoredRK[9];
	char xorRK_L[6], xorRK_R[6];
	char S0_out[3], S1_out[3];
	char S01[5], S01Perm[5];
	char xoredLeft[6];

    	for(int i = 0; i < 4; i++){
        	left[i] = input[i];
        	right[i] = input[i + 4];
    	}
	left[4] = '\0';
	right[4] = '\0';

	ApplyPermutation(right, EP, expRight, 8);

	xorBlock(expRight, K, xoredRK, 8);

	for(int i = 0; i < 4; i++){
        	xorRK_L[i] = xoredRK[i];
        	xorRK_R[i] = xoredRK[i + 4];
    	}

	ApplySBox(xorRK_L, S0, S0_out);
    	ApplySBox(xorRK_R, S1, S1_out);

	concat(S0_out, S1_out, S01, 2);
	ApplyPermutation(S01, P4, S01Perm, 4);


	xorBlock(S01Perm, left, xoredLeft, 4);
    	concat(xoredLeft, right, output, 4);
}

void encrypt(char input[9]){
	char out0[9];
	char out1[9];
	char out2[9];
	char out3[9];

	ApplyPermutation(input, IP, out0, 8);
	
	f_K(out0, K1, out1);
	
	ApplyPermutation(out1, SW, out2, 8);

	f_K(out2, K2, out3);

	ApplyPermutation(out3, IPinv, cipher, 8);
}

void decrypt(char input[9], char decrypted[9]){
	char out0[9];
    	char out1[9];
    	char out2[9];
    	char out3[9];

    	ApplyPermutation(input, IP, out0, 8);
    
    	f_K(out0, K2, out1); 
    
    	ApplyPermutation(out1, SW, out2, 8);
    
    	f_K(out2, K1, out3);

    	ApplyPermutation(out3, IPinv, decrypted, 8);
}

int main(){
	genKeys();

	char input[9], decrypted[9];

    	printf("Enter 8 bit Message: ");
    	scanf("%8s", input);

	encrypt(input);
	printf("Encrypted Message: %s\n", cipher);

	decrypt(cipher, decrypted);
    	printf("Decrypted Message: %s\n", decrypted);

	return 0;
}
