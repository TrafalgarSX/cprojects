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

#define array_size(x) (sizeof(x) / sizeof(x[0]))
#define IF_ERROR(x) if(x) {result = -1; goto END;}

int socket_server(int iFamily, int iType, int iProtocol, int iPort, const char *ip){
  int result = 0;
  char recvbuf[1024] = {0};

#ifdef _WIN32
  WSADATA wsaData = {0};
  int iResult = 0;
  SOCKET sock = INVALID_SOCKET;
  SOCKET clientSock = INVALID_SOCKET;
  struct sockaddr_in sockAddr;
  struct sockaddr_in clientAddr;
  int clientAddrSize;

  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  IF_ERROR(iResult != 0);

  sock = socket(iFamily, iType, iProtocol);
  IF_ERROR(sock == INVALID_SOCKET);

  sockAddr.sin_family = iFamily;
  sockAddr.sin_addr.s_addr = inet_addr(ip);
  sockAddr.sin_port = htons(iPort);
  iResult = bind(sock, (struct sockaddr*)&sockAddr, sizeof(sockAddr));
  IF_ERROR(iResult == SOCKET_ERROR);

  if (iType == SOCK_STREAM) {
    iResult = listen(sock, SOMAXCONN);
    IF_ERROR(iResult == SOCKET_ERROR);
  }

  // Accept a client socket
  clientAddrSize = sizeof(clientAddr);
  printf("waiting for accept client connect start ....\n");
  clientSock = accept(sock, (struct sockaddr*)&clientAddr, &clientAddrSize);
  printf("waiting for accept client connect end....\n");
  IF_ERROR(clientSock == INVALID_SOCKET);

  // Receive until the peer shuts down the connection
  do {
    printf("waiting for receive start ....\n");
    iResult = recv(clientSock, recvbuf,  array_size(recvbuf), 0);
    printf("waiting for receive end....\n");
    if (iResult > 0) {
      printf("Bytes received: %d, recvbuf is :%s \n", iResult, recvbuf);
	  // Echo the buffer back to the sender
	  iResult = send(clientSock, "This something from server", iResult, 0);
	  IF_ERROR(iResult == SOCKET_ERROR);
    } else if (iResult == 0) {
      wprintf(L"Connection closed\n");
    } else {
      wprintf(L"recv function failed with error: %ld\n", WSAGetLastError());
      closesocket(clientSock);
      WSACleanup();
      return 1;
    }
  } while (iResult > 0);

  // shutdown the connection since we're done
  iResult = shutdown(clientSock, SD_SEND);
  IF_ERROR(iResult == SOCKET_ERROR);

#elif defined(__linux__)
#endif

END:
  // cleanup
  closesocket(clientSock);
  closesocket(sock);
  WSACleanup();
  return result;

}

int main(void)
{
  int result = -1;
  int iFamily = AF_INET;
  int iType = SOCK_STREAM;
  int iProtocol = IPPROTO_TCP;
  int iPort = 8888;
  char *pszAddress = "127.0.0.1";
  
  result = socket_server(iFamily, iType, iProtocol, iPort, pszAddress);
  if(result != 0) {
    printf("client_server() failed\n");
  }

  system("pause");
  return 0;
}
