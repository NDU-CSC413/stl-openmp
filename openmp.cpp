
#include <iostream>
#include <chrono>
#include <thread>

using Duration = std::chrono::duration<double, std::milli>;

#define TIMEIT(dur,...)\
   {\
    auto start = std::chrono::high_resolution_clock::now();\
    __VA_ARGS__\
    auto end = std::chrono::high_resolution_clock::now();\
     dur = std::chrono::duration<double, std::milli>(end - start);\
}
#define LOAD 1
#define TERMS 5
void seq_sin(int n, int terms, float* a, float* b) {
	for (int k = 0; k < LOAD; ++k) {
		for (int i = 0; i < n; ++i) {
			float x = a[i];
			float value = x;
			float num = x * x * x;
			float denom = 6;
			int sign = -1;
			for (int j = 1; j <= terms; ++j) {
				value += sign * num / denom;
				num *= x * x;
				denom *= (2 * j + 2) * (2 * j + 3);
				sign *= -1;
			}
			b[i] = value;
		}
	}
}
void openmp_sin(int n, int terms, float* a, float* b) {
	for (int k = 0; k < LOAD; ++k) {
#pragma omp parallel for
		for (int i = 0; i < n; ++i) {
			float x = a[i];
			float value = x;
			float num = x * x * x;
			float denom = 6;
			int sign = -1;
			for (int j = 1; j <= terms; ++j) {
				value += sign * num / denom;
				num *= x * x;
				denom *= (2 * j + 2) * (2 * j + 3);
				sign *= -1;
			}
			b[i] = value;
		}
		}
}

const int n = 1 << 24;
float a[n], b[n];

#define NUM_TRIALS 10
int main()
{
	Duration d_mp{}, d_seq{}, d{};
  /* dry run*/
	seq_sin(n, TERMS, a, b);
	std::cout << "Array size = " << n << "\n";
	for (int i = 0; i < NUM_TRIALS; ++i) {
		TIMEIT(d,
			openmp_sin(n, TERMS, a, b);
		)
			d_mp += d;
		TIMEIT(d,
				seq_sin(n, TERMS, a, b);
		)
			d_seq += d;

	}
    std::cout << d_mp.count()/NUM_TRIALS << "\n";
	std::cout << d_seq.count() / NUM_TRIALS << "\n";

}
