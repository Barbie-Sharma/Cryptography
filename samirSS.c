#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int random_number(long x, long a, long c, int m) {
    long result = (x * a + c) % m;
    int res = (int)result;
    return res;
}

int main() {

    int S, n, k;
    printf("Enter Secret (S): ");
    scanf("%d", &S);
    printf("Enter Number Of Partitions(n): ");
    scanf("%d", &n);
    printf("Enter number of points(k): ");
    scanf("%d", &k);
    int polynomial[k];
    polynomial[0] = S;
    for (int i = 1; i < k; i++) {
        polynomial[i] = random_number(time(0), 17, time(0) + 3, 967);
    }
    int points[n][2];
    for (int i = 1; i < n + 1; i++) {
        points[i - 1][0] = i;
        int sum = 0;
        for (int j = 0; j < k; j++) {
            sum += pow(points[i - 1][0], j) * polynomial[j];
        }
        points[i - 1][1] = sum;
    }
    int random_points[k][2];

    for (int i = 0; i < k; i++) {
        random_points[i][0] = points[i][0];
        random_points[i][1] = points[i][1];
    }

    double l[k];
    int found_Secret = 0;
    for (int i = 0; i < k; i++) {
        l[i] = 1.0;
        for (int j = 0; j < k; j++) {
            if (j != i) {
                l[i] *= (double)-1 * (random_points[j][0]) / (random_points[i][0] - random_points[j][0]);
            }
        }
        found_Secret += (int)(l[i]) * random_points[i][1];
    }
    printf("\nSecret Found: %d\n", found_Secret);
    return 0;
}

