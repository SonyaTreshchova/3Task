#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "matrix.h"
#include "task.h"

#define INPUT_FILE "data.dat"

typedef struct
{
	int n;
	double *a;
	double *b;
	double *x;
	int *index;
	int my_rank;
	int total_threads;
} ARGS;

long int thread_time = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *Solution(void *p_arg)
{
	ARGS *arg = (ARGS*)p_arg;
	long int t1;

	t1 = get_time();
	SolveSystem(arg->n, arg->a, arg->b, arg->x, arg->index, arg->my_rank, arg->total_threads);
	t1 = get_time() - t1;

	pthread_mutex_lock(&mutex);
	thread_time += t1;
	pthread_mutex_unlock(&mutex);

	return NULL;
}

int main(int argc, char **argv)
{
	int i;
	int n;
	double *a;
	double *b;
	double *x;
	int *index;
	long int t_full;
	int total_threads;
	pthread_t *threads;
	ARGS *args;

	if (argc > 1)
		total_threads = atoi(argv[1]);
	else
	{
		printf("Некорректные аргументы!\n");
		return -1;
	}

		if (argc == 3)
			n = atoi(argv[2]);
		else
		{
			printf("Некорректные аргументы!\n");
			return -3;
		}

		if (n <= 0)
		{
			printf("Некорректное n!\n");
			return -4;
		}
		

	a = (double*)malloc(n * n * sizeof(double));
	b = (double*)malloc(n * sizeof(double));
	x = (double*)malloc(n * sizeof(double));
	index = (int*)malloc(n * sizeof(double));
	threads = (pthread_t*)malloc(total_threads * sizeof(pthread_t));
	args = (ARGS*)malloc(total_threads * sizeof(ARGS));

	if (!(a && b && x && index && threads && args))
	{
		printf("Не достаточно памяти!\n");

		if (a) free(a);
		if (b) free(b);
		if (x) free(x);
		if (index) free(index);
		if (threads) free(threads);
		if (args) free(args);

		return -6;
	}

	InputMatrix(n, a, b);

	printf("Матрица A:\n");
	OutputMatrix(n, a, b);


	for (i = 0; i < total_threads; i++)
	{
		args[i].n = n;
		args[i].a = a;
		args[i].b = b;
		args[i].x = x;
		args[i].index = index;
		args[i].my_rank = i;
		args[i].total_threads = total_threads;
	}

	t_full = get_full_time();

	for (i = 0; i < total_threads; i++)
		if (pthread_create(threads + i, 0, Solution, args + i))
		{
			printf("Cannot create thread %d!\n", i);

			if (a) free(a);
			if (b) free(b);
			if (x) free(x);
			if (index) free(index);
			if (threads) free(threads);
			if (args) free(args);

			return -7;
		}

	for (i = 0; i < total_threads; i++)
		if (pthread_join(threads[i], 0))
		{
			printf("Cannot wait thread %d!\n", i);

			if (a) free(a);
			if (b) free(b);
			if (x) free(x);
			if (index) free(index);
			if (threads) free(threads);
			if (args) free(args);

			return -8;
		}
	t_full = get_full_time() - t_full;

	if (t_full == 0)
		t_full = 1;

	printf("\nRezulting vector:\n");
	OutputVector(n, x);

	printf("\nВремя = %f sec.\n", t_full/100.);
	printf("\nSolution time\t\t= %.2f sec.\n\n", (double)t_full / CLOCKS_PER_SEC);

	InputMatrix(n, a, b);

	printf("\n||Ax - b|| = %e\n", TestSolution(n, a, b, x));

	free(a);
	free(b);
	free(x);
	free(index);
	free(threads);
	free(args);

	return 0;
}
