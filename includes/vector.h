/* vector.h */

#ifndef VECTOR_H
#define VECTOR_H

#include "libs.h"

struct Vector{
	size_t size;
	double *data;
};

struct Vector *vec_alloc(size_t size);
struct Vector *vec_zeros(size_t size);
struct Vector *vec_ones(size_t size);
struct Vector *vec_scalar(size_t size, double scalar);
struct Vector *vec_arange(size_t size, double start, double step);
struct Vector *vec_linspace(size_t size, double start, double end);
struct Vector *vec_from_array(const double *arr, size_t size);


void dest_vector(struct Vector *vector);
void print_vector(const struct Vector *vector);

struct Vector *add_vector(const struct Vector *a, const struct Vector *b);
struct Vector *sub_vector(const struct Vector *a, const struct Vector *b);

#endif
