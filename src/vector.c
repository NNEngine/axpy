/* vector.c */

#include "libs.h"
#include "vector.h"

/* ===========================================
                Vector creation
   =========================================== */

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

struct Vector *vec_scalar(size_t size, double scalar)
{
    struct Vector *v = vec_alloc(size);
    if(!v) return NULL;

    for(size_t i = 0; i < size; i++){
        v->data[i] = scalar;
    }

    return v;
}

struct Vector *vec_arange(size_t size, double start, double step)
{
    struct Vector *v = vec_alloc(size);
    if(!v) return NULL;

    double value = start;

    for(size_t i = 0; i < size; i++){
        v->data[i] = value;
        value += step;
    }

    return v;
}

struct Vector *vec_linspace(size_t size, double start, double end)
{
    if(size == 0) return NULL;

    struct Vector *v = vec_alloc(size);
    if(!v) return NULL;

    if(size == 1){
        v->data[0] = start;
        return v;
    }

    double step = (end - start) / ((double)size -1);

    for(size_t i = 0; i < size; i++){
        v->data[i] = start + step * (double)i;
    }

    /* guarantee exact endpoint */
    v->data[size - 1] = end;

    return v;
}

void axpy_init_rng(void)
{
    srand((unsigned)time(NULL));
}

struct Vector *vec_rand(size_t size, double lower_limit, double upper_limit)
{
    if(size == 0) return NULL;

    if(upper_limit <= lower_limit) return NULL;

    struct Vector *v = vec_alloc(size);
    if(!v) return NULL;

    double range = upper_limit - lower_limit;

    for(size_t i = 0; i < size; i++){
        double random_number = (double)rand() / (double)RAND_MAX;
        v->data[i] = lower_limit + range * random_number;
    }

    return v;
}

struct Vector *vec_randn(size_t size, double mean, double variance)
{
    if (size == 0) return NULL;
    if (variance <= 0.0) return NULL;

    struct Vector *v = vec_alloc(size);
    if (!v) return NULL;

    double stddev = sqrt(variance);

    for (size_t i = 0; i < size; i++) {
        double u1 = ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0);
        double u2 = ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0);

        double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);

        v->data[i] = mean + stddev * z;
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

/* ===============================================
            Destruction / Debug
   =============================================== */

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



/*  ===================================================
            Vector Aggregation Functions
    ===================================================*/

double vec_aggr_sum(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return 0.0;

    double total_sum = 0.0;

    for(size_t i = 0; i < vector->size; i++){
        total_sum += vector->data[i];
    }

    return total_sum;
}

double vec_aggr_mean(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return 0.0;

    double total_sum = vec_aggr_sum(vector);

    return total_sum / (double)vector->size;
}

double vec_aggr_min(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return 0.0;

    double min_value = DBL_MAX;

    for(size_t i = 0; i < vector->size; i++){
        if(min_value > vector->data[i]){
            min_value = vector->data[i];
        }
    }

    return min_value;
}

int vec_aggr_argmin(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return -1;

    size_t min_index = 0;
    double min_value = vector->data[0];

    for(size_t i = 0; i < vector->size; i++){
        if(vector->data[i] < min_value){
            min_value = vector->data[i];
            min_index = i;
        }
    }
    return (int)min_index;
}

double vec_aggr_max(const struct Vector *vector)
{
    if(!vector || !vector->data) return 0.0;
    if(vector->size == 0) return 0.0;

    double max_value = -DBL_MAX;

    for(size_t i = 0; i < vector->size; i++){
        if(max_value < vector->data[i]){
            max_value = vector->data[i];
        }
    }
    return max_value;
}

int vec_aggr_argmax(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return -1;

    size_t max_index = 0;
    double max_value = vector->data[0];

    for(size_t i = 0; i < vector->size; i++){
        if(vector->data[i] > max_value){
            max_value = vector->data[i];
            max_index = i;
        }
    }
    return (int)max_index;
}

/* ===================================================
            Vector Math Operations (Out of Place)
   ===================================================*/

struct Vector *vec_math_pow(const struct Vector *vector, double power)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector) return NULL;

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = pow(vector->data[i],power);
    }
    return new_vector;
}

struct Vector *vec_math_sqrt(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector) return NULL;

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = sqrt(vector->data[i]);
    }
    return new_vector;
}


struct Vector *vec_math_cbrt(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector) return NULL;

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = cbrt(vector->data[i]);
    }
    return new_vector;
}

struct Vector *vec_math_sin(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector) return NULL;

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = cbrt(vector->data[i]);
    }
    return new_vector;
}



/* ===================================================
            Vector Math Operations (inplace)
   ===================================================*/
int vec_math_pow_inplace(struct Vector *vector, double power)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    for(size_t i = 0; i < size; i++){
        vector->data[i] = pow(vector->data[i], power);
    }
    return 0;
}

int vec_math_sqrt_inplace(struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    for(size_t i = 0; i < size; i++){
        vector->data[i] = sqrt(vector->data[i]r);
    }
    return 0;
}

int vec_math_cbrt_inplace(struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    for(size_t i = 0; i < size; i++){
        vector->data[i] = cbrt(vector->data[i]r);
    }
    return 0;
}




/* ====================================================
            Vector operations (BLAS)
   ==================================================== */

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
