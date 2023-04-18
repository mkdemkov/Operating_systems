#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

pid_t transmitter_pid;
int received_bits = 0;
int received_number = 0;

void sigusr1_handler(int sig)
{
    received_bits++;
    received_number |= (0 << (received_bits - 1));  // установим текущий бит нулевым
    kill(transmitter_pid, SIGUSR1);
}

void sigusr2_handler(int sig)
{
    received_bits++;
    received_number |= (1 << (received_bits - 1));  // установим текущий бит единичным
    kill(transmitter_pid, SIGUSR2);
}

void sigint_handler(int sig)
{
    printf("Receiver: received number %d\n", received_number);
    exit(0);
}

int main()
{
    printf("Receiver PID: %d\n", getpid());

    printf("Enter transmitter PID: ");
    scanf("%d", &transmitter_pid);

    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);
    signal(SIGINT, sigint_handler);

    while (1) {
        pause();
    }
}

