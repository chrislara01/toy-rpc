#include "../rpc.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct
{
    reportable_t parent;

    long unsigned int op1;
    long unsigned int op2;
    long unsigned int op3;
    float op4;
    long unsigned int op5;
    float op6; 
    float res;
} my_struct_t;

void *parse_parameters(void *data)
{
    const char *buf = (const char *)(data);

    my_struct_t *d = (my_struct_t *)(malloc(sizeof(my_struct_t)));

    if (d)
    {
        sscanf(buf, "%ld %ld %ld %f %ld %f", &d->op1, &d->op2, &d->op3, &->op4, &d->op5, &d->op6);
    }

    return (void *)d;
}

double quadratic_func(double a, double b, double c, double x)
{
    return a*x*x + b*x + c;
}

double quadratic_deriv(double a, double b, double x)
{
    return 2*a*x + b;
}

/*Calcula una raiz real de la funcion cuadratica f(x) = op1*x^2 + op2*x + op3
* Utiliza el método de Newton-Raphson
* Sus parametros vinen dentro de la estructura data que 
* son los coeficientes de la función, una aproximación 
* inicial, un número maximo de iteraciones y la tolerancia.
*/
void *do_work(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

    long unsigned int a = d->op1;
    long unsigned int b = d->op2;
    long unsigned int c = d->op3;
    float x0 = d->op4;
    long unsigned int max_iter = d->op5; 
	float tol = d->op6;

	double x1 = x0;
    int iter = 0;

    while (iter < max_iter)
    {
        double fx = quadratic_func(a, b, c, x1);
        double fpx = quadratic_deriv(a, b, x1);
        double dx = -fx / fpx;
        x1 += dx;

        if (fabs(dx) <= tol)
        {
            d->res = x1;
            break;
        }

        iter++;
    }
    if (fabs(dx) > tol)
		d->res = NAN;
    
    return data;
}

reportable_t *report(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

    d->parent.data = (char *)(malloc(255 * sizeof(char)));

    snprintf(d->parent.data, 255, "LCM(%ld,%ld) = %ld\n", d->op1, d->op2, d->res);
    d->parent.len = strlen(d->parent.data);

    return (reportable_t *)(data);
}

void clean_up(void *params, void *result, reportable_t *report)
{
    if (report && report->data)
    {
        free(report->data);
    }

    if (params)
    {
        free(params);
    }
}
