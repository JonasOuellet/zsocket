// zsocket.cpp : Defines the exported functions for the DLL.
//

#include "framework.h"
#include "zsocket.h"

#include <cstring>
#include <string>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


std::string address = "127.0.0.1";
std::string port = "6001";


ZSOCKET_API float GetSocketAddr(char* text, double number, char* memBlockIn, int memBlockSizeIn,
    char* memBlockOut, int memBlockSizeOut, char** zData)
{

    std::string _addr = address + ':' + port;

    if (memBlockSizeOut > _addr.length())
    {
        strcpy_s(memBlockOut, memBlockSizeOut, _addr.c_str());
        return 0.0f;
    }
    else
    {
        // memory block too small
        return static_cast<float>(_addr.length());
    }
}


ZSOCKET_API float SetSocketAddr(char* text, double number, char* memBlockIn, int memBlockSizeIn,
    char* memBlockOut, int memBlockSizeOut, char** zData)
{
    std::string _addr = text;
    size_t pos = _addr.find(':');
    if (pos != std::string::npos) {
        address = _addr.substr(0, pos);
        port = _addr.substr(pos + 1);

        return 0.0f;
    }

    return -1.0f;
}


// https://docs.microsoft.com/en-us/windows/win32/winsock/complete-client-code
ZSOCKET_API float SocketSend(char* text, double number, char* memBlockIn, int memBlockSizeIn,
    char* memBlockOut, int memBlockSizeOut, char** zData)
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL,
                    * ptr = NULL,
                    hints;

    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        if (memBlockSizeOut) {
            std::string msg = "WSAStartup failed with error: " + iResult;
            strcpy_s(memBlockOut, memBlockSizeOut, msg.c_str());
        }
        return static_cast<float>(iResult);
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(address.c_str(), port.c_str(), &hints, &result);
    if (iResult != 0) {
        if (memBlockSizeOut) {
            std::string msg = "getaddrinfo failed with error: " + iResult;
            strcpy_s(memBlockOut, memBlockSizeOut, msg.c_str());
        }
        WSACleanup();
        return static_cast<float>(iResult);
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            int error = WSAGetLastError();
            if (memBlockSizeOut) {
                std::string msg = "socket failed with error: " + error;
                strcpy_s(memBlockOut, memBlockSizeOut, msg.c_str());
            }
            WSACleanup();
            return static_cast<float>(error);
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        if (memBlockSizeOut) {
            strcpy_s(memBlockOut, memBlockSizeOut, "Unable to connect to server!");
        }
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    iResult = send(ConnectSocket, text, (int)strlen(text), 0);
    if (iResult == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (memBlockSizeOut) {
            std::string msg = "send failed with error: " + error;
            strcpy_s(memBlockOut, memBlockSizeOut, msg.c_str());
        }
        closesocket(ConnectSocket);
        WSACleanup();
        return static_cast<float>(error);
    }

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (memBlockSizeOut) {
            std::string msg = "shutdown failed with error: " + error;
            strcpy_s(memBlockOut, memBlockSizeOut, msg.c_str());
        }
        closesocket(ConnectSocket);
        WSACleanup();
        return static_cast<float>(error);
    }

    // Receive until the peer closes the connection
    do {

        iResult = recv(ConnectSocket, memBlockOut, memBlockSizeOut, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0.0f;
}