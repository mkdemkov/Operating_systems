#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_MSG_SIZE 1024

int main(int argc, char *argv[]) {

    // проверяем, что IP-адрес и порт заданы в аргументах командной строки
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <ip-address> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // создаем сокет
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // настраиваем параметры сокета
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    // устанавливаем соединение с сервером
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_MSG_SIZE];

    // читаем сообщения из стандартного ввода и отправляем их серверу
    while (fgets(buffer, MAX_MSG_SIZE, stdin) != NULL) {
        send(sockfd, buffer, strlen(buffer), 0);
        printf("Sent message to server: %s", buffer);

        // если отправлено сообщение "The End", завершаем работу
        if (strcmp(buffer, "The End\n") == 0) {
            close(sockfd);
            return 0;
        }

        // читаем ответ от сервера
        ssize_t num_bytes = recv(sockfd, buffer, MAX_MSG_SIZE, 0);
        if (num_bytes == -1) {
            perror("recv");
        } else {
            buffer[num_bytes] = '\0';
            printf("Received message from server: %s", buffer);
        }
    }

    close(sockfd);

    return 0;
}
