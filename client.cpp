#include <iostream>//to use input output features
#include <cstring>//to use string fns.
#include <winsock2.h>//socket related fns like connect,recv etc
#include <thread>//to use thread

#pragma comment(lib, "ws2_32.lib")//helps find the actual code for the winsock fns.

SOCKET clientSocket;//SOCKET IS THE DATA TYPE

    void receiveMessages(){

        while(true){
        char buffer[1024] = {0};
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer)-1, 0);

if(bytesReceived <= 0)
{
    std::cout << "Server disconnected\n";
    break;
}

buffer[bytesReceived] = '\0';

std::cout << "Server says: " << buffer << "\n";
        }
    }

int main() {

    // Start Winsock  // wsaData is the container to store info
    WSADATA wsaData;//WSADATA=datatype, store info about winsock, like version supported,system status etc

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {//c
        std::cout << "WSAStartup failed\n";
        return 1;
    }

    // Create client socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(clientSocket == INVALID_SOCKET) {
        std::cout << "Socket creation failed\n";
        return 1;
    }

    std::cout << "Client socket created\n";

    // Define server address
    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;//This address will use IPv4.
    serverAddress.sin_port = htons(8080);//h=host,to,n=network,s=short,Convert 8080 from host format to network format.
    
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");//Convert text IP into a format Windows networking understands.

    // Connect to server
//      Use clientSocket
//      Connect to the destination
//      stored inside serverAddress
//      Read the entire structure
//      using its size
    if(connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cout << "Connection failed\n";
        return 1;
    }

    std::cout << "Connected to server successfully!\n";
    // create receiver thread
    std::thread receiver(receiveMessages);
    std::cout << "NEW CLIENT BUILD RUNNING\n";
    while(true) {

    char message[1024];

    std::cout << "You: ";
    std::cin.getline(message, 1024);
    
    if(strcmp(message,"/exit")==0)
    {
        std::cout << "EXIT DETECTED\n";
        closesocket(clientSocket);
        break;
    }

    send(clientSocket, message, strlen(message), 0);
    std::cout << "Message sent\n";

    }
    receiver.join();
    WSACleanup();

    return 0;
}