#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

int main() {
    const int samples = 10000000;
    int num_inside_circle = 0;
    double pi_estimate;

    // Seed the random number generator with the current time
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int num_threads = omp_get_max_threads(); // Define it here so it's a global variable accessible to all threads
    int samples_per_thread = samples / num_threads;
    int remainder = samples % num_threads;

    #pragma omp parallel
    {
        // Calculate samples_per_thread and remainder
        
        int thread_id = omp_get_thread_num();
        int start = thread_id * samples_per_thread + std::min(thread_id, remainder);
        int end = start + samples_per_thread + (thread_id < remainder ? 1 : 0);

        int local_circle_points = 0;
        for (int i = start; i < end; i++) {
            double x = static_cast<double>(rand()) / RAND_MAX;
            double y = static_cast<double>(rand()) / RAND_MAX;

            if (x * x + y * y <= 1.0) {
                local_circle_points++;
            }
        }

        // Accumulate local_circle_points without a critical section
        #pragma omp critical
        {
            num_inside_circle += local_circle_points;
        }
    }

    pi_estimate = 4.0 * num_inside_circle / samples;
    std::cout << "Estimated value of pi: " << pi_estimate << std::endl;

    return 0;
}


