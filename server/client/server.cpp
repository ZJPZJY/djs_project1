#include<stdio.h>
#include<winsock.h>
#include<string.h>
#include <time.h>

int str_2ip_port(char* str, char** ip, int* port);
void get_time(struct timer *t);
//将一个timer结构体转化为 “second.txt”字符串
char* get_time_str(char* s, struct timer t);

struct timer {
	time_t second;
	WORD millisecond;
};


/*
* 实现流程主要是绑定好套接字，然后发送消息，等待接收消息
*/
int main(int argc,char **argv) {
	struct timer t_old, t_new;
	get_time(&t_old);
	//FILE *f =  fopen(itoa(t_old.second),"rb+")
	char* ip;int port;
	int cnt = 30;
	if (argc >1 && !str_2ip_port(argv[1], &ip, &port))
	{
		//windows启动 socket服务
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			printf("WSAStartup() error!");
		//这里使用的是TCP报文通信，SOCK_DGRAM为UDP
		SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
		SOCKET new_s;
		struct sockaddr_in saddr, new_addr;

		memset(&saddr, 0, sizeof(saddr));
		saddr.sin_family = AF_INET;
		saddr.sin_addr.s_addr = inet_addr(ip);
		saddr.sin_port = htons(port);

		int b = bind(s, (struct sockaddr*)&saddr, sizeof(saddr));
			if (b == SOCKET_ERROR) {
				printf("SOCKET_ERROR\n");
				return 0;
			}
			else
				printf("bind %s:%d success\n", ip, port);

		if (listen(s, 5) == SOCKET_ERROR)
			printf("error");
		int szClntAddr = sizeof(new_addr);
		new_s = accept(s, (SOCKADDR*)&new_addr, &szClntAddr);

		if (new_s == -1) {
			printf("accept feiled\n");
			return 0;
		}

		char message[] = "Hello World!";
		char* recvmess = (char*)malloc(sizeof(message));
		//message[0] = 0x36;
		while (cnt--) {
			get_time(&t_old);
			send(new_s, message, sizeof(message), 0);
			printf("i send:%s at %d:%d and", message, t_old.second, t_old.millisecond);
			recv(new_s, recvmess, 20, 0);
			//Sleep(1);
			get_time(&t_new);
			printf("i recv:%s at %d:%d\n", recvmess, t_new.second, t_new.millisecond);
		}
	}

	return 0;
}

int str_2ip_port(char* str, char** ip, int* port) {
	if (str == NULL) return -1;
	*ip = str;
	char* i = strchr(*ip, ':');
	if (i == NULL)
		return -1;
	i[0] = '\0';
	i = i + 1;
	*port = atoi(i);
	printf("ip :%s\nport :%d\n", *ip,  *port);
	return 0;
}

void get_time(struct timer *t)
{
	time(&(t->second));
	SYSTEMTIME t1;
	GetSystemTime(&t1);
	t->millisecond = t1.wMilliseconds;

}
char* get_time_str(char *s,struct timer t) {
	return s;
}