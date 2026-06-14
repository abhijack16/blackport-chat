#include "../include/client.h"
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

void startClient() {

    WSADATA wsData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsData);

    if (result != 0) {
        std::cout << "WSAStartup failed\n";
        return;
    }

    std::cout << "[CLIENT] Winsock initialized\n";
}
SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

if(clientSocket == INVALID_SOCKET) {
    std::cout << "Socket creation failed\n";
    WSACleanup();
    return;
}

std::cout << "Socket created successfully\n";