/* vector.c */

#include "libs.h"
#include "vector.h"

/* PIE MACRO*/

#ifndef M_PI
#define M_PI 3.1415926535897932
#endif


/* ===========================================
                Vector creation
   =========================================== */

struct Vector *vec_alloc(size_t size)
{
    struct Vector *v = malloc(sizeof *v);

    if (!v)
    {
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc error: failed to allocate Vector struct (%s)\n",
                strerror(errno));
        return NULL;
    }

    v->size = size;
    v->data = malloc(size * sizeof(double));

    if (!v->data)
    {
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc error: failed to allocate data buffer (%s)\n",
                stderror(errno));
        free(v);
        return NULL;
    }

    return v;
}

struct Vector *vec_zeros(size_t size)
{
    struct Vector *v = malloc(sizeof *v);

    if (!v)
    {
        errno = ENOMEM;
        fprintf(stderr,
                "vec_zeros: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    v->size = size;
    v->data = calloc(size, sizeof(double));

    if (!v->data)
    {
        errno = ENOMEM;
        fprintf(stderr,
                "vec_zeros: failed to allocate data buffer (%s)\n",
                stderror(errno));
        free(v);
        return NULL;
    }

    return v;
}

struct Vector *vec_ones(size_t size)
{
    struct Vector *v = vec_alloc(size);
    if(!v)
    {
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%S)\n",
                strerror(errno));
        return NULL;
    }

    for(size_t i = 0; i < size; i++){
        v->data[i] = 1.0;
    }

    return v;
}

struct Vector *vec_scalar(size_t size, double scalar)
{
    struct Vector *v = vec_alloc(size);
    if(!v)
    {
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%S)\n",
                strerror(errno));
        return NULL;
    }

    for(size_t i = 0; i < size; i++){
        v->data[i] = scalar;
    }

    return v;
}

struct Vector *vec_arange(size_t size, double start, double step)
{
    struct Vector *v = vec_alloc(size);
    if(!v)
    {
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%S)\n",
                strerror(errno));
        return NULL;
    }

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
    if(!v)
    {
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%S)\n",
                strerror(errno));
        return NULL;
    }

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
    if(!v)
    {
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%S)\n",
                strerror(errno));
        return NULL;
    }

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
    if(!v)
    {
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%S)\n",
                strerror(errno));
        return NULL;
    }

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
    if(!v)
    {
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%S)\n",
                strerror(errno));
        return NULL;
    }

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
    if(!new_vector)
    {
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%S)\n",
                strerror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = pow(vector->data[i],power);
    }
    return new_vector;
}

struct Vector *vec_math_sqrt(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector)
    {
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%S)\n",
                strerror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = sqrt(vector->data[i]);
    }
    return new_vector;
}


struct Vector *vec_math_cbrt(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector)
    {
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = cbrt(vector->data[i]);
    }
    return new_vector;
}

struct Vector *vec_math_sin(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector)
    {
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = sin(vector->data[i]);
    }
    return new_vector;
}

struct Vector *vec_math_cos(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = cos(vector->data[i]);
    }
    return new_vector;
}

struct Vector *vec_math_tan(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = tan(vector->data[i]);
    }
    return new_vector;
}


struct Vector *vec_math_asin(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = asin(vector->data[i]);
    }
    return new_vector;
}

struct Vector *vec_math_acos(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = acos(vector->data[i]);
    }
    return new_vector;
}

struct Vector *vec_math_atan(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = atan(vector->data[i]);
    }
    return new_vector;
}

struct Vector *vec_math_sinh(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = sinh(vector->data[i]);
    }
    return new_vector;
}

struct Vector *vec_math_cosh(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = cosh(vector->data[i]);
    }
    return new_vector;
}


struct Vector *vec_math_tanh(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = tanh(vector->data[i]);
    }
    return new_vector;
}

