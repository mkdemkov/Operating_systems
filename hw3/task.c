#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int fibonacciNumber(int n) {
    if (n <= 1)
        return n;
    return fibonacciNumber(n - 1) + fibonacciNumber(n - 2);
}

int factorial(int n) {
    if (n == 0)
        return 1;
    return n * factorial(n - 1);
}

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]); // get the argument

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        printf("Factorial of %d is %d\n", n, factorial(n));
        exit(0);
    } else {
        // Parent process
        wait(NULL);
        printf("Fibonacci number of %d is %d\n", n, fibonacciNumber(n));
    }
    return 0;
}
