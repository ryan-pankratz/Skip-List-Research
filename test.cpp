#include <iostream>    
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include "SkipList.h"
// #include "array.h"
#include "LinkedArrayList.h"

#include <chrono>
using namespace std::chrono;

int main() {

    srand(time(nullptr));
    int n = 1000000;

    int arr[n];
    float array[n];
    SkipList* sl = new SkipList();
    LinkedArrayList* lal = new LinkedArrayList();

    for (int i = 0; i < n; i++) {
        arr[i] = rand() % n + 1; // Random number between 1 and n
    }

    std::copy(arr, arr + n, array);

    std::cout << "---- Skip List runtime ----\n";

    auto start = high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        sl->insert(arr[i]);
    }

    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start);

    std::cout << "Total Time: ";
    std::cout << (float) ((float) duration.count() / 1000000);
    std::cout << "\n";

    std::cout << "Average Time: ";
    std::cout << (float) (((float) (duration.count() / 1000000)) / n);
    std::cout << "\n";

    std::cout << "---------------------------\n";

    std::cout << "\n";

    std::cout << "----  Sorting runtime  ----\n";

    start = high_resolution_clock::now();

    int len = n;

    std::sort(array, array + len);

    end = high_resolution_clock::now();

    duration = duration_cast<microseconds>(end - start);

    std::cout << "Total Time: ";
    std::cout << (float) ((float) duration.count() / 1000000);
    std::cout << "\n";

    std::cout << "Average Time: ";
    std::cout << (float) (((float) (duration.count() / 1000000)) / n);
    std::cout << "\n";

    std::cout << "---------------------------\n";

    std::cout << "---- LinkedArrList Runtime ----\n";

    start = high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        lal->insert(arr[i]);
    }

    end = high_resolution_clock::now();

    duration = duration_cast<microseconds>(end - start);

    std::cout << "Total Time: ";
    std::cout << (float) ((float) duration.count() / 1000000);
    std::cout << "\n";

    std::cout << "Average Time: ";
    std::cout << (float) (((float) (duration.count() / 1000000)) / n);
    std::cout << "\n";

    std::cout << "---------------------------\n";

    std::cout << "---- LinkedArrList Erase Runtime ----\n";

    start = high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        lal->erase(arr[i]);
    }

    end = high_resolution_clock::now();

    duration = duration_cast<microseconds>(end - start);

    std::cout << "Total Time: ";
    std::cout << (float) ((float) duration.count() / 1000000);
    std::cout << "\n";

    std::cout << "Average Time: ";
    std::cout << (float) (((float) (duration.count() / 1000000)) / n);
    std::cout << "\n";

    std::cout << "---------------------------\n";

    std::cout << "---- Skip List Erase Runtime ----\n";

    start = high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        sl->erase(arr[i]);
    }

    end = high_resolution_clock::now();

    duration = duration_cast<microseconds>(end - start);

    std::cout << "Total Time: ";
    std::cout << (float) ((float) duration.count() / 1000000);
    std::cout << "\n";

    std::cout << "Average Time: ";
    std::cout << (float) (((float) (duration.count() / 1000000)) / n);
    std::cout << "\n";

    std::cout << "---------------------------\n";

    std::cout << "\n";

    std::cout << "----  Array Erase Runtime  ----\n";

    start = high_resolution_clock::now();

    len = n;

    for (int i = 0; i < n; i++) {
        removeVal(array, arr[i], len);
        len--;
    }

    end = high_resolution_clock::now();

    duration = duration_cast<microseconds>(end - start);

    std::cout << "Total Time: ";
    std::cout << (float) ((float) duration.count() / 1000000);
    std::cout << "\n";

    std::cout << "Average Time: ";
    std::cout << (float) (((float) (duration.count() / 1000000)) / n);
    std::cout << "\n";

    std::cout << "---------------------------\n";

    std::cout << "---- LinkedArrList Runtime ----\n";

    start = high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        lal->insert(arr[i]);
    }

    end = high_resolution_clock::now();

    duration = duration_cast<microseconds>(end - start);

    std::cout << "Total Time: ";
    std::cout << (float) ((float) duration.count() / 1000000);
    std::cout << "\n";

    std::cout << "Average Time: ";
    std::cout << (float) (((float) (duration.count() / 1000000)) / n);
    std::cout << "\n";

    std::cout << "---------------------------\n";

    sl->deleteAll();
    delete sl;

    lal->deleteAll();
    delete lal;

    return 0;
}