struct Vector *vec_math_loge(const struct Vector*vector)
{
    if(!vector || !vector->data || vector->size ==0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for (size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = log(vector->data[i]);
    }

    return new_vector;
}

struct Vector *vec_math_log(const struct Vector *vector, double base)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;
    if(base <= 1.0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    double log_base = log(base);

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = log(vector->data[i]) / log_base;
    }
    return new_vector;
}

struct Vector *vec_math_exp(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = exp(vector->data[i]);
    }
    return new_vector;
}

struct Vector *vec_math_floor(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = floor(vector->data[i]);
    }
    return new_vector;
}

struct Vector *vec_math_ceil(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = ceil(vector->data[i]);
    }
    return new_vector;
}

struct Vector *vec_math_fmod(const struct Vector *vector, double divisor)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;
    if(divisor == 0.0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = fmod(vector->data[i], divisor);
    }
    return new_vector;
}

struct Vector *vec_math_trunc(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = trunc(vector->data[i]);
    }
    return new_vector;
}

struct Vector *vec_math_round(const struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return NULL;

    struct Vector *new_vector = vec_alloc(vector->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        new_vector->data[i] = round(vector->data[i]);
    }
    return new_vector;
}

/* ===================================================
            Vector Math Operations (inplace)
   ===================================================*/
int vec_math_pow_inplace(struct Vector *vector, double power)
{
    if(!vector || !vector->data || vector->size == 0) return -1;

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = pow(vector->data[i], power);
    }
    return 0;
}

int vec_math_sqrt_inplace(struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return -1;

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = sqrt(vector->data[i]);
    }
    return 0;
}

int vec_math_cbrt_inplace(struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return -1;

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = cbrt(vector->data[i]);
    }
    return 0;
}

int vec_math_sin_inplace(struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return -1;

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = sin(vector->data[i]);
    }
    return 0;
}

int vec_math_cos_inplace(struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return -1;

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = cos(vector->data[i]);
    }
    return 0;
}

int vec_math_tan_inplace(struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return -1;

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = tan(vector->data[i]);
    }
    return 0;
}


int vec_math_asin_inplace(struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return -1;

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = asin(vector->data[i]);
    }
    return 0;
}

int vec_math_acos_inplace(struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return -1;

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = acos(vector->data[i]);
    }
    return 0;
}

int vec_math_atan_inplace(struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return -1;

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = atan(vector->data[i]);
    }
    return 0;
}

int vec_math_sinh_inplace(struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return -1;

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = sinh(vector->data[i]);
    }
    return 0;
}

int vec_math_cosh_inplace(struct Vector *vector)
{
    if (!vector || !vector->data) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: invalid vector pointer (%s)\n",
                strerror(errno));
        return NULL;
    }

    if (v->size == 0) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: vector size is zero (%s)\n",
                strerror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = cosh(vector->data[i]);
    }
    return 0;
}

int vec_math_tanh_inplace(struct Vector *vector)
{
    if (!vector || !vector->data) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: invalid vector pointer (%s)\n",
                strerror(errno));
        return NULL;
    }

    if (v->size == 0) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: vector size is zero (%s)\n",
                strerror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = tanh(vector->data[i]);
    }
    return 0;
}

int vec_math_loge_inplace(struct Vector *vector)
{
    if (!vector || !vector->data) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: invalid vector pointer (%s)\n",
                strerror(errno));
        return NULL;
    }

    if (v->size == 0) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: vector size is zero (%s)\n",
                strerror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = log(vector->data[i]);
    }
    return 0;
}

int vec_math_log_inplace(struct Vector *vector, double base)
{
    if (!vector || !vector->data) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: invalid vector pointer (%s)\n",
                strerror(errno));
        return NULL;
    }

    if (v->size == 0) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: vector size is zero (%s)\n",
                strerror(errno));
        return NULL;
    }

    double log_base = log(base);

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = log(vector->data[i]) / log_base;
    }
    return 0;
}

int vec_math_exp_inplace(struct Vector *vector)
{
    if (!vector || !vector->data) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: invalid vector pointer (%s)\n",
                strerror(errno));
        return NULL;
    }

    if (v->size == 0) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: vector size is zero (%s)\n",
                strerror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = exp(vector->data[i]);
    }
    return 0;
}

