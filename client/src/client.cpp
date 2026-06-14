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