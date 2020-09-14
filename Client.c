#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <winsock.h>

#define MAXLINE 1024

SOCKET        	sd;
struct sockaddr_in serv;
char  		str[1024];
char  		str_r[1024];
WSADATA 		wsadata;
int timeout = 50,n;

void *recvMess(void *argu) {    // 每隔一秒鐘印出一次 George 的函數
    while (1) {
        setsockopt(sd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));
		n=recv(sd, str_r, MAXLINE, 0); //接收來自其他client的訊息

		if(strcmp(str_r,"人數已滿")==0)
		{
			closesocket(sd);
			printf("人數已滿>.< \n");
			return 0;
		}
		if(n!=-1)
            printf("recv: %s (%d bytes)\n",str_r,strlen(str_r)+1);
    }
}

int main(int argc, char** argv) {
   int n,i,j,total=0;
    pthread_t thread1;

   WSAStartup(0x101,(LPWSADATA) &wsadata); // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用


   sd=socket(AF_INET, SOCK_STREAM, 0); //開啟一個 TCP socket.

   serv.sin_family       = AF_INET;
   serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
   serv.sin_port         = htons(5678);

   connect(sd, (LPSOCKADDR) &serv, sizeof(serv)); // 連接至 echo server


    pthread_create(&thread1, NULL, &recvMess, NULL);

	while(1){

		gets(str);
		send(sd, str, strlen(str)+1, 0); //傳送訊息
	}

   closesocket(sd); //關閉TCP socket

   WSACleanup();  // 結束 WinSock DLL 的使用
   system("pause");

   return 0;
}
