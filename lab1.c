#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <float.h>

void print_arr(double *arr, int len)
{
	int i;
	printf("arr=[");
	for (i=0; i<len; i++)
		printf(" %.2f", arr[i]);
	printf("]\n");
}

void fill_arr(double *arr, int len, unsigned int *seed, int left, int right)
{
	int i;
	for (i=0; i<len; i++)
	{
		arr[i] = rand_r(seed) / (double)RAND_MAX * (right - left) + left;
	}
}

void apply_m1_func(double *arr, int len)
{
	int i;
	for (i=0; i<len; i++)
	{
		arr[i] = 1 / tanh(sqrt(arr[i]));
	}
}

void apply_m2_func(double *arr, int len, double *arr_copy)
{
	int i;
	for (i=0; i<len; i++)
	{
		double prev = 0;
		if (i > 0)
			prev = arr_copy[i - 1];
		arr[i] = fabs(sin(arr_copy[i] + prev));
	}
}

void apply_merge_func(double *m1, double *m2, int m2_len)
{
	int i;
	for (i=0; i<m2_len; i++)
	{
		m2[i] = pow(m1[i], m2[i]);
	}
}

void copy_arr(double *src, int len, double *dst)
{
	int i;
	for (i=0; i<len; i++)
		dst[i] = src[i];
}

void stupid_sort(double *arr, int len)
{
    int i = 0;
    double tmp;
    while (i < len - 1)
    {
        if (arr[i+1] < arr[i])
        {
            tmp = arr[i];
            arr[i] = arr[i+1];
            arr[i+1] = tmp;
            i = 0;
        }
        else i++;
    }
}

double min_not_null(double *arr, int len)
{
	int i;
	double min_val = DBL_MAX;
	for (i=0; i<len; i++)
	{
		if (arr[i] < min_val && arr[i] > 0)
			min_val = arr[i];
	}
	return min_val;
}

double reduce(double *arr, int len)
{
	int i;
	double min_val = min_not_null(arr, len);
	double x = 0;
	for (i=0; i<len; i++)
	{
		if ((int)(arr[i] / min_val) % 2 == 0)
			x += sin(arr[i]);
	}
	return x;
}

int main(int argc, char* argv[])
{
	int i, N;
	int A = 729;
	unsigned int seed1, seed2;
	struct timeval T1, T2;
	long delta_ms;
	N = atoi(argv[1]); /* N равен первому параметру командной строки */
	gettimeofday(&T1, NULL); /* запомнить текущее время T1 */
	for (i=0; i<1; i++) /* 50 экспериментов */
	{
		int m1_len = N, m2_len = N / 2;
		double m1[m1_len];
		double m2[m2_len];
		double m2_copy[m2_len];
		double x;
		seed1 = i;
		seed2 = i;

		printf("\nFill arrays\n");
		fill_arr(m1, m1_len, &seed1, 1, A);
		fill_arr(m2, m2_len, &seed2, A, 10 * A);
		print_arr(m1, m1_len);
		print_arr(m2, m2_len);

		printf("\nMap\n");
		apply_m1_func(m1, m1_len);
		copy_arr(m2, m2_len, m2_copy);
		apply_m2_func(m2, m2_len, m2_copy);
		print_arr(m1, m1_len);
		print_arr(m2, m2_len);

		printf("\nMerge\n");
		apply_merge_func(m1, m2, m2_len);
		print_arr(m1, m1_len);
		print_arr(m2, m2_len);

		printf("\nSort\n");
		stupid_sort(m2, m2_len);
		print_arr(m1, m1_len);
		print_arr(m2, m2_len);

		x = reduce(m2, m2_len);
		printf("\nX=%.4f\n", x);

	}
	gettimeofday(&T2, NULL); /* запомнить текущее время T2 */
	delta_ms = 1000*(T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec)	/ 1000;
	printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms); /* T2 - T1 */
	return 0;
}
