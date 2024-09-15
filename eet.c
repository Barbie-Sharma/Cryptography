#include <stdio.h>

void ExtendedEuclidean(int A, int B, int *x, int *y) {
	if (B == 0) {
        	* x = 1;
        	* y = 0;
        	return;
    	}

    	int x1, y1;
    	ExtendedEuclidean(B, A % B, &x1, &y1);
    	* x = y1;
    	* y = x1 - (A / B) * y1;
}

int main() {
	int A = 321, B = 121;
	int s, t;
	ExtendedEuclidean(A, B, &s, &t);
	printf("s = %d, t = %d\n", s, t);
    	return 0;
}