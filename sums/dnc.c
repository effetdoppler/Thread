#include <err.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>

#define INITIALIZE_ARRAY

void * worker(void *arg);

struct thread_data
{
    unsigned char *start;       // Points to the first element to add.
    long size;                  // Number of elements to add.
    long threshold;             // Threshold.
    unsigned long sum;          // Sum of the elements.
};

// Return the sum of an array of bytes.
// 'start' points to the first element of an array.
// 'size' is the number of elements of the array.
unsigned long linear_sum(unsigned char *start, long size)
{
    long sum = 0;
    for(long i = 0; i < size; i++)
    {
        sum += *(start+i);
    }
    return sum;
}

unsigned long dnc_sum(unsigned char *start, long size, long threshold)
{
    // TODO
    // Implement the divide-and-conquer algorithm.
    if (size <= threshold)
        return linear_sum(start, size);

    long size1 = size / 2;
    struct thread_data thread;
    thread.size = size1;
    thread.start = start;
    thread.threshold = threshold;
    
    long size2 = size - size1;
    unsigned char* mid = start + size1;
    pthread_t thr;
    pthread_create(&thr, NULL, worker, (void*)&thread);  // Use a thread.
    unsigned long s2 = dnc_sum(mid, size2, threshold);

    // Wait for s1.
    pthread_join(thr, NULL);

    return thread.sum + s2;
}

// Counter of threads.
atomic_int thread_count = 1;

// The thread function.
void * worker(void *arg)
{
    // Increment the counter of threads.
    atomic_fetch_add(&thread_count, 1);

    // TODO
    // - Get the thread data passed as parameters.
    struct thread_data *thread = (struct thread_data*)arg;
    // - Call dnc_sum().
    unsigned long sum = dnc_sum(thread->start, thread->size, thread->threshold);
    //   (It may execute recursively another thread.)
    // - Store the result in the 'sum' field.
    thread->sum = sum;
    // - Return from the function.
    return NULL;
}

int main(int argc, char **argv)
{
    // Get the arguments.
    // argv[1] = array_size = the size of the array of bytes (greater than 63).
    // argv[2] = thread_number = the minimum number of threads (between 1 and 16).
    // -----------------------------------------------------------------------

    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: array_size thread_number");

    long array_size = atol(argv[1]);
    if (array_size <= 63)
        errx(EXIT_FAILURE, "The size of the array is not valid (must be greater than 63).");

    long thread_number = atol(argv[2]);
    if (thread_number < 1 || thread_number > 16)
        errx(EXIT_FAILURE, "The number of threads is not valid (between 1 and 16).");

    // -----------------------------------------------------------------------

    // Allocate the array of bytes.
    unsigned char *bytes = malloc(array_size);
    if (bytes == NULL)
        errx(EXIT_FAILURE, "Not enough memory!\nTry to reduce the size of the array.");

#ifdef INITIALIZE_ARRAY
    // Initialize the array.
    printf("Initializing array.. ");
    fflush(stdout);
    for (long i = 0; i < array_size; i++)
        bytes[i] = 1;
    printf("OK\n");
#endif

    // Print the sum and the number of threads.
    printf("Sum................. %lu\n", dnc_sum(bytes, array_size, 1 + array_size / thread_number));
    printf("Number of threads... %i\n", atomic_load(&thread_count));

    // Free the array and exit.
    free(bytes);
    return EXIT_SUCCESS;
}