// Holland Ho
// CPSC 351 
// Assignment 4 Sockets
// 11/20/19 
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// make sure to compile with -lws2_32 for the linker
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
// socket functions to use
// sendto(), closesocket(), recv()
/*
TODO 
thread is not working properly, sending a message from our end will end it


*/
// global flag for end of message
bool msgEnd = false; 

// Function to make socket
SOCKET MakeSocket(unsigned short Port) { 
    SOCKET sock = INVALID_SOCKET;
    SOCKADDR_IN Addr = {0};

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock == INVALID_SOCKET) {
        cout << "socket function failed with error = %d\n" << WSAGetLastError();
        WSACleanup();

        return 1;
    }

    Addr.sin_family = AF_INET;
    Addr.sin_port = htons(Port);
    Addr.sin_addr.s_addr = inet_addr("127.0.0.1");


    if (bind(sock, (SOCKADDR *)&Addr, sizeof(Addr)) == SOCKET_ERROR) {
        
        cout << "bind failed with error %u\n " << WSAGetLastError();
        closesocket(sock);
        WSACleanup();

        return 1;
    }

    // if socket is successful
    return sock;
}

// sending data func
BOOL SendData(SOCKET sock, unsigned short Port) {
    SOCKADDR_IN SendAddr = {0};
    char buf[1024];

    SendAddr.sin_family = AF_INET;
    SendAddr.sin_port = htons(Port);
    SendAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// test
	while (1) {
		cout << ">";
		fgets(buf, 1024, stdin);

		sendto(sock, buf, strlen(buf), 0, (SOCKADDR*)&SendAddr, sizeof(SendAddr));
	}
    // if message is successful return true to continue
    return true;
}

// thread func for receiving messages
DWORD WINAPI RecvThread(LPVOID lpParam) {
    SOCKET sock = (SOCKET)lpParam;
    SOCKADDR_IN RecvAddr = {0};
    int iRet;
    int iRecvSize; 

    // buffer allowing 1024 chars
    char buf[1024];

    // print out the message from sender
    while (!msgEnd) {
        iRecvSize = sizeof(RecvAddr);
        iRet = recvfrom(sock, buf, 1024, 0, (SOCKADDR *)&RecvAddr, &iRecvSize);
    
        if (iRet == SOCKET_ERROR) {
            cout << "recvfrom failed with error %d\n" << WSAGetLastError();
            continue; //testing
        }

        // mark the end of message string
        buf[iRet] = '\0';
        cout << inet_ntoa(RecvAddr.sin_addr) << " " << htons(RecvAddr.sin_port) << ": " << buf << "\n>";
    }

    cout << "Ending Recv Thread... \n";

    return 0;
}


int main() {
    
    WSADATA wsaData = {0};
    SOCKET sock;
    sockaddr_in RecvAddr;

    // Port to send to Oates App
    unsigned short DesPort = 3514;
    // Receiving port
    unsigned short RecPort = 3515;

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // creating our socket 
    sock = MakeSocket(RecPort);

    // if socket creation is successful
    if (sock) {
        cout << "Socket was successfully created \n" ;
        // make thread for receiving 
        HANDLE hThread = CreateThread(NULL, 0, RecvThread, (PVOID)sock, 0, NULL);

        while (1){
            // if messages stop then break out
            if (!SendData(sock, DesPort)) {
                break;
            }

            msgEnd = true;
            closesocket(sock);

            if (WaitForSingleObject(hThread, 3000) == WAIT_TIMEOUT) {
                cout << "Thread has ended \n";
                TerminateThread(hThread, 0);
            }
        }
    }

    WSACleanup();

    return 0;

}





