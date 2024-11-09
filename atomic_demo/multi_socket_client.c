#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8888

void *send_request(void *arg) {
    int sock_fd;
    struct sockaddr_in server_addr;
    char *msg = (char *)arg;

    // 创建 socket
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 设置 server_addr 结构体
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    server_addr.sin_port = htons(SERVER_PORT);

    // 连接服务器
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // 发送请求
    if (send(sock_fd, msg, strlen(msg), 0) == -1) {
        perror("send");
        exit(EXIT_FAILURE);
    }

    close(sock_fd);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[3];
    char *msgs[3] = {"Hello", "World", "!"};
    int i;

    // 创建线程发送请求
    for (i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, send_request, (void *)msgs[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // 等待线程结束
    for (i = 0; i < 3; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
