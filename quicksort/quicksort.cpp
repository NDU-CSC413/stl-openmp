/*****************************************************************//**
 * @file   quicksort.cpp
 * @brief  quicksort using openmp tasks
 * 
 * @author Hikmat Farhat
 * @date   April 2021
 *********************************************************************/

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <omp.h>
#include <chrono>
#include <execution>

using Duration = std::chrono::duration<double, std::milli>;

#define TIMEIT(dur,...)\
   {\
    auto clock_start = std::chrono::high_resolution_clock::now();\
    __VA_ARGS__\
    auto clock_end = std::chrono::high_resolution_clock::now();\
     dur = std::chrono::duration<double, std::milli>(clock_end - clock_start);\
}
const int n = 1 << 24;
const int cutoff = 1 << 18;
template <typename Iter>
Iter partition(Iter start, Iter end) {
	/* assume the input is random and select the
	* first element as pivot
	*/
	double pivot_value = *start;
	auto i = start, j = end - 1;
	while (i != j) {
		while (i != j && pivot_value < *j) --j;
		while (i != j && *i <= pivot_value) ++i;
		std::iter_swap(i, j);
	}
	std::iter_swap(start, i);
	return i;
}

template<typename Iter>
void seqQuicksort(Iter start, Iter end) {
	{
			auto d = std::distance(start, end);
			if (d <= cutoff) {
				std::sort(start, end);
			}
			else {
				Iter mid = partition(start, end);
#pragma omp task
				seqQuicksort(mid + 1, end);
#pragma omp task
				seqQuicksort(start, mid);
			}
	}
}
int main()
{
	
	std::vector<float> v(n);

	std::random_device rd;
	std::uniform_real_distribution<float> dist(1, 100);
	std::generate(v.begin(), v.end(),
		[&]() {
			return dist(rd);
		}
		);
	std::vector<float> u = v;
	std::cout << "omp start\n";
	double d = omp_get_wtime();
#pragma omp parallel
	{
#pragma omp single
		seqQuicksort(v.begin(), v.end());
	}
	d = omp_get_wtime() - d;

	std::cout <<"is sorted="<< std::is_sorted(v.begin(), v.end());
	std::cout << "\n" << 1000*d << "\n";
	Duration dur;

	TIMEIT(dur,
		std::sort(std::execution::par, u.begin(), u.end());
	)
		std::cout << std::is_sorted(u.begin(), u.end());
	std::cout << "std::sort(parallel) " << dur.count() << "\n";
}
