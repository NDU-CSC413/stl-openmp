/*****************************************************************//**
 * \file   BFS.cpp
 * \brief  parallel-ready sequential and parallel implementation
 * of BFS. Integers represent nodes, i.e., node 0, node 1...etc
 *
 * \author hikmat
 * \date   March 2022
 *********************************************************************/

#include <iostream>
#include <list>
#include <vector>
#include <limits>
#include <omp.h>
#include <chrono>

using Duration = std::chrono::duration<double, std::milli>;


#define NOT_APPL -2
#define NOT_SET -1

int num_threads = 4;
// number of nodes
const int m = 1 << 12;
const int n = m * m;
//adjacency list
std::vector<std::list<int>> adj(n);
std::vector<int> parent(n);
std::vector<int> dist(n);


void bfs() {

    std::vector<int> queue;
    queue.push_back(0);
    dist[0] = 0;
    parent[0] = NOT_APPL;
    int d = 1;
    while (!queue.empty()) {
        // for (auto u = queue.begin(); u != queue.end(); ++u) {
#pragma omp parallel num_threads(num_threads)
        {
            //must be here for openmp to create a queue for each thread
            std::vector<int> queueP;
            //at this point we have 8 concurrent threads, each having its own queueP

#pragma omp for 
            for (int i = 0; i < queue.size(); ++i) {
                auto u = queue[i];
                for (auto v = adj[u].begin(); v != adj[u].end(); ++v) {
                    if (parent[*v] == NOT_SET) {
                        parent[*v] = u;
                        dist[*v] = d;
                        queueP.push_back(*v);
                    }
                }
            }//implicit barrier here
            // at this point all threads have finishe processing their assigned range
            // an put the resul in their local queue, queueP
            int id = omp_get_thread_num();
            //if (id == 0)queue.clear();
#pragma omp single
            queue.clear();
//#pragma omp barrier
#pragma omp critical
            {
                queue.insert(queue.end(), queueP.begin(), queueP.end());
            }

        }//end omp parallel. implicit barrier

        d++;
    }
}
int main()
{

     /* mxm graph */
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            if (i != 0)
                adj[j + m * i].push_back(j + m * (i - 1));
            if (i != m - 1)
                adj[j + m * i].push_back(j + m * (i + 1));
            if (j != 0)
                adj[j + m * i].push_back(j - 1 + m * i);
            if (j != m - 1)
                adj[j + m * i].push_back(j + 1 + m * i);

        }
    }
    double total = 0;
    int num_trials = 10;
    for (int t = 0; t < num_trials; ++t) {

		std::fill(dist.begin(), dist.end(), std::numeric_limits<int>::max());
		std::fill(parent.begin(), parent.end(), NOT_SET);

        Duration dur;
        auto clock_start = std::chrono::high_resolution_clock::now();

        bfs();
        auto clock_end = std::chrono::high_resolution_clock::now();
        dur = std::chrono::duration<double, std::milli>(clock_end - clock_start);
        total += dur.count();
    }
    std::cout << "Grid size " << m << "X" << m << "\n";
    std::cout << "Number of threads " << num_threads << "\n";
    std::cout << "Average duration = " << total / num_trials << "\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << "distance of " << i << " is " << dist[i] << "\n";
    }


}
