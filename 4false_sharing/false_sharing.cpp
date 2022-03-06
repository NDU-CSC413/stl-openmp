/*****************************************************************//**
 * @file   false_sharing.cpp
 * @brief  Computing PI using OpenMP
 * illustrates false sharing
 * IMPORTANT: compile and run on the command line using
 * cl /openmp /EHsc /nologo /DX false_sharing.cpp
 * where X is one of the values below
 * use FALSE_SHARING for X to illustrate
 * use ALIGN for X to align elements on different cache lines
 * use LOCAL for X to use a local copy for each thread
 * @author Hikmat Farhat
 * @date   April 2021
 *********************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <omp.h>

#define LOCAL
//#define FALSE_SHARING
//#define ALIGN


int main()
{
	const int num_steps = 1 << 28;
	const double dx = 1.0 / (double)num_steps;
	const int num_threads =8;
	omp_set_num_threads(num_threads);
#ifdef ALIGN
	alignas(64)
	 double results[num_threads][8]{};
#else 
	double results[num_threads]{};
#endif // ALIGN

	double d = omp_get_wtime();
#pragma omp parallel 
	{
		int nthreads = omp_get_num_threads();
		long long block_size = num_steps / nthreads;
		int idx = omp_get_thread_num();
		if (idx == 0)std::cout << "num threads= " << nthreads << "\n";
		double r = 0.0;
		double midpoint;
		for (int i = idx*block_size; i<(idx+1)*block_size ; ++i) {
			midpoint = (i + 0.5) * dx;
#if defined FALSE_SHARING
			results[idx] += 4.0 / (1 + midpoint * midpoint);
#elif defined ALIGN
		results[idx][0] += 4.0 / (1 + midpoint * midpoint);
#else 
			r+= 4.0 / (1 + midpoint * midpoint);

#endif // FALSE_SHARING

		}
#ifdef LOCAL
		results[idx] = r;

#endif // LOCAL

	}

	double r = 0;
	for (int i = 0; i < num_threads; ++i)
#ifdef ALIGN
		r += results[i][0];
#else
		r += results[i];
#endif // ALIGN
	std::cout << "result " << r*dx << "\n";
	d = omp_get_wtime() - d;
	/* duration in ms */
	std::cout << 1000 * d << "\n";
}