int vec_math_floor_inplace(struct Vector *vector)
{
    if (!vector || !vector->data) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: invalid vector pointer (%s)\n",
                strerror(errno));
        return NULL;
    }

    if (v->size == 0) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: vector size is zero (%s)\n",
                strerror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = floor(vector->data[i]);
    }
    return 0;
}

int vec_math_ceil_inplace(struct Vector *vector)
{
    if(!vector || !vector->data || vector->size == 0) return -1;

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = ceil(vector->data[i]);
    }
    return 0;
}

int vec_math_fmod_inplace(struct Vector *vector, double divisor)
{
    if (!vector || !vector->data) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: invalid vector pointer (%s)\n",
                strerror(errno));
        return NULL;
    }

    if (v->size == 0) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: vector size is zero (%s)\n",
                strerror(errno));
        return NULL;
    }
    if(divisor == 0.0) return NULL;

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = fmod(vector->data[i], divisor);
    }
    return 0;
}

int vec_math_trunc_inplace(struct Vector *vector)
{
    if (!vector || !vector->data) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: invalid vector pointer (%s)\n",
                strerror(errno));
        return NULL;
    }

    if (v->size == 0) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: vector size is zero (%s)\n",
                strerror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = trunc(vector->data[i]);
    }
    return 0;
}

int vec_math_round_inplace(struct Vector *vector)
{
    if (!vector || !vector->data) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: invalid vector pointer (%s)\n",
                strerror(errno));
        return NULL;
    }

    if (vector->size == 0) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: vector size is zero (%s)\n",
                strerror(errno));
        return NULL;
    }

    for(size_t i = 0; i < vector->size; i++){
        vector->data[i] = round(vector->data[i]);
    }
    return 0;
}


/* Elementwise multiplication (Hadamard multiplication)*/

struct Vector *vec_mul(const struct Vector *a, const struct Vector *b)
{
    if(!a || !b || !a->data || !b->data) return NULL;
    if(a->size == 0 || b->size ==0) return NULL;
    if(a->size != b->size) return NULL:

    struct Vector *new_vec = vec_alloc(a->size);
    if(!new_vector){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno));
        return NULL;
    }

    for(size_t i= 0; i < a->size; i++){
        new_vec->data[i] = a->data[i] * b->data[i];
    }

    return new_vec;
}

int vec_mul_inplace(struct Vector *a, const struct Vector *b)
{
    if(!a || !b || !a->data || !b->data) return -1;
    if(a->size != b->size) return -1;

    for(size_t i = 0; i < a->size; i++){
        a->data[i]*=b->size[i];
    }

    return 0;

}



/* BLAS Level-1 */

double vec_dot(const struct Vector *a, const struct Vector *b)
{
    if (!a || !b) return 0.0;
    if (a->size != b->size) return 0.0;

    /*double sum = 0.0;
    for (size_t i = 0; i < a->size; i++) {
        sum += a->data[i] * b->data[i];
    }*/

    return cblas_ddot(
        (int)a->size,     // number of elements
        a->data, 1,       // vector a, stride 1
        b->data, 1        // vector b, stride 1
    );
}


int vec_copy(struct Vector *dest, const struct Vector *src)
{
    if (!dest || !src) return -1;
    if (dest->size != src->size) return -1;

    cblas_dcopy(
        (int)src->size,   // number of elements
        src->data, 1,     // source vector, stride 1
        dest->data, 1     // destination vector, stride 1
    );

    return 0;
}

int vec_scale_inplace(struct Vector *v, double scalar)
{
    if (!v) return -1;
    if (!v->data) return -1;

    cblas_dscal(
        (int)v->size,   // number of elements
        scalar,         // scaling factor
        v->data, 1      // vector data, stride 1
    );

    return 0;
}

