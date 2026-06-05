#include <iostream>
#include <winsock2.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")
SOCKET clientSocket;

 void receiveMessages(){
        while(true){
        char buffer[1024] = {0};
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer)-1, 0);

if(bytesReceived <= 0)
{
    std::cout << "client disconnected\n";
    break;
}

buffer[bytesReceived] = '\0';

std::cout << "client says: " << buffer << "\n";
        }
    }


int main() {

    // Start Winsock
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cout << "WSAStartup failed\n";
        return 1;
    }

    // Create socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(serverSocket == INVALID_SOCKET) {
        std::cout << "Socket creation failed\n";
        return 1;
    }

    std::cout << "Socket created successfully\n";

    // Define server address
    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if(bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cout << "Bind failed\n";
        return 1;
    }

    std::cout << "Bind successful\n";

    // Listen
    if(listen(serverSocket, 5) == SOCKET_ERROR) {
        std::cout << "Listen failed\n";
        return 1;
    }

    std::cout << "Server is listening on port 8080...\n";

    clientSocket = accept(serverSocket, NULL, NULL);

if(clientSocket == INVALID_SOCKET) {
    std::cout << "Client connection failed\n";
    return 1;
}

std::cout << "Client connected successfully!\n";

std::thread receiver(receiveMessages);
while(true) {

    char reply[1024];

std::cout << "You: ";
std::cin.getline(reply, 1024);
 if(strcmp(reply, "/exit") == 0)
        {
            closesocket(clientSocket);
            break;
        }

    send(clientSocket, reply, strlen(reply), 0);
}
    receiver.join();

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}