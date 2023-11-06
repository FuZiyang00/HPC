#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono> 

int binarySearch(const std::vector<int>& arr, const int target, int start, int end) {
    int left = start;
    int right = end;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return mid;
        }
        else if (arr[mid] < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return -1; // Element not found
}

int parallelBinarySearch(const std::vector<int>& arr, int target) {
    int numThreads = omp_get_max_threads();
    int n = arr.size();
    int chunkSize = n / numThreads;
    int remainder = n % numThreads;

    int foundIndex = -1; // Initialize to -1 if not found

    #pragma omp parallel
    {
        int threadId = omp_get_thread_num();
        int start = threadId * chunkSize + std::min(threadId, remainder); // adds to the start index the min between id and remainder
        int end = start + chunkSize + (threadId < remainder ? 1 : 0); // if the index is smaller than reminder we should add 1 to the end index
        int result = binarySearch(arr, target, start, end);

        #pragma omp critical
        {
            if (result != -1) {
                foundIndex = result; // Store the result from the first thread that finds the target
            }
        }
    }

    return foundIndex;
}

int main() {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 33, 44, 55};
    int target = 7;
    std::cout<< "parallel execution"<<std::endl;
    // Start measuring time
    auto start_time = std::chrono::high_resolution_clock::now();

    int result = parallelBinarySearch(arr, target);

    // Stop measuring time
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    if (result != -1) {
        std::cout << "Element " << target << " found at index " << result << std::endl;
    } else {
        std::cout << "Element " << target << " not found in the array." << std::endl;
    }
    // Print the execution time
    std::cout << "Time taken for binary search: " << duration.count() << " microseconds" << std::endl;
}