int vec_axpy_inplace(struct Vector *y, const struct Vector *x, double a)
{
    /* Y[i] = alpha * X[i] + Y[i] */
    if (!y || !x) return -1;
    if (y->size != x->size) return -1;
    if (!y->data || !x->data) return -1;

    cblas_daxpy(
        (int)y->size,   // number of elements
        a,              // scalar multiplier
        x->data, 1,     // vector x
        y->data, 1      // vector y (modified in-place)
    );

    return 0;
}

double vec_norm2(const struct Vector *v)
{
    /*double sum = 0.0;
    for (size_t i = 0; i < v->size; i++) {
        sum += v->data[i] * v->data[i];
    }*/

    if (!v || !v->data) return 0.0;

    return cblas_dnrm2(
        (int)v->size,   // number of elements
        v->data, 1      // vector data, stride 1
    );
}

double vec_asum(const struct Vector *v)
{

    /*double sum = 0.0;
    for (size_t i = 0; i < v->size; i++) {
        sum += fabs(v->data[i]);
    }*/
    if (!v || !v->data) return 0.0;

    return cblas_dasum(
        (int)v->size,
        v->data, 1
    );
}

int vec_iamax(const struct Vector *v)
{
    /*int idx = 0;
    double max_val = fabs(v->data[0]);
    for (size_t i = 1; i < v->size; i++) {
        double val = fabs(v->data[i]);
            if (val > max_val) {
            max_val = val; idx = (int)i;
        }
    }
    return idx;*/
    if (!v || !v->data) return -1;

    return cblas_idamax(
        (int)v->size,
        v->data, 1
    );
}

/* ====================================================
            Arithmetic operations (BLAS)
   ==================================================== */

struct Vector *vec_add(const struct Vector *a, const struct Vector *b)
{
    if (!a || !b) return NULL;
    if (a->size != b->size) return NULL;

    struct Vector *c = vec_alloc(a->size);
    if(!c){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno)
        );
        return NULL;
    }

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

struct Vector *vec_sub(const struct Vector *a, const struct Vector *b)
{
    if (!a || !b) return NULL;
    if (a->size != b->size) return NULL;

    struct Vector *c = vec_alloc(a->size);
    if(!c){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno)
        );
        return NULL;
    }

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

/* Scalar Functions (out of place)*/

struct Vector *vec_add_scalar(const struct Vector *v, double s)
{
    if (!v || !v->data) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: invalid vector pointer (%s)\n",
                strerror(errno));
        return NULL;
    }

    if (v->size == 0) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: vector size is zero (%s)\n",
                strerror(errno));
        return NULL;
    }

    struct Vector *new_vec = vec_alloc(v->size);
    if(!new_vec){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno)
        );
        return NULL;
    }

    for(size_t i = 0; i < v->size; i++){
        new->data = v->data[i] + s;
    }
    return new_vec;
}

struct Vector *vec_sub_scalar(const struct Vector *v, double s)
{
    if (!v || !v->data) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: invalid vector pointer (%s)\n",
                strerror(errno));
        return NULL;
    }

    if (v->size == 0) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: vector size is zero (%s)\n",
                strerror(errno));
        return NULL;
    }

    struct Vector *new_vec = vec_alloc(v->size);
    if(!new_vec){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno)
        );
        return NULL;
    }

    for(size_t i = 0; i < v->size; i++){
        new->data = v->data[i] - s;
    }
    return new_vec;
}

struct Vector *vec_mul_scalar(const struct Vector *v, double s)
{
    if (!v || !v->data) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: invalid vector pointer (%s)\n",
                strerror(errno));
        return NULL;
    }

    if (v->size == 0) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: vector size is zero (%s)\n",
                strerror(errno));
        return NULL;
    }

    struct Vector *new_vec = vec_alloc(v->size);
    if(!new_vec){
        errno = ENOMEM;
        fprintf(stderr,
                "vec_alloc: failed to allocate Vector struct (%s)\n",
                stderror(errno)
        );
        return NULL;
    }

    for(size_t i = 0; i < v->size; i++){
        new->data = v->data[i] * s;
    }
    return new_vec;
}

