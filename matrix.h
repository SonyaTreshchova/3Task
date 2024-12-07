#ifndef __MATRIX_H_INCLUDED__
#define __MATRIX_H_INCLUDED__
#include <stdio.h>

double f(int i, int j, int n);

void InputMatrix(int n, double *a, double *b);

void OutputMatrix(int n, double *a, double *b);

void OutputVector(int n, double *x);

double TestSolution(int n, double *a, double *b, double *x);

double SolAccuracy(int n, double *x);

long int get_time(void);

long int get_full_time(void);

#endif /* __MATRIX_H_INCLUDED__ */
