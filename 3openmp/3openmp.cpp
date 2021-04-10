/*****************************************************************//**
 * @file   3openmp.cpp
 * @brief  Computing PI using openmp
 * 
 * @author Hikmat Farhat
 * @date   April 2021
 *********************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <omp.h>

/* single program multiple data*/
void pi(double stepsize, int from, int to, double& res) {
	double sum = 0, midpoint;
	for (int i = from; i < to; ++i) {
		midpoint = (i + 0.5) * stepsize;
		sum += 1.0 / (1 + midpoint * midpoint);
	}
	res += 4.0 * stepsize * sum;
}

int main()
{
	const int num_steps=1<<28;
	const double dx = 1.0 / (double)num_steps;
	const int num_threads = 8;
	omp_set_num_threads(num_threads);
	std::vector<double> results(num_threads);
	double d=omp_get_wtime();
#pragma omp parallel 
	{

		int nthreads = omp_get_num_threads();
		long long block_size = num_steps /nthreads;
		int idx = omp_get_thread_num();
		pi(dx, idx * block_size, (idx + 1) * block_size,results[idx]);

	}
	
	std::cout<<std::accumulate(results.begin(), results.end(), 0.0)<<"\n";
	d = omp_get_wtime() - d;
	/* duration in ms */
	std::cout << 1000*d << "\n";
}
