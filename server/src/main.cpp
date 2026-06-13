#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {

    WSADATA wsData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsData);
    
    if(result != 0) {
        std::cout << "WSAStartup failed\n";
        return 1;
    }

    std::cout << "[BLACKPORT SERVER] Winsock initialized successfully\n";
    
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(serverSocket == INVALID_SOCKET) {
        std::cout << "Socket creation failed\n";
        WSACleanup();
        return 1;
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
        return 1;
    }

    std::cout << "Bind successful\n";

    if(listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cout << "Listen failed: " << WSAGetLastError() << '\n';
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Listening for connections...\n";

    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

    if(clientSocket == INVALID_SOCKET) {
        std::cout << "Accept failed: " << WSAGetLastError() << '\n';

        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connected successfully!\n";
    
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}