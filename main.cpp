#include "threads.h"

int main(int argc, char **argv) {
    char *SizeCh = argv[1];
    char *end;
    char *NumThreadsCh = argv[2];
    size_t arraySize = strtol(SizeCh, &end, 10);
    size_t maxThreadCount = strtol(NumThreadsCh, &end, 10);
    cout << "Size of array = " << arraySize << endl;
    cout << "Number of threads = " << maxThreadCount << endl;
    size_t maxValue = 100, processorCores = std::thread::hardware_concurrency();
    if (maxThreadCount > processorCores) maxThreadCount = processorCores;
    int *array = new int[arraySize];
    srand(time(nullptr));

    for (int i = 0; i < arraySize; ++i) {
        array[i] = static_cast<int>(rand() % maxValue + 1);
    }

    printArray(array, arraySize);
    auto startTime = std::chrono::steady_clock::now(); // начальное время

    cout << "Sort started\n";

    ShellSortParallel(array, arraySize, maxThreadCount);

    cout << "Sort ended\n";

    auto endTime = std::chrono::steady_clock::now(); // конечное время
    std::chrono::duration<double> sortingTime = endTime - startTime;
    printArray(array, arraySize);
    if (isSorted(array, arraySize)) {
        cout << "Array is sorted :)\n";
    } else {
        cout << "Array is not sorted :(\n";
    }
    cout << "Sorting time : " << sortingTime.count() << " seconds.\n";
    delete[] array;
    return 0;
}
