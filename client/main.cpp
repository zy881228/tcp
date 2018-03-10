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
        while(1) {
            //send request
            char request[100] = {0};
            char recvBuf[100] = {0};
            cout << "Please type in request:" << endl;
            cin >> request;
            //printf("request:%s",request);
            string command(request);
            //printf("command:%s\n",command);
            if (command == "down") {
                //printf("???");
                if(send(sockClient, request, sizeof(request), 0) != SOCKET_ERROR);
                {
                    recv(sockClient, recvBuf, sizeof(recvBuf), 0);
                    cout<<"%s"<<recvBuf<<endl;
                }

                FILE *fp = NULL;
                if ((fp = fopen("C:\\Users\\123\\CLionProjects\\Client\\recv.jpg", "wb+")) == NULL) {
                    wprintf(L"fopen function failed with error: %ld\n", WSAGetLastError());
                    return 3;
                }
                size_t size = 0;
                char recvFile[1024];
                do {
                    size = recv(sockClient, recvFile, sizeof(recvFile), MSG_WAITALL);
                    if (size == SOCKET_ERROR) {

                        break;
                    }
                    if (size == 0) {
                        break;
                    }
                    fwrite(recvFile, size, 1, fp);
                } while (size = sizeof(recvFile));
                fclose(fp);
                cout << "transmission end!!\n";
                if (size == 0) {
                    break;
                }
            }
            if (command == "clos") {
                send(sockClient, request, sizeof(request), 0);
                cout << "TCP connection closed!" << endl;
                break;
            }
            if (command == "recn") {
                send(sockClient, request, sizeof(request), 0);
                closesocket(sockClient);

            }
            if(send(sockClient, request, sizeof(request), 0)!= SOCKET_ERROR){
                recv(sockClient, recvBuf, sizeof(recvBuf), 0);
                cout<<"%s"<<recvBuf<<endl;
            } else{
                printf("send failed:%d\n", WSAGetLastError());
                shutdown(sockClient,2);
                sockClient = socket(AF_INET, SOCK_STREAM, 0);
                if(connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) != INVALID_SOCKET){
                    cout<<"TCP Reconnect success!!"<<endl;
                }
            }
        }
    }

    //close
    closesocket(sockClient);
    WSACleanup();
}