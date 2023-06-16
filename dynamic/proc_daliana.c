#include "../rpc.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct
{
    reportable_t parent;

    long unsigned int op1;
    long unsigned int op2;
    long unsigned int op3;
    long unsigned int res;
} my_struct_t;

// Función auxiliar para calcular la potencia
void power(long unsigned int base, long unsigned int exponent, long unsigned int *result)
{
    *result = 1;
    for (long unsigned int i = 0; i < exponent; i++) {
        *result *= base;
    }
}

//Función que calcula el factorial de un número
void factorial(long unsigned int n, long unsigned int *result)
{
    long unsigned int fact = 1;
    for (int i = 1; i <= n; i++) {
        fact *= i;
    }
    *result = fact;
}

// Esta función se encarga de analizar y extraer los parámetros de entrada del buffer
void *parse_parameters(void *data)
{
    const char *buf = (const char *)(data);

    my_struct_t *d = (my_struct_t *)(malloc(sizeof(my_struct_t)));

    if (d)
    {
        sscanf(buf, "%ld %ld %ld", &d->op1, &d->op2, &d->op3);
    }

    return (void *)d;
}

/*Esta función se encarga de realizar las operaciones de suma, resta, multiplicación, división y potencia en dependencia del valor
del primer parámetro. También puede calcular el factorial del mayor operando*/
void *do_work(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

    d->res = 0; // Inicializar el resultado

    if (d->op1 == 1)
    {
        d->res = d->op2 + d->op3; // Suma
    }
    else if (d->op1 == 2)
    {
        d->res = d->op2 - d->op3; // Resta
    }
    else if (d->op1 == 3)
    {
        d->res = d->op2 * d->op3; // Multiplicación
    }
    else if (d->op1 == 4)
    {
        if (d->op3 != 0)
        {
            d->res = d->op2 / d->op3; // División
        }
    }
    else if (d->op1 == 5)
    {
        power(d->op2, d->op3, &(d->res)); // Potencia
    }
    else if (d->op1 == 6) {
        long unsigned int mayor = d->op2 > d->op3 ? d->op2 : d->op3; // Obtener el mayor de los otros dos operandos
        factorial(mayor, &(d->res)); // Calcular el factorial del mayor
    }

    return data;
}

// Esta función genera un informe a partir de los resultados obtenidos del cálculo
reportable_t *report(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

    d->parent.data = (char *)(malloc(255 * sizeof(char)));

    if (d->op1 < 1 || d->op1 > 6) //Si es un número que no está entre 1 y 6 error
    {
        snprintf(d->parent.data, 255, "Error: Operación no válida.\n");
    }
    else if (d->op1 == 4 && d->op3 == 0) //Si la operación es de división y el denominador es 0, error
    {
        snprintf(d->parent.data, 255, "Error: División entre cero.\n");
    }
    else
    {
        snprintf(d->parent.data, 255, "Resultado: %ld\n", d->res);
    }

    d->parent.len = strlen(d->parent.data);

    return (reportable_t *)(data);
}

// Esta función se encarga de liberar la memoria asignada durante el proceso
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


