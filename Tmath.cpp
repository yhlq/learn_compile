/* Tmath.cpp */
#include "Tmath.h"

double avg(double *a, int n) {
    int i;
    double total = 0.0;
    for (i = 0; i < n; i++) {
        total += a[i];
    }
    return total / n;
}

int add(int a, int b)
{
	return a + b;
}

