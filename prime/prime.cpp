// prime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <omp.h>
using Duration = std::chrono::duration<double, std::milli>;

#define TIMEIT(dur,...)\
   {\
    auto start = std::chrono::high_resolution_clock::now();\
    __VA_ARGS__\
    auto end = std::chrono::high_resolution_clock::now();\
     dur = std::chrono::duration<double, std::milli>(end - start);\
}

const int n = 400000;
const int step = 10000;

int seqPrime(int n) {
    int count = 0;
    for (int j = 2; j < n; ++j) {
        bool p = true;
        for (int i = 2; i < j; ++i) {
            if (j % i == 0) {
                p = false;
                break;
            }
        }
        if (p)count++;
    }
    return count;
}
int seqPrime(int m, int n) {
    int count = 0;
    for (int j = m; j < n; ++j) {
        bool p = true;
        for (int i = 2; i < j; ++i) {
            if (j % i == 0) {
                p = false;
                break;
            }
        }
        if (p)count++;
    }
    return count;
}

int parPrime(int n) {
    int count = 0;
    int i, j;
    bool p;
#pragma omp parallel private(i,p)
    {
        
#pragma omp for reduction(+:count) schedule (dynamic,100)
        for (j = 2; j < n; ++j) {
            p = true;
            for (i = 2; i < j; ++i) {
                if (j % i == 0) {
                    p = false;
                    break;
                }
            }
            if (p)count++;
        }    
    }
    return count;
}
int main()
{
    int total;
    Duration d;
    //const int n = 7920;
  
  /*  TIMEIT(d,
    total=seqPrime(2, step);
    )*/
    //std::cout << "Duration= " << d.count() << "\t" << "total=" << total << "\n";

 //   for (int i = step; i < n; i += step) {
 //       TIMEIT(d,
 //       total = seqPrime(i, i +step);
 //       )
	////std::cout << "Duration= " << d.count() << "\t" << "total=" << total << "\n";

 //   }
    std::cout << "------------------\n";
    omp_set_num_threads(8);
    TIMEIT(d,
       total = parPrime(n);
   )
  std::cout <<"Duration= "<<d.count()<<"\t"<<"total=" << total << "\n";

}
