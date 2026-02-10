# <div align = "center">axpy</div>

<img width="2560" height="1440" alt="Let’s Colonize (1)" src="https://github.com/user-attachments/assets/784e8b55-77ba-4496-a7b7-186c1a9a0cc3" />


**axpy** is a high-performance BLAS-backed vector mathematics library written in C, designed for scientific computing, numerical analysis, and machine learning applications. The library provides efficient vector creation, aggregation, elementwise mathematics, and BLAS Level-1 operations using OpenBLAS or compatible BLAS implementations.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)


## Features

* ✨ Efficient vector creation utilities (zeros, ones, linspace, random, etc.)
* 🚀 BLAS-accelerated vector arithmetic and dot products
* 📦 Comprehensive mathematical and aggregation operations
* 🔧 In-place and out-of-place computation APIs for performance control


## Installation

### Requirements

* GCC / Clang
* OpenBLAS (or compatible BLAS library)

### Build using Makefile

```bash
git clone https://github.com/NNEngine/axpy.git
cd axpy
make
```


## Quick Start

```c
#include "vector.h"

int main()
{
    struct Vector *a = vec_ones(5);
    struct Vector *b = vec_scalar(5, 2.0);

    struct Vector *c = vec_add(a, b);

    print_vector(c);

    dest_vector(a);
    dest_vector(b);
    dest_vector(c);

    return 0;
}
```

Compile example:

```bash
gcc -Iincludes src/vector.c examples/demo.c -lopenblas -o demo
./demo
```

## Documentation

### Core API Example

#### `vec_add(const struct Vector *a, const struct Vector *b)`

Performs elementwise addition of two vectors.

**Parameters:**

* `a` : Pointer to first vector
* `b` : Pointer to second vector

**Returns:**

* Newly allocated vector containing the result

**Example:**

```c
struct Vector *c = vec_add(a, b);
```


## Development

### Setup

```bash
git clone https://github.com/{USERNAME}/c_vector_blas.git
cd c_vector_blas
make
```

### Running Example

```bash
make demo
./demo
```

### Static Analysis (recommended)

```bash
gcc -Wall -Wextra -pedantic
```

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m "Add amazing feature"`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request


## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.


## Roadmap

* Statistical vector operations
* Masking / filtering utilities
* Sorting and ranking functions
* BLAS Level-2 matrix–vector support
