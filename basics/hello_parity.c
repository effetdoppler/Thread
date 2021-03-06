#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <err.h>




void* fn_thread(void* arg);

int main(int argc, char** argv)
{
    // Check the number of arguments.
    if (argc < 2)
        errx(EXIT_FAILURE, "The number of threads is missing.");
    else if (argc > 2)
        errx(EXIT_FAILURE, "Specify only the number of threads.");
    // - Convert the argument into a long integer.
    //   Use atol(3).
    //   This value represents the number of threads.
    long nb = atol(argv[1]);
    // - If the argument is not valid (i.e. lower than or equal to zero),
    //   exit with an error message.
    if (nb <= 0)
        errx(EXIT_FAILURE, "The number of threads is not valid.");
    // - Create and execute the threads.
    
    pthread_t thr[nb];
    for (long i = 0; i < nb; i++)
    {
        int e;
        if (i % 2 == 0)
            e = pthread_create(&thr[i], NULL, fn_thread, "Hello from an even thread.");
        else
            e = pthread_create(&thr[i], NULL, fn_thread, "Hello from an odd thread.");
        //   If an error occurs, exit with an error message.
        //   You can use err(3), but the 'errno' variable is not set automatically.
        //   You have to set it manually to the return value of pthread_create().
        if (e!=0)
        {
            errno = e;
            err(EXIT_FAILURE, "pthread_create()");
        }

    }
    for (long i = 0; i < nb; i++)
        pthread_join(thr[i], NULL);
    
   
    // - Finally, there are two possibilities to terminate the main thread :
    //   1. Wait for all the other threads to end.
    //      Return from the function (terminate the process).
    return EXIT_SUCCESS;
    
    //   2. Do not wait for the other threads.
    //      Terminate the main thread but not the process.
    //   (The two possibilites are right.)
}

// Define the thread function.
void* fn_thread(void* arg)
{
    // - Print a message.
    // - Return from the function.
    sleep(rand() % 3);
    char *message = arg;
    printf("%s\n", message);
    return NULL;
}