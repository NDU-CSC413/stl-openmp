/*****************************************************************//**
 * @file   5openmp.cpp
 * @brief  openmp for construct
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
	const int num_steps = 1 << 28;
	const double dx = 1.0 / (double)num_steps;
	const int num_threads = 8;
	omp_set_num_threads(num_threads);
	std::vector<double> results(num_threads);
	double d = omp_get_wtime();
	double res=0;
	double sum = 0;
	double  midpoint;

#pragma omp parallel 
	{
#pragma omp for
			for (int i = 0; i <num_steps; ++i) {
				midpoint = (i + 0.5) * dx;
				sum += 1.0 / (1 + midpoint * midpoint);
			}
	}

	//std::cout << std::accumulate(results.begin(), results.end(), 0.0) << "\n";
	std::cout << 4*sum*dx << "\n";
	d = omp_get_wtime() - d;
	/* duration in ms */
	std::cout << 1000 * d << "\n";
}
