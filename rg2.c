// esasharahi@gmail.com
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define LOOP(i, n) for(size_t i = 0; i < n; i++)
#define DIM 2
#define H 0.0001
//-------------------- functions for rg4
double *s_p (const double *a, size_t n, double r)  // scalar product.
{
	double *temp = malloc(n * sizeof(double));
	LOOP(i, n)
	temp[i] = r * a[i];
	return temp;
}
double *a_a (const double *a1, const double *a2, size_t n)  // add a2 to a1.
{
	double *temp = malloc(n * sizeof(double));
	LOOP(i, n)
	temp[i] = a1[i] + a2[i];
	return temp;
}
//--------------------
double f1 (double *params, double t) {return cos(t);}
double f2 (double *params, double t) {return -sin(t);}
//--------------------
typedef double (*func)(double*, double);
//--------------------
void update (func* funcs, double *params, double *t, int funcs_length)
{
	double *k1 = malloc(funcs_length * sizeof(double));
	double *k2 = malloc(funcs_length * sizeof(double));
	LOOP(i, funcs_length)
	{
		k1[i] = H * funcs[i](params, *t);
		double *temp1 = a_a(params, k1, funcs_length);
		k2[i] = H * funcs[i](temp1, (*t) + H);
		double *temp2 = a_a(k1, k2, funcs_length);
		double *temp3 = s_p(temp2, funcs_length, 0.5);
		params[i] += temp3[i];
		
		free(temp1);
		free(temp2);
		free(temp3);
		temp1 = temp2 = temp3 = NULL;
	}
	free(k1);
	free(k2);
	k1 = k2 = NULL;
	(*t) += H;
	//printf("%f\n", fabs((params[0] * params[0] + params[1] * params[1]) - 1));
}
//--------------------
int main()
{
	func funcs[] = {f1, f2};
	FILE *fp = fopen("out.csv", "w");
	size_t step = 100000;
	double t = 0.0;
	double *params = malloc(DIM * sizeof(double));
	params[0] = 0.0;
	params[1] = 1.0;
	fprintf(fp, "x, y, z\n");
	LOOP(i, step)
	{
		update(funcs, params, &t, DIM);
		fprintf(fp, "%f, %f, %f\n", t, params[0], params[1]);
	}
	fclose(fp);
	free(params);
}
