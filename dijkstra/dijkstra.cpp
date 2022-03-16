// dijkstra.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <limits>
#include <random>
#include <omp.h>
std::random_device rd;
std::uniform_int_distribution<> distribution(1, 6);


#define INFTY 100
const int n = 1<<14;
const int m = n * n;
std::vector<int> edge_cost(m);
//std::vector<int> edge_cost{
//    0,2,7,6,
//    2,0,1,8,
//    7,1,0,1,
//    6,8,1,0
//
//};
std::vector<int> dist(n);
std::vector<int> notdone(n);
int min_idx;
int min_dist;

void init() {
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            if (i == j)edge_cost[i*n+j] = 0;//distance to self is 0
            else {
                //cost of edge (i,j)
                //make sure the generated numbers are < INFINITY
                edge_cost[i * n + j] = distribution(rd);
                edge_cost[j * n + i] = edge_cost[i * n + j];
            }

        }
    }
    for (int i = 0; i < n; ++i) {
        notdone[i] = 1;
        dist[i] = INFTY;
    }
    dist[0] = 0;
    //notdone[0] = 0;

}
void dowork() {
    omp_set_num_threads(4);
#pragma omp parallel
    {
        int local_min_idx;
        unsigned int local_min_dist;

        for (int step = 0; step < n; ++step) {
#pragma omp single
            {
                min_idx = 0;
                min_dist = INFTY;
            }
            local_min_dist = INFTY;

#pragma omp for nowait
            for (int i = 0; i < n; ++i) {
                if (notdone[i] && dist[i] < local_min_dist) {
                    local_min_dist = dist[i];
                    local_min_idx = i;
                }
            }
#pragma omp critical
            {
                if (local_min_dist < min_dist) {
                    min_dist = local_min_dist;
                    min_idx = local_min_idx;
                }
            }
#pragma omp barrier

#pragma omp single
            {
                notdone[min_idx] = 0;
            }
#pragma omp for
            for (int i = 1; i < n; ++i) {
                if (dist[min_idx] + edge_cost[min_idx * n + i] < dist[i])
                    dist[i] = dist[min_idx] + edge_cost[min_idx * n + i];
            }

        }
    }
        
}
int main()
{
    init();
   /* for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << "d[" << i << "," << j << "]=" << edge_cost[i * n + j] << "\n";
        }
    }*/
    std::cout << "Running Dijkstra\n";
    double d = omp_get_wtime();
    dowork();
  /*  std::cout << "distances\n";
    for (int i = 0; i < n; ++i)
        std::cout << i << ", dist=" << dist[i] << "\n";*/
    d = omp_get_wtime() - d;
    /* duration in ms */
    std::cout << 1000 * d << "\n";
}

