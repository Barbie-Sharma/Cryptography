#include <stdio.h>

int main() {
	int x;
	int * y;
	* y = x; 
	printf("%d", * y);
    	return 0;
}