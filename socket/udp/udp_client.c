/************************************************
  Project:Socket communication demo(udp) client codes
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
		printf("error args,请输入命令行参数ip和port:\n");
		return -1;
	}
	int sfd;
	sfd=socket(AF_INET,SOCK_DGRAM,0);//客户端生成一个socket描述符sfd，采用ipv4和udp协议通信
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;//定义服务器的socket地址结构ser，指明服务器的通信协议、ip地址，端口号
	memset(&ser,0,sizeof(ser));//清空地址结构
	ser.sin_family=AF_INET;//采用ipv4协议
	ser.sin_port=htons(atoi(argv[2]));//端口号，将主机字节序的port转换为网络字节序的port
	ser.sin_addr.s_addr=inet_addr(argv[1]);//ip，将点分十进制的主机字节序的ip转换为32位二进制数值的网络字节序的ip
	int ret;
	int len=sizeof(ser);
	ret=sendto(sfd,"I am udpclient",14,0,(struct sockaddr*)&ser,len);
	if(-1==ret)
	{
		perror("sendto");
		return -1;
	}
	printf("Client:已成功连接到服务器,请向服务器发送信息:\n");
	char buf[128]={0};
	//struct sockaddr_in cli;
	//memset(&cli,0,sizeof(cli));
	while(1)
	{
		memset(&buf,0,sizeof(buf));
		read(0,buf,sizeof(buf));
		ret=sendto(sfd,buf,strlen(buf)-1,0,(struct sockaddr*)&ser,len);
		//ret=recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr*)&cli,&len);
		if(-1==ret)
		{
			perror("sendto");
			return -1;
		}
	}
	close(sfd);
	return 0;
}
