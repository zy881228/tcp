#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main()
{
    //load winsock
    WSADATA wsaData;
    char buff[1024];
    memset(buff, 0, sizeof(buff));

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed to load Winsock");
        return 0;
    }

    SOCKADDR_IN addrSrv;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(8888);
    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    //create socket
    SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
    if(SOCKET_ERROR == sockClient){
        printf("Socket() error:%d", WSAGetLastError());
        return 1;
    }

    //connect
    if(connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET){
        printf("Connect failed:%d", WSAGetLastError());
        return 2;
    }else
    {
                //send request
                char request[100] = {0};
                cout<<"Please type in request:"<<endl;
                cin>>request;
                //printf("request:%s",request);
                string command(request);
                //printf("command:%s\n",command);
                if(command == "down") {
                    //printf("???");
                    send(sockClient, request, sizeof(request), 0);
                    FILE *fp = NULL;
                    if ((fp = fopen("C:\\Users\\123\\CLionProjects\\Client\\recv.jpg", "wb+")) == NULL) {
                        wprintf(L"fopen function failed with error: %ld\n", WSAGetLastError());
                        return 3;
                    }
                size_t size = 0;
                char recvBuf[1024];
                do {
                    size = recv(sockClient, recvBuf, sizeof(recvBuf), MSG_WAITALL);
                    if (size == SOCKET_ERROR) {
                        break;
                    }
                    fwrite(recvBuf, size, 1, fp);
                } while ( size = sizeof(recvBuf));
                fclose(fp);
                cout << "transmission end!!";
        }
        if(command == "clos") {
            send(sockClient, request, sizeof(request), 0);
            closesocket(sockClient);
        }
    }

    //close
    closesocket(sockClient);
    WSACleanup();
}