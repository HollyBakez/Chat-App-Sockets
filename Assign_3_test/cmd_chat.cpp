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

    if (bind(sock, (SOCKADDR *)&Addr, sizeof(Addr)) == SOCKET_ERROR) {
        
        cout << "bind failed with error %u\n " << WSAGetLastError();
        closesocket(sock);
        WSACleanup();

        return 1;
    }

    // if socket is successful
    return sock;
}


int main() {
    
    int iResult;
    WSADATA wsaData;

    SOCKET SendSocket = INVALID_SOCKET;
    sockaddr_in RecvAddr;

    // Port to send to Oates App
    unsigned short DesPort = 3514;
    // Receiving port
    unsigned short RecPort = 3515;

    //buffer for user and rec msg
    char SendBuf[1024];
    int BufLen = 1024;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        cout <<" socket failed with error" << WSAGetLastError();
        WSACleanup();
        return 1;
    }

    // set up recv structure with the ip address 127.0.0.1
    // and our port number
    RecvAddr.sin_family = AF_INET;
    RecvAddr.sin_port = htons(Port);
    RecvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    iResult = sendto(SendSocket, SendBuf, BufLen, 0, (SOCKADDR *) & RecvAddr, sizeof(RecvAddr));

    // error handling for socket
    if (iResult == SOCKET_ERROR) {
        cout << "sendto failed with error " << WSAGetLastError();
        closesocket(SendSocket);
        WSACleanup();
        return 1;

    }

    cout << "Finished sending. Closing socket. \n";
    iResult = closesocket(SendSocket);
    if (iResult == SOCKET_ERROR) {
        cout << "clossocket failed with error: " << WSAGetLastError();
        WSACleanup();
        return 1;
    }

    cout << "Exiting. \n";
    WSACleanup();
    return 0;


}










/*


int main(){
    //Declare and initialize variables
    WSADATA wsaData = {0};
    int iResult = 0 ;
    // intialize two ports for send and recv
    // Source Port set to port 3515
    WORD wSrcPort = 3515;
    WORD wDstPort = 3514; 

    //initialize invalid socket
    SOCKET sock = INVALID_SOCKET;
    int iFamily = AF_INET;
    int iType = SOCK_DGRAM;
    int iProtocol = IPPROTO_UDP;

    // initiates use of Winsock
    // needed or else socket fails
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        wprintf(L"WSAStartup failed: %d\n", iResult);
        return 1;
    }

    // create thread to send socket
    sock = socket(iFamily, iType, iProtocol);

    if (sock == INVALID_SOCKET) {
        cout << "socket function has failed \n" ; 
    }
    else {
        cout << "function succeeded \n";
    }

    if (sock){
        HANDLE hThread = CreateThread(NULL, 0, RecvThread, (PVOID)sock, 0, NULL);
    }

    return 0;
}


*/