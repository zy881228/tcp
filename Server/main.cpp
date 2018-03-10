#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main() {
    WSADATA wsaData;
    int port = 8888;


    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to load Winsock");
        return 0;
    }

    //create socket
    SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN addrSrv;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(port); //bind port
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    int retVal = bind(sockSrv, (LPSOCKADDR) &addrSrv, sizeof(SOCKADDR_IN));
    if (retVal == SOCKET_ERROR) {
        printf("Failed bind:%d\n", WSAGetLastError());
        return 0;
    }

    if (listen(sockSrv, 10) == SOCKET_ERROR) {
        printf("Listen failed:%d", WSAGetLastError());
        return 0;
    }
    cout<<"Server is ready!\n";
    SOCKADDR_IN addrClient;
    int len = sizeof(SOCKADDR);

    while (1) {
        //listenning for client
        SOCKET sockConn = accept(sockSrv, (SOCKADDR *) &addrClient, &len);
        if(sockConn != SOCKET_ERROR)
        {
            cout<<"connect success!\n";
            //send(sockConn, "connect success!Please send request!", sizeof("connect success!Please send request!"), 0);
            /*int currentPos=0;//breakpoint value
            if(recv(sockConn,(char*)&currentPos,sizeof(currentPos),0)==SOCKET_ERROR)
            {
                cout<<"The Clinet Socket is Closed/n";
                break;
            }else
            {
                cout<<"The Currentpos is The"<<currentPos<<"/n";
                getPos(currentPos,client);
            }*/
        } else{
            cout<<"accept fail!/n";
        }
        while(1) {
            //receive message
            char recvBuf[100] = {0};
            memset(recvBuf, 0, sizeof(recvBuf));
            size_t size = 0;
            size = recv(sockConn, recvBuf, sizeof(recvBuf), 0);
            if (size != SOCKET_ERROR) {
                char reply[] = "Received!!";
                send(sockConn, reply, sizeof(reply), 0);
            }
            printf("%s\n", recvBuf);
            string command(recvBuf);
            //download pic
            //printf("command:%s\n",command);
            if (command == "down") {
                char buffer[] = "start transmission......\n";
                //send(sockConn, buffer, sizeof(buffer), 0);
                cout << "%s\n" << buffer;
                FILE *fp = NULL;
                if ((fp = fopen("C:\\Users\\123\\CLionProjects\\Server\\test.jpg", "rb")) == NULL) {
                    printf("fopen failed with error: %ld\n", GetLastError());
                    return 0;
                }
                size_t size = 0;
                char sendBuf[1024];
                while (!feof(fp)) {
                    size = fread(sendBuf, sizeof(char), sizeof(sendBuf), fp);
                    if (send(sockConn, sendBuf, size, 0) == SOCKET_ERROR) {
                        printf("send failed with error: %ld\n", GetLastError());
                        return 0;
                    }
                }
                cout << "picture send success!!\n";
                closesocket(sockConn);
                break;
            }
            //close client
            if (command == "clos") {
                closesocket(sockConn);
                cout<<"Client connection closed!!\n";
                break;
            }
            if (command == "recn") {
                closesocket(sockConn);
                cout<<"Client connection closed!!\n";
                break;
            }
        }
    }

    closesocket(sockSrv);
    WSACleanup();
    system("pause");
}