#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>

#include "matrix.h"

#define MAX_OUTPUT_SIZE 10

double f(int i, int j, int n)
{
	if (i == j) {
		if (i == 0) {
			return -1.;
		}
		if (i == n-1) {
			return -(n-1.)/n;
		}
		return -2;
	}
	if (i - j == 1) {
		return 1;
	}
	if (j - i == 1) {
		return 1;
	}
	return 0;
}

void InputMatrix(int n, double *a, double *b)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			a[i * n + j] = f(i, j, n);
		}
		b[i] = 0;
	}
	b[n-1] = 1;
}

void OutputMatrix(int n, double *a, double *b)
{
	int i, j;
	int m = (n > MAX_OUTPUT_SIZE) ? MAX_OUTPUT_SIZE : n;

	for (i = 0; i < m; i++)
	{
		printf("| ");
		for (j = 0; j < m; j++)
			printf("%10.3g ", a[i * n +j]);
		printf(" |   %10.3g\n", b[i]);
	}
}

void OutputVector(int n, double *x)
{
	int i;
	int m = (n > MAX_OUTPUT_SIZE) ? MAX_OUTPUT_SIZE : n;

	for (i = 0; i < m; i++)
		printf("%10.3g ", x[i]);
}

double TestSolution(int n, double *a, double *b, double *x)
{
	int i, j;
	double tmp;
	double rezult;

	rezult = 0.0;
	for (i = 0; i < n; i++)
	{
		tmp = 0.0;
		for (j = 0; j < n; j++)
			tmp += a[i * n + j] * x[j];
		tmp -= b[i];
		rezult += tmp * tmp;
	}
	return sqrt(rezult);
}

double SolAccuracy(int n, double *x)
{
	int i;
	double tmp;
	double rezult;

	rezult = 0.0;
	for (i = 0; i < n; i++)
	{
		tmp = x[i];
		if (i == n-1) tmp -= 1.0;
		rezult += tmp * tmp;
	}
	return sqrt(rezult);
}

long int get_time(void)
{
	struct rusage buf;

	getrusage(RUSAGE_SELF, &buf);

	return buf.ru_utime.tv_sec * 100 + buf.ru_utime.tv_usec/10000;
}

long int get_full_time(void)
{
	struct timeval buf;

	gettimeofday(&buf, 0);

	return buf.tv_sec * 100 + buf.tv_usec/10000;
}
