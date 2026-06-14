#include "../include/client.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
void startClient() {

    WSADATA wsData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsData);

    if (result != 0) {
        std::cout << "WSAStartup failed\n";
        return;
    }

    std::cout << "[CLIENT] Winsock initialized\n";

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(clientSocket == INVALID_SOCKET) {
        std::cout << "Socket creation failed\n";
        WSACleanup();
        return;
    }

    std::cout << "Socket created successfully\n";

    sockaddr_in serverHint{};

serverHint.sin_family = AF_INET;
serverHint.sin_port = htons(54000);

inet_pton(AF_INET, "127.0.0.1", &serverHint.sin_addr);

int connectResult = connect(
    clientSocket,
    (sockaddr*)&serverHint,
    sizeof(serverHint)
);

if (connectResult == SOCKET_ERROR) {
    std::cout << "Connection failed\n";
    closesocket(clientSocket);
    WSACleanup();
    return;
}

std::cout << "Connected to server successfully!\n";
}