/************************************************
  Project:Socket communication demo client codes
  Author :Lumengru
  **********************************************/
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("error args!请输入命令行参数ip和port:\n");
		return -1;
	}
	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);//客户端生成一个socket描述符
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;//该结构体变量存储ip、port等服务器端的socket信息数据
	memset(&ser,0,sizeof(ser));//清空结构体
	ser.sin_family=AF_INET;//采用ipv4协议
	ser.sin_port=htons(atoi(argv[2]));//端口号，将主机字节序的port转换为网络字节序的port
	ser.sin_addr.s_addr=inet_addr(argv[1]);//ip，将点分十进制ip转换为网络字节序的32位二进制数值
	int ret;
	//向服务器发出连接请求，将客户端生成的sfd描述符信息连接到ip、port指定的服务器上去
	ret=connect(sfd,(struct sockaddr*)&ser,sizeof(ser));
	if(-1==ret)
	{
		perror("connect");
		return -1;
	}
	printf("Client:已成功连接到服务器,您可以向服务器发送消息了!\n");
	char buf[128]={0};
	fd_set rdset;//定义一个监控描述符集
	while(1)
	{
		FD_ZERO(&rdset);//清空监控描述符集
		FD_SET(0,&rdset);//将标准输入加入监控
		FD_SET(sfd,&rdset);//将与服务器通信的sfd加入监控，以监控服务器是否有消息传送过来
		ret=select(sfd+1,&rdset,NULL,NULL,NULL);//用select函数将监控描述符集rdset的读变化
		if(ret>0)
		{
			if(FD_ISSET(sfd,&rdset))//检查sfd是否有读变化，即判断服务器是否有信息发送过来
			{
				memset(buf,0,sizeof(buf));
				ret=recv(sfd,buf,sizeof(buf),0);//接收服务器发送的信息,存入缓冲区buf
				if(ret>0)
				{
					printf("Server:%s\n",buf);
				}else if(0==ret)
				{
					printf("byebye,服务器已下线,聊天终止!\n");
					break;
				}
			}
			if(FD_ISSET(0,&rdset))//检查标准输入是否有读变化，即检查客户端标准输入是否有用户输入
			{
				memset(buf,0,sizeof(buf));
				ret=read(0,buf,sizeof(buf));//读取标准输入
				if(ret>1)
				{
					ret=send(sfd,buf,strlen(buf)-1,0);//将用户输入信息发送给服务器
				}else if(ret=1){
					printf("Client:您未输入任何信息,请重新输入!\n");
				}else{
					printf("byebye,客户端准备退出，聊天即将关闭!\n");
					break;
				}
			}
		}
	}
	close(sfd);
	return 0;
}
