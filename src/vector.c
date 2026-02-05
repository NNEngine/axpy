/* vector.c */

#include "libs.h"
#include "vector.h"

/* =========================
   Vector creation
   ========================= */

struct Vector *vec_alloc(size_t size)
{
    struct Vector *v = malloc(sizeof *v);
    if (!v) return NULL;

    v->size = size;
    v->data = malloc(size * sizeof(double));

    if (!v->data) {
        free(v);
        return NULL;
    }

    return v;
}

struct Vector *vec_zeros(size_t size)
{
    struct Vector *v = malloc(sizeof *v);
    if (!v) return NULL;

    v->size = size;
    v->data = calloc(size, sizeof(double));

    if (!v->data) {
        free(v);
        return NULL;
    }

    return v;
}

struct Vector *vec_ones(size_t size)
{
    struct Vector *v = vec_alloc(size);
    if(!v) return NULL;

    for(size_t i = 0; i < size; i++){
        v->data[i] = 1.0;
    }

    return v;
}

struct Vector *vec_scalar(size_t size, const double scalar)
{
    struct Vector *v = vec_alloc(size);
    if(!v) return NULL;

    for(size_t i = 0; i < size; i++){
        v->data[i] = scalar;
    }

    return v;
}

struct Vector *vec_from_array(const double *arr, size_t size)
{
    if (!arr) return NULL;

    struct Vector *v = vec_alloc(size);
    if (!v) return NULL;

    cblas_dcopy(
        (int)size,
        arr, 1,
        v->data, 1
    );

    return v;
}

/* =========================
   Destruction / Debug
   ========================= */

void dest_vector(struct Vector *vector)
{
    if (!vector) return;

    free(vector->data);
    free(vector);
}

void print_vector(const struct Vector *vector)
{
    if (!vector) return;

    for (size_t i = 0; i < vector->size; i++) {
        printf("%lf ", vector->data[i]);
    }
    printf("\n");
}

/* =========================
   Vector operations (BLAS)
   ========================= */

struct Vector *add_vector(const struct Vector *a, const struct Vector *b)
{
    if (!a || !b) return NULL;
    if (a->size != b->size) return NULL;

    struct Vector *c = vec_alloc(a->size);
    if (!c) return NULL;

    /* c = a */
    cblas_dcopy(
        (int)a->size,
        a->data, 1,
        c->data, 1
    );

    /* c = c + b */
    cblas_daxpy(
        (int)b->size,
        1.0,
        b->data, 1,
        c->data, 1
    );

    return c;
}

struct Vector *sub_vector(const struct Vector *a, const struct Vector *b)
{
    if (!a || !b) return NULL;
    if (a->size != b->size) return NULL;

    struct Vector *c = vec_alloc(a->size);
    if (!c) return NULL;

    /* c = a */
    cblas_dcopy(
        (int)a->size,
        a->data, 1,
        c->data, 1
    );

    /* c = c - b */
    cblas_daxpy(
        (int)b->size,
        -1.0,
        b->data, 1,
        c->data, 1
    );

    return c;
}
