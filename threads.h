#pragma once

#include <iostream>
#include <thread>
#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <random>

using std::cout;
using std::endl;

void printArray(const int *array, const size_t arraySize) {
    if (arraySize <= 100) {
        for (int i = 0; i < arraySize; ++i) {
            cout << array[i] << "\t";
        }
        cout << endl;
    }
}

bool isSorted(const int *array, size_t arraySize) {
    for (size_t i = 0; i + 1 < arraySize; ++i) {
        if (array[i] > array[i + 1])
            return false;
    }
    return true;
}

void ShellSortParallelStep(int *array, size_t arraySize, size_t startPos, size_t endPos, size_t step) {

    for (size_t currentPos = startPos; currentPos < endPos; currentPos++) {
        size_t n = arraySize - currentPos;
        int *a = array + currentPos;
        for (int j = step; j < n; j += step) {
            int key = a[j];
            int i = static_cast<int>(j - step);
            while ((i >= 0) && (a[i] > key)) {
                a[i + step] = a[i];
                i -= step;
            }
            a[i + step] = key;
        }
    }
}

void ShellSortParallel(int *array, size_t arraySize, size_t maxThreadCount) {
    size_t step = arraySize / 2;

    //  "группы" - окошки с числами,
    //        находящиеся на расстоянии step друг от друга, отмечены знаком "/\"
    //  "группы", переданные одному потоку отмечены знаком "!"
    //  |---|---|---|---|---|---|---|---|---|
    //  | ! | ! |   | ! | ! |   | ! | ! |   | . . .
    //  |---|---|---|---|---|---|---|---|---|
    //    /\          /\          /\

    while (step > 0) {
        size_t threadCount = step;
        size_t offsetsPerThread = 1; //сколько "групп" чисел будет обрабатывать один поток
        if (step > maxThreadCount) {
            threadCount = maxThreadCount;
            offsetsPerThread = (step - 1) / threadCount + 1; //округляем кол-во "групп" в большую сторону
        }

        size_t startOffset = 0;
        auto **Threads = new std::thread *[threadCount];
        for (size_t i = 0; i < threadCount; ++i) {
            size_t endOffset = startOffset + offsetsPerThread;
            if (endOffset > step) {
                endOffset = step;
            }
            Threads[i] = new std::thread(ShellSortParallelStep, array, arraySize, startOffset, endOffset, step);
            startOffset += offsetsPerThread;
        }

        for (size_t i = 0; i < threadCount; ++i) {
            Threads[i]->join();
            delete Threads[i];
            Threads[i] = nullptr;
        }
        delete[] Threads;

        step /= 2;
    }
}


