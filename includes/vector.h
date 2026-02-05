/* vector.h */

#ifndef VECTOR_H
#define VECTOR_H

#include "libs.h"

#define M_PI 3.1415926535897932

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


void dest_vector(struct Vector *vector);
void print_vector(const struct Vector *vector);

/* VECTOR AGGREGATION FUNCTION*/

struct Vector *add_vector(const struct Vector *a, const struct Vector *b);
struct Vector *sub_vector(const struct Vector *a, const struct Vector *b);

#endif
