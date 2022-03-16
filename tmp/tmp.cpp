#include <iostream>
#include <algorithm>
#include <random>
#include <vector>

int n = 1 << 10;

template <typename Iter>
Iter partition(Iter start, Iter end) {
    float pivot_value = *start;
    auto i = start,j = end - 1;
    while (i != j) {
        while (i != j && *j > pivot_value)--j;
        while (i != j && *i <= pivot_value)++i;
        //
        std::iter_swap(i, j);
    }

    std::iter_swap(start, i);
    return i;

}

template<typename Iter>
void quickSort(Iter start,Iter end){

    auto d = std::distance(start, end);
    if (d <= cutoff)
        std::sort(start, end);

    mid = partition(start, end);
    quickSort(start, mid);
    quickSort(mid, end);
}

int main() {
   /* std::vector<float> v(n);
    std::random_device rd;
    std::uniform_real_distribution<float> dist(1, 1000);
    std::generate(v.begin(), v.end(),
        [&]() {
            return dist(rd);
        }

    );*/
    std::vector<float> v{ 15,2,8,33,5,45,24,80,9 };
    auto mid = partition(v.begin(), v.end());
    for (auto x : v)
        std::cout << x << ",";
    std::cout << "\n";

}


