// prime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <thread>
#include <omp.h>
using Duration = std::chrono::duration<double, std::milli>;

#define TIMEIT(dur,...)\
   {\
    auto start = std::chrono::high_resolution_clock::now();\
    __VA_ARGS__\
    auto end = std::chrono::high_resolution_clock::now();\
     dur = std::chrono::duration<double, std::milli>(end - start);\
}

const int n = 800000;
const int step = 10000;

int parPrime(int n) {
   
#pragma omp parallel 
    {
#pragma omp for //schedule (dynamic)
        for (int j = 1; j <=     n; ++j) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100*j));
        }
    }
    return 10;
}
int main()
{
    Duration d;
    int res;
    TIMEIT(d,
        res=parPrime(32);
    )
        std::cout << res << " " << d.count() << "\n";
}
