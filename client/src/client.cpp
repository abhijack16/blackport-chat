#include "../include/client.h"
#include <ws2tcpip.h>
#include <iostream>
#include <string>
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

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == INVALID_SOCKET) {
        std::cout << "Socket creation failed\n";
        WSACleanup();
        return;
    }

    std::cout << "Socket created successfully\n";

    sockaddr_in serverHint{};

    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(54000);

    serverHint.sin_addr.s_addr = inet_addr("127.0.0.1");

    int connectResult = connect(
        clientSocket,
        (sockaddr*)&serverHint,
        sizeof(serverHint)
    );

    if (connectResult == SOCKET_ERROR) {
        std::cout << "Connection failed: " << WSAGetLastError() << '\n';
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    std::cout << "Connected to server successfully!\n";
    
    while(true) {
        std::string message;

        std::cout << "Enter message: ";
        std::getline(std::cin, message);

        if(message == "exit")
        break;

        send(
            clientSocket,
            message.c_str(),
            message.size() + 1,
            0
        );

        std::cout << "Message sent!\n";

        const int BUFFER_SIZE = 4096;
        char buffer[BUFFER_SIZE];

        ZeroMemory(buffer, BUFFER_SIZE);

        int bytesReceived = recv(
            clientSocket,
            buffer,
            BUFFER_SIZE,
            0
        );

        std::cout << "Client received " << bytesReceived << " bytes\n";

        if(bytesReceived > 0) {
            std::cout << "Server replied: " << buffer << '\n';
        }
        else if(bytesReceived == 0) {
            std::cout << "Server closed connection\n";
        }
        else {
            std::cout << "recv failed: " << WSAGetLastError() << '\n';
        }
    }

    closesocket(clientSocket);
    WSACleanup();
}
