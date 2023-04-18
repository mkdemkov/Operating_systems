#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

pid_t receiver_pid;
int current_bit = 0;

void send_bit(int bit)
{
    if (bit == 0) {
        kill(receiver_pid, SIGUSR1);
    } else {
        kill(receiver_pid, SIGUSR2);
    }
}

void sigusr1_handler(int sig)
{
    if (current_bit != 0) {
        printf("Transmitter error: no confirmation received for bit %d\n", current_bit - 1);
        exit(1);
    }
    current_bit++;
    send_bit((rand() % 2));  // отправляем следующий бит рандомный
}

void sigusr2_handler(int sig)
{
    if (current_bit != 0) {
        printf("Transmitter error: no confirmation received for bit %d\n", current_bit - 1);
        exit(1);
    }
    current_bit++;
    send_bit((rand() % 2));  // отправляем следующий бит рандомный
}

void sigint_handler(int sig)
{
    printf("Transmitter: transmission finished\n");
    exit(0);
}

int main()
{
    printf("Transmitter PID: %d\n", getpid());

    printf("Enter receiver PID: ");
    scanf("%d", &receiver_pid);

    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);
    signal(SIGINT, sigint_handler);

    printf("Enter integer to transmit: ");
    int n;
    scanf("%d", &n);

    for (int i = 0; i < 32; i++) {
        send_bit((n >> i) & 1);
        pause();  // ждем подтверждения от получателя
    }

    printf("Transmitter error: no confirmation received for bit 31\n");
    exit(1);
}

