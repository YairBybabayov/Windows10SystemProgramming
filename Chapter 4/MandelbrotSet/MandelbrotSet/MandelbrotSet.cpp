#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include<complex.h>
#include <memory>

#define WIDTH 120
#define HEIGHT 120
#define MAX_ITER 100

typedef struct {
    int thread_id;
    int start_row;
    int end_row;
    int(*output)[WIDTH]; // Pointer to the output array
} ThreadData;

// Function to compute the magnitude of a complex number
double complex_magnitude(_Dcomplex c) {
    return sqrt(c._Val[0] * c._Val[0] + c._Val[1] * c._Val[1]);
}

// Function to calculate the Mandelbrot set
int mandelbrot(_Dcomplex c) {
    _Dcomplex z = { {0.0, 0.0} };  // Initialize z to (0, 0)
    int n = 0;

    while (complex_magnitude(z) <= 2.0 && n < MAX_ITER) {
        double temp_real = z._Val[0] * z._Val[0] - z._Val[1] * z._Val[1] + c._Val[0];
        z._Val[1] = 2.0 * z._Val[0] * z._Val[1] + c._Val[1];
        z._Val[0] = temp_real;
        n++;
    }
    return n;
}

// Thread function to calculate the Mandelbrot set for a range of rows
DWORD WINAPI calculate_mandelbrot(PVOID arg) {
    auto data = static_cast<ThreadData*>(arg);

    for (int y = data->start_row; y < data->end_row; y++) {
        for (int x = 0; x < WIDTH; x++) {
            double real = (x / (double)WIDTH) * 3.0 - 2.0; // Scale to [-2, 1]
            double imag = (y / (double)HEIGHT) * 3.0 - 1.5; // Scale to [-1.5, 1.5]
            _Dcomplex c = { {real, imag} };

            int iter = mandelbrot(c);

            // Store result in output array
            data->output[y][x] = (iter == MAX_ITER) ? 0 : 1; // 0 for inside the set, 1 for outside
        }
    }

    return 0;
}

// Function to display the Mandelbrot set as ASCII art
void display_mandelbrot(int output[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (output[y][x] == 0) {
                printf("#");
            }
            else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void save_ppm(const char* filename, int output[HEIGHT][WIDTH]) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "w"); // Use fopen_s for safety
    if (err != 0 || file == NULL) {
        perror("Unable to open file for writing");
        return;
    }

    // PPM header
    fprintf(file, "P3\n%d %d\n255\n", WIDTH, HEIGHT);

    // Write pixel data
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int color = (output[y][x] == MAX_ITER) ? 0 : (255 - (output[y][x] * 255 / MAX_ITER)); // Mapping to grayscale
            fprintf(file, "%d %d %d ", color, color, color); // Grayscale
        }
        fprintf(file, "\n");
    }

    fclose(file);
}


int main() {
    int num_threads;
    printf("Enter number of threads: ");
    scanf_s("%d", &num_threads);

    // Allocate output array
    int output[HEIGHT][WIDTH] = { 0 };

    // Create thread data
    auto thread_data = std::make_unique<ThreadData[]>(num_threads);
    auto threads = std::make_unique<HANDLE[]>(num_threads);

    // Calculate rows per thread
    int rows_per_thread = HEIGHT / num_threads;

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].start_row = i * rows_per_thread;
        thread_data[i].end_row = (i == num_threads - 1) ? HEIGHT : (i + 1) * rows_per_thread; // Last thread handles the remainder
        thread_data[i].output = output;

        threads[i] = CreateThread(NULL, 0, calculate_mandelbrot, &thread_data[i], 0, NULL);
    }

    // Wait for threads to complete
    WaitForMultipleObjects(num_threads, threads.get(), TRUE, INFINITE);

    // Display the results
    display_mandelbrot(output);
    save_ppm("mandelbrot.ppm", output);

    // Clean up
    for (int i = 0; i < num_threads; i++) {
        CloseHandle(threads[i]);
    }

    return 0;
}
