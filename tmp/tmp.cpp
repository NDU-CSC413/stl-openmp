// tmp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <omp.h>

void pi(double stepsize,int start,int end, double& res) {
	double midpoint,sum=0;
	for (int i = start; i < end; ++i) {
		midpoint = (i + 0.5) * stepsize;
		sum +=1.0 / (1.0 + midpoint * midpoint);
	}
	res = 4 * sum*stepsize;
}

int main()
{
	const int num_steps = 1 << 28;
	const double stepsize = 1.0 / num_steps;
	const int num_threads = 2;
	//omp_set_num_threads(num_threads);
	std::vector<double> results(num_threads);
	double clock_start = omp_get_wtime();
#pragma omp parallel num_threads(num_threads)
	{

		int block_size = num_steps / num_threads;
		int id = omp_get_thread_num();
		pi(stepsize, id * block_size, (id + 1) * block_size, results[id]);
		
		
	}// implicit barrier 

	std::cout << std::accumulate(results.begin(), results.end(), 0.0) << "\n";
	double clock_end = omp_get_wtime();
	std::cout << "duration = " << 1000*(clock_end - clock_start) << "\n";

}

