/*****************************************************************//**
 * @file   2openmp.cpp
 * @brief  First intro to openmp
 * 
 * @author Hikmat Farhat
 * @date   April 2021
 *********************************************************************/

#include <iostream>
#include <omp.h>

int main()
{
	omp_set_num_threads(4);
#pragma omp parallel 
	{
		int num_threads = omp_get_num_threads();
		
		int id = omp_get_thread_num();
		if (id == 0)std::cout << "actual number of threads " << num_threads << "\n";
		std::cout << "hello from thread "<<id<<"\n";
		}
}