struct Vector *vec_div_scalar(const struct Vector *v, double s)
{
    if (!v || !v->data) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: invalid vector pointer (%s)\n",
                strerror(errno));
        return NULL;
    }

    if (v->size == 0) {
        errno = EINVAL;
        fprintf(stderr,
                "vec_div_scalar: vector size is zero (%s)\n",
                strerror(errno));
        return NULL;
    }

    if (s == 0.0) {
        errno = ERANGE;
        fprintf(stderr,
                "vec_div_scalar: division by zero scalar (%s)\n",
                strerror(errno));
        return NULL;
    }

    struct Vector *new_vec = vec_alloc(v->size);
    if (!new_vec) {
        errno = ENOMEM;
        fprintf(stderr,
                "vec_div_scalar: allocation failed (%s)\n",
                strerror(errno));
        return NULL;
    }

    for (size_t i = 0; i < v->size; ++i){
        new_vec->data[i] = v->data[i] / s;
    }

    return new_vec;
}

/* Scalar Functions (inplace) */
int vec_add_scalar_inplace(struct Vector *v, double s)
{
    if (!v) {
        errno = EINVAL;
        fprintf(stderr, "vec_div_scalar_inplace error: vector pointer is NULL\n");
        return -1;
    }

    if (!v->data) {
        errno = EINVAL;
        fprintf(stderr, "vec_div_scalar_inplace error: vector data pointer is NULL\n");
        return -1;
    }

    if (v->size == 0) {
        errno = EINVAL;
        fprintf(stderr, "vec_div_scalar_inplace error: vector size is zero\n");
        return -1;
    }

    for(size_t i = 0; i < v->size; i++){
        v->data[i]+=s;
    }
    return 0;
}

int vec_sub_scalar_inplace(struct Vector *v, double s)
{
    if (!v) {
        errno = EINVAL;
        fprintf(stderr, "vec_div_scalar_inplace error: vector pointer is NULL\n");
        return -1;
    }

    if (!v->data) {
        errno = EINVAL;
        fprintf(stderr, "vec_div_scalar_inplace error: vector data pointer is NULL\n");
        return -1;
    }

    if (v->size == 0) {
        errno = EINVAL;
        fprintf(stderr, "vec_div_scalar_inplace error: vector size is zero\n");
        return -1;
    }

    for(size_t i = 0; i < v->size; i++){
        v->data[i]-=s;
    }
    return 0;
}

int vec_mul_scalar_inplace(struct Vector *v, double s)
{
    if (!v) {
        errno = EINVAL;
        fprintf(stderr, "vec_div_scalar_inplace error: vector pointer is NULL\n");
        return -1;
    }

    if (!v->data) {
        errno = EINVAL;
        fprintf(stderr, "vec_div_scalar_inplace error: vector data pointer is NULL\n");
        return -1;
    }

    if (v->size == 0) {
        errno = EINVAL;
        fprintf(stderr, "vec_div_scalar_inplace error: vector size is zero\n");
        return -1;
    }

    for(size_t i = 0; i < v->size; i++){
        v->data[i]*=s;
    }
    return 0;
}

int vec_div_scalar_inplace(struct Vector *v, double s)
{
    if (!v) {
        errno = EINVAL;
        fprintf(stderr, "vec_div_scalar_inplace error: vector pointer is NULL\n");
        return -1;
    }

    if (!v->data) {
        errno = EINVAL;
        fprintf(stderr, "vec_div_scalar_inplace error: vector data pointer is NULL\n");
        return -1;
    }

    if (v->size == 0) {
        errno = EINVAL;
        fprintf(stderr, "vec_div_scalar_inplace error: vector size is zero\n");
        return -1;
    }

    if (s == 0.0) {
        errno = ERANGE;
        fprintf(stderr, "vec_div_scalar_inplace error: division by zero scalar\n");
        return -1;
    }

    for(size_t i = 0; i < v->size; i++){
        v->data[i]/=s;
    }
    return 0;
}
