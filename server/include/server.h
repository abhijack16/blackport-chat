#pragma once

#include <winsock2.h>

void startServer();
void receiveMessages(SOCKET clientSocket);
SOCKET acceptClient(SOCKET serverSocket);