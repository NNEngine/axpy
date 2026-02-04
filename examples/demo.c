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
	print_vector(vsub);

    return 0;
}
