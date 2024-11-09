#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifdef _WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
#elif defined(__linux__)
#  include <sys/socket.h>
#endif

int socket_client(const char *ip, int port, const char *sendbuf) {
    // Send and receive data.
    int bytesSent;
    int bytesRecv = SOCKET_ERROR;
    char recvbuf[1024] = "";

#ifdef _WIN32
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    // Create a SOCKET for connecting to server
    SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(ConnectSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // The sockaddr_in structure specifies the address family,
    // IP address, and port of the server to be connected to.
    struct sockaddr_in clientService;
    memset(&clientService, 0, sizeof(clientService));
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(ip);
    clientService.sin_port = htons(port);

    // Connect to server.
    if(connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        printf("Failed to connect.\n");
        WSACleanup();
        return 1;
    }

    // Send some test string to server
    bytesSent = send(ConnectSocket, sendbuf, strlen(sendbuf), 0);
    printf("Bytes Sent: %ld\n", bytesSent);

    // shutdown the connection since no more data will be sent
/*    iResult = shutdown(ConnectSocket, SD_SEND);
    if(iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
*/

    // Receive until the peer closes the connection
    do {
        bytesRecv = recv(ConnectSocket, recvbuf, 200, 0);
        if (bytesRecv > 0) {
            printf("Bytes received: %d, receive is %s \n", bytesRecv, recvbuf);
		    break;
        }
        else if(bytesRecv == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());
    } while(bytesRecv > 0);

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
#elif defined(__linux__)
#endif
}

int main(void) {
  int sockfd;
  char buf[1024];
  int n;
  char *ip = "127.0.0.1";
  int port = 8888;
  char sendbuf[200] = "This is a test string from client";

  sockfd = socket_client(ip, port, sendbuf);
  system("pause");
}
