#include "../includes/vector.h"
#include "../includes/libs.h"

int main() {
    double a[] = {1, 2, 3};
		double b[] = {4, 5, 6};

		struct Vector *va = vec_from_array(a, 3);
		struct Vector *vb = vec_from_array(b, 3);

		struct Vector *vadd = add_vector(va, vb);
		struct Vector *vsub = sub_vector(va, vb);

		print_vector(vadd);
		dest_vector(vadd);
		print_vector(vsub);
		dest_vector(vsub);

		struct Vector *zero = vec_zeros(5);
		print_vector(zero);
		dest_vector(zero);

		struct Vector *ones = vec_ones(5);
		print_vector(ones);
		dest_vector(ones);

		struct Vector *scalar = vec_scalar(5, 100);
		print_vector(scalar);
		dest_vector(scalar);

		struct Vector *arange = vec_arange(5, 0.0, -10.0);
		print_vector(arange);
		dest_vector(arange);

		struct Vector *linspace = vec_linspace(5, 2.0, -1.0);
		print_vector(linspace);
		dest_vector(linspace);

    return 0;
}
