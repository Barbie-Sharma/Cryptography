#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int mod_exp(int a, int b, int m)
{
    int result = 1;
    a = a % m;
    while (b > 0)
    {
        if (b % 2 == 1)
        {
            result = (result * a) % m;
        }
        a = (a * a) % m;
        b /= 2;
    }
    return result;
}

int mod_inverse(int a, int m)
{
    a = a % m;
    for (int x = 1; x < m; x++)
    {
        if ((a * x) % m == 1)
        {
            return x;
        }
    }
    return -1;
}

void elgamal_sign(int p, int g, int x, int m, int *s, int *t)
{
    int r = 5;
    *s = mod_exp(g, r, p);
    int r_inv = mod_inverse(r, p - 1);
    *t = (r_inv * (m - x * (*s))) % (p - 1);
    if (*t < 0)
    {
        *t += p - 1;
    }
}

int elgamal_verify(int p, int g, int y, int m, int s, int t)
{
    int gm = mod_exp(g, m, p);
    int ys = mod_exp(y, s, p);
    int st = mod_exp(s, t, p);
    return (st * ys) % p == gm;
}
int main()
{
    int p = 43; // Prime number
    int g = 3;  // Generator
    int x = 8;  // Bob's private key
    
    int y = mod_exp(g, x, p);
  
    int m;
    printf("Enter Message: ");
    scanf("%d", &m);
   
    int s, t;
    elgamal_sign(p, g, x, m, &s, &t);
    printf("Signature: (%d, %d)\n", s, t);
   
    int valid = elgamal_verify(p, g, y, m, s, t);
    if (valid)
    {
        printf("Signature is valid.\n");
    }
    else
    {
        printf("Signature is invalid.\n");
    }
    return 0;
}
