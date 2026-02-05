/* vector.h */

#ifndef VECTOR_H
#define VECTOR_H

#include "libs.h"

struct Vector{
	size_t size;
	double *data;
};

/* VECTOR CREATION FUNCTIONS*/
struct Vector *vec_alloc(size_t size);
struct Vector *vec_zeros(size_t size);
struct Vector *vec_ones(size_t size);
struct Vector *vec_scalar(size_t size, double scalar);
struct Vector *vec_arange(size_t size, double start, double step);
struct Vector *vec_linspace(size_t size, double start, double end);
void axpy_init_rng(void);
struct Vector *vec_rand(size_t size, double lower_limit, double upper_limit);
struct Vector *vec_randn(size_t, double mean, double variance);
struct Vector *vec_from_array(const double *arr, size_t size);

/* Destruction / print vector*/

void dest_vector(struct Vector *vector);
void print_vector(const struct Vector *vector);

/* VECTOR AGGREGATION FUNCTION*/
double vec_aggr_sum(const struct Vector *vector);
double vec_aggr_mean(const struct Vector *vector);
double vec_aggr_min(const struct Vector *vector);
double vec_aggr_max(const struct Vector *vector);
int vec_aggr_argmin(const struct Vector *vector);
int vec_aggr_argmax(const struct Vector *vector);


/* math Functions  (Out Of Place)*/
struct Vector *vec_math_pow(const struct Vector *vector, double power);
struct Vector *vec_math_sqrt(const struct Vector *vector);
struct Vector *vec_math_cbrt(const struct Vector *vector);
struct Vector *vec_math_sin(const struct Vector *vector);
struct Vector *vec_math_cos(const struct Vector *vector);
struct Vector *vec_math_tan(const struct Vector *vector);
struct Vector *vec_math_asin(const struct Vector *vector);
struct Vector *vec_math_acos(const struct Vector *vector);
struct Vector *vec_math_atan(const struct Vector *vector);
struct Vector *vec_math_sinh(const struct Vector *vector);
struct Vector *vec_math_cosh(const struct Vector *vector);
struct Vector *vec_math_tanh(const struct Vector *vector);
struct Vector *vec_math_loge(const struct Vector *vector);
struct Vector *vec_math_log(const struct Vector *vector, double base);
struct Vector *vec_math_exp(const struct Vector *vector);
struct Vector *vec_math_floor(const struct Vector *vector);
struct Vector *vec_math_ceil(const struct Vector *vector);
struct Vector *vec_math_fmod(const struct Vector *vector, double divisor);
struct Vector *vec_math_trunc(const struct Vector *vector);
struct Vector *vec_math_round(const struct Vector *vector);


/* math Functions  (inplace)*/
int vec_math_pow_inplace(struct Vector *vector, double power);
int vec_math_sqrt_inplace(struct Vector *vector);
int vec_math_cbrt_inplace(struct Vector *vector);
int vec_math_sin_inplace(struct Vector *vector);
int vec_math_cos_inplace(struct Vector *vector);
int vec_math_tan_inplace(struct Vector *vector);
int vec_math_asin_inplace(struct Vector *vector);
int vec_math_acos_inplace(struct Vector *vector);
int vec_math_atan_inplace(struct Vector *vector);
int vec_math_sinh_inplace(struct Vector *vector);
int vec_math_cosh_inplace(struct Vector *vector);
int vec_math_tanh_inplace(struct Vector *vector);
int vec_math_loge_inplace(struct Vector *vector);
int vec_math_log_inplace(struct Vector *vector, double base);
int vec_math_exp_inplace(struct Vector *vector);
int vec_math_floor_inplace(struct Vector *vector);
int vec_math_ceil_inplace(struct Vector *vector);
int vec_math_fmod_inplace(struct Vector *vector, double divisor);
int vec_math_trunc_inplace(struct Vector *vector);
int vec_math_round_inplace(struct Vector *vector);

/* Arithmetic Functions */
struct Vector *add_vector(const struct Vector *a, const struct Vector *b);
struct Vector *sub_vector(const struct Vector *a, const struct Vector *b);

#endif
