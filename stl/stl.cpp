// foreach.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <chrono>
#include <execution>
#include <random>
#include <omp.h>
using Duration = std::chrono::duration<double, std::milli>;

#define TIMEIT(dur,...)\
   {\
    auto start = std::chrono::high_resolution_clock::now();\
    __VA_ARGS__\
    auto end = std::chrono::high_resolution_clock::now();\
     dur = std::chrono::duration<double, std::milli>(end - start);\
}

int main()
{
	const int n = 1 << 22;
	std::vector<double> u(n), v(n);
	Duration seq_d,par_d;
	std::fill(v.begin(), v.end(), 1.0);
	/* std::for_each */
	TIMEIT(seq_d,
		std::for_each( v.begin(), v.end(),
			[](double& x) {
				return  x /= 7.0;
			}
		);
	)
	TIMEIT(par_d,
		std::for_each(std::execution::par, v.begin(), v.end(),
			[](double& x) {
				return  x /= 7.0;
			}
		);
	)
	std::cout <<"for_each " <<seq_d.count()<<"\t"<<par_d.count() << "\n";
	/* std::transofrm */
	TIMEIT(seq_d,
		std::transform( v.begin(), v.end(), v.begin(),
			[](double& x) {
				return  x / 7.0;
			}
	);
	)
	TIMEIT(par_d,
			std::transform(std::execution::par, v.begin(), v.end(), v.begin(),
				[](double& x) {
					return  x / 7.0;
				}
			);
	)
	std::cout << "transform " << seq_d.count() << "\t" << par_d.count() << "\n";

	std::random_device rd;
	std::uniform_real_distribution<> dist(1, 10000);
	/* std::generate */
	TIMEIT(seq_d,
		std::generate(u.begin(), u.end(),
		[&]() {
			return dist(rd);
		}
	);
	)

	TIMEIT(par_d,
		std::generate(std::execution::par, v.begin(), v.end(),
		[&]() {
			return dist(rd);
		}
		);
	)
	std::cout << "generate " << seq_d.count() << "\t" << par_d.count() << "\n";
	///* reverse */

	TIMEIT(seq_d,
		std::reverse(u.begin(), u.end());
	)
		TIMEIT(par_d,
			std::reverse(std::execution::par, v.begin(), v.end());
	)
		std::cout << "reverse " << seq_d.count() << "\t" << par_d.count() << "\n";
	/* maximum */
	double seq_max, par_max;
	TIMEIT(seq_d,
		seq_max=*std::max_element(v.begin(), v.end());
	)
		TIMEIT(par_d,
		par_max=*std::max_element(std::execution::par, v.begin(), v.end());
	)
		std::cout << "max_element " << seq_d.count() << "\t" << par_d.count() << "\n";
	if (seq_max != par_max)std::cout << "error\n";

	/* std::sort  */

	TIMEIT(seq_d,
		std::sort(u.begin(), u.end());
	)
	TIMEIT(par_d,
		std::sort(std::execution::par,v.begin(), v.end());
	)
	std::cout << "sort " << seq_d.count() << "\t" << par_d.count() << "\n";
	/* std::count_if*/
	int seq_r,par_r;
	TIMEIT(seq_d,
		seq_r=std::count_if(u.begin(), u.end(),
			[](double x) {
				int i = x;
				return (i % 2 == 0);
			}
			);
	)
		TIMEIT(par_d,
			par_r=std::count_if(std::execution::par,u.begin(), u.end(),
				[](double x) {
					int i = x;
					return (i % 2 == 0);
				}
	);
	)
		std::cout << "count_if " << seq_d.count() << "\t" << par_d.count()<<"\n";
	if (seq_r != par_r)std::cout << "error\n";
}
