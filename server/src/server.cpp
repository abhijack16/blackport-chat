#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

void receiveMessages(SOCKET clientSocket){

    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    while(true) {
        ZeroMemory(buffer, BUFFER_SIZE);

        int bytesReceived = recv(
            clientSocket,
            buffer,
            BUFFER_SIZE,
            0
        );

        std::cout << "bytesReceived = " << bytesReceived << '\n';

        if(bytesReceived > 0) {
            std::cout << "Received: " << buffer << '\n';

            if(std::string(buffer) == "exit") {
                std::cout << "Client requested to disconnect.\n";
                break;
            }

            int bytesSent = send(
                clientSocket,
                buffer,
                bytesReceived,
                0
            );

            std::cout << "Server sent " << bytesSent << " bytes\n";
        }
        else if(bytesReceived == 0) {
            std::cout << "Client disconnected.\n";
            break;
        }
        else {
            std::cout << "recv() failed: " << WSAGetLastError() << '\n';
            break;
        }
    }
    
}

SOCKET acceptClient(SOCKET serverSocket) {

    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

    if(clientSocket == INVALID_SOCKET) {
        std::cout << "Accept failed: " << WSAGetLastError() << '\n';
        return INVALID_SOCKET;
    }

    std::cout << "Client connected successfully!\n";

    return clientSocket;
}

void startServer() {

    WSADATA wsData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsData);
    
    if(result != 0) {
        std::cout << "WSAStartup failed\n";
        return;
    }

    std::cout << "[BLACKPORT SERVER] Winsock initialized successfully\n";
    
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(serverSocket == INVALID_SOCKET) {
        std::cout << "Socket creation failed\n";
        WSACleanup();
        return;
    }

    std::cout << "Socket created successfully\n";

    sockaddr_in serverHint{};

    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(54000);
    serverHint.sin_addr.s_addr = INADDR_ANY;

    if(bind(serverSocket, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) {
        std::cout << "Bind failed: " << WSAGetLastError() << '\n';
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Bind successful\n";

    if(listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cout << "Listen failed: " << WSAGetLastError() << '\n';
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Listening for connections...\n";

    SOCKET clientSocket = acceptClient(serverSocket);

    if(clientSocket == INVALID_SOCKET) {
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    receiveMessages(clientSocket);

    std::cout << "Closing client connection...\n";
    
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return;
}