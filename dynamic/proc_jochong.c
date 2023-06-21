#include "../rpc.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    reportable_t parent;

    long unsigned int num;
    int is_perfect;
} my_struct_t;

void *parse_parameters(void *data)
{
    const char *buf = (const char *)(data);

    my_struct_t *d = (my_struct_t *)(malloc(sizeof(my_struct_t)));

    if (d)
    {
        sscanf(buf, "%ld", &d->num);
    }

    return (void *)d;
}

void *do_work(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

    long unsigned int sum = 1;
    for (long unsigned int i = 2; i * i <= d->num; i++) {
        if (d->num % i == 0) {
            sum += i;
            if (i * i != d->num) {
                sum += d->num / i;
            }
        }
    }
    d->is_perfect = (sum == d->num);

    return data;
}

reportable_t *report(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

    d->parent.data = (char *)(malloc(255 * sizeof(char)));

    snprintf(d->parent.data, 255, "%ld %s a perfect number\n", d->num, d->is_perfect ? "is" : "is not");
    d->parent.len = strlen(d->parent.data);

    return (reportable_t *)(data);
}