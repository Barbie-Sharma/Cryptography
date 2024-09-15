#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char K1[9];
char K2[9];
char cipher[9];
char plaintext[9];

int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
int P8[] = {6, 3, 7, 4, 8, 5, 10, 9};
int IP[] = {2, 6, 3, 1, 4, 8, 5, 7};
int IPinv[] = {4, 1, 3, 5, 7, 2, 8, 6};
int EP[] = {4, 1, 2, 3, 2, 3, 4, 1};
int P4[] = {2, 4, 3, 1};
int SW[] = {5, 6, 7, 8, 1, 2, 3, 4};

int S0[4][4] = {{1, 0, 3, 2}, {3, 2, 1, 0}, {0, 2, 1, 3}, {3, 1, 3, 2}};
int S1[4][4] = {{0, 1, 2, 3}, {2, 0, 1, 3}, {3, 0, 1, 0}, {2, 1, 0, 3}};

void readTextFromFile(char text[9], FILE* file) {
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    fscanf(file, "%8s", text);
}

void ApplyPermutation(char X[], int permutation[], char result[], int len) {
	for (int j = 0; j < len; j++) {
        	result[j] = X[permutation[j] - 1];
    	}
    	result[len] = '\0';
}

void leftShift(char array[6]) {
	char temp = array[0];
    	for (int i = 0; i < 4; i++) {
        	array[i] = array[i + 1];
    	}
    	array[4] = temp;
}

void concat(char left[], char right[], char result[], int half) {
    	strncpy(result, left, half);
    	strncpy((result + half), right, half);
    	result[2 * half] = '\0';
}


void xorBlock(const char block1[], const char block2[], char result[], int len) {
    for (int j = 0; j < len; j++) {
        result[j] = (block1[j] ^ block2[j]) + '0';
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

void SDESKeySchedule(char bin[11]) {
	char temp[11];

    	ApplyPermutation(bin, P10, temp, 10);

    	char left[6];
    	left[5] = '\0';

    	char right[6];
    	right[5] = '\0';

    	for (int i = 0; i < 5; i++) {
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


void decrypt(char input[9], char decrypted[9]) {
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


void printBinaryAndStore(int n, char possibleKey[]) {
	for (int i = 9; i >= 0; i--) {
        	int bit = (n >> i) & 1;
        	possibleKey[9 - i] = bit + '0';
    	}
	possibleKey[10] = '\0';
}

int areStringsEqual(const char *str1, const char *str2) {
	return strcmp(str1, str2) == 0;
}

void bruteForceAttack(char ciphertext[9], char plaintext[9]) {
	char decrypted[9];
	char possibleKey[11];

	for (int i = 0; i < 1024; i++) {
		printBinaryAndStore(i, possibleKey);
		SDESKeySchedule(possibleKey);
        	decrypt(ciphertext, decrypted);

        	if (areStringsEqual(decrypted, plaintext)) {
            		printf("Key found to break %s: %s\n", ciphertext, possibleKey);
            		break;
        	} else {
            		continue;
        	}
    	}
}

int main() {	
	FILE* ciphers[] = {fopen("ciphertext.txt", "r")};
    	FILE* plaintexts[] = {fopen("plaintexts.txt", "r")};

    	if (ciphers[0] == NULL || plaintexts[0] == NULL) {
        	perror("Error opening files");
        	exit(1);
    	}

    	char cipher[9];
    	char plaintext[9];

    	int pairCount = 0; 
	int total_pairs = sizeof(ciphers);

    	for(int i = 0; i<total_pairs; i++){
        	readTextFromFile(cipher, ciphers[0]);
        	readTextFromFile(plaintext, plaintexts[0]);

        	printf("Round %d:\n", pairCount + 1);
        	printf("Ciphertext: %s\n", cipher);
        	printf("Plaintext: %s\n", plaintext);

        	bruteForceAttack(cipher, plaintext);

        	pairCount++;
    	}

    	fclose(ciphers[0]);
    	fclose(plaintexts[0]);

    	return 0;
}