#include <stdio.h>
#include <string.h>

char alphabet[26] = "abcdefghijklmnopqrstuvwxyz";

void concatKey(char possibleKey[], char longKey[]) {
	int i = 0;
	for (i = 0; i < 12; i++) {
        	longKey[i] = possibleKey[i % 3];
    	}
    	longKey[11] = '\0'; 
}

void decrypt(char ciphertext[], char longKey[], char plaintext[]) {
	int i = 0;
    	for (i = 0; i < 12; i++) {
        	int shift = longKey[i] - 'a'; 
        	plaintext[i] = alphabet[(ciphertext[i] - 'a' - shift + 26) % 26];
    	}
	plaintext[11] = '\0';
}


int containsSubString(char str[], char substr[]) {
	return strstr(str, substr) != NULL; 
}


void bruteForceAttack(char ciphertext[], char plaintext[]) {
	char possibleKey[4]; 
    	char longKey[12];
    	int i, j, k;

    	for (i = 'a'; i <= 'z'; i++) {
        	for (j = 'a'; j <= 'z'; j++) {
            		for (k = 'a'; k <= 'z'; k++) {
                		possibleKey[0] = i;
                		possibleKey[1] = j;
                		possibleKey[2] = k;
                		possibleKey[3] = '\0';
                
                		concatKey(possibleKey, longKey); 

                		decrypt(ciphertext, longKey, plaintext);

                		if (containsSubString(plaintext, "sot")) { 
                    			printf("Key found to break ciphertext: %s\n", possibleKey);
                    			printf("Plaintext is: %s\n", plaintext);
                		}
            		}
        	}
    	}
}

int main() {
	char ciphertext[12]; 
    	char plaintext[12];

    	printf("Enter ciphertext: ");
    	scanf("%s", ciphertext);

    	bruteForceAttack(ciphertext, plaintext);
    	return 0;
}