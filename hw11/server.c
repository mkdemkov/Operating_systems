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

    // привязываем сокет к адресу и порту
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // начинаем прослушивание входящих соединений
    if (listen(sockfd, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on %s:%d\n", argv[1], atoi(argv[2]));

    // ожидаем входящих соединений
    int client_sockfd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    while (1) {
        client_sockfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_addr_len);
        if (client_sockfd == -1) {
            perror("accept");
            continue;
        }

        printf("New client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        char buffer[MAX_MSG_SIZE];
        ssize_t num_bytes;

        // читаем данные от клиента №1 и отправляем их клиенту №2
        while ((num_bytes = recv(client_sockfd, buffer, MAX_MSG_SIZE, 0)) > 0) {
            buffer[num_bytes] = '\0';
            printf("Received message from client %s:%d: %s", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

            // перенаправляем сообщение клиенту №2
            send(client_sockfd, buffer, num_bytes, 0);
            printf("Sent message to client %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            
            // если получено сообщение "The End", завершаем работу
            if (strcmp(buffer, "The End\n") == 0) {
                close(client_sockfd);
                close(sockfd);
                return 0;
            }
        }

        if (num_bytes == -1) {
            perror("recv");
        }

        close(client_sockfd);
        printf("Client disconnected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }

    return 0;
}
