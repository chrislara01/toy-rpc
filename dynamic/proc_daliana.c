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
    long unsigned int res;
} my_struct_t;

// Función auxiliar para calcular la potencia
void power(long unsigned int base, long unsigned int exponent, long unsigned int *result)
{
    *result = (long unsigned int)pow(base, exponent);
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
del primer parámetro*/
void *do_work(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

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
        else
        {
            printf("Error: División entre cero.\n");
            
        }
    }
    else if (d->op1 == 5)
    {
        power(d->op2, d->op3, &(d->res)); // Potencia
    }
    else
    {
        printf("Error: Operación no válida.\n");
        
    }

    return data;
}

// Esta función genera un informe a partir de los resultados obtenidos del cálculo
reportable_t *report(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

    d->parent.data = (char *)(malloc(255 * sizeof(char)));

    snprintf(d->parent.data, 255, "Result: %ld\n", d->res);
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


