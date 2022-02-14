// TODO: Insert the 'include' directives.

void* fn_thread(void* arg);

int main(int argc, char** argv)
{
    // Check the number of arguments.
    if (argc < 2)
        errx(EXIT_FAILURE, "The number of threads is missing.");
    else if (argc > 2)
        errx(EXIT_FAILURE, "Specify only the number of threads.");

    // TODO
    // - Convert the argument into a long integer.
    //   Use atol(3).
    //   This value represents the number of threads.
    // - If the argument is not valid (i.e. lower than or equal to zero),
    //   exit with an error message.
    // - Create and execute the threads.
    //   If an error occurs, exit with an error message.
    //   You can use err(3), but the 'errno' variable is not set automatically.
    //   You have to set it manually to the return value of pthread_create().
    // - Finally, there are two possibilities to terminate the main thread :
    //   1. Wait for all the other threads to end.
    //      Return from the function (terminate the process).
    //   2. Do not wait for the other threads.
    //      Terminate the main thread but not the process.
    //   (The two possibilites are right.)
}

// Define the thread function.
void* fn_thread(void* arg __attribute__((unused)))
{
    // TODO
    // - Print a message.
    // - Return from the function.
}