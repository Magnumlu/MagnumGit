/************************************************
  Project:Socket communication demo(udp) server codes
  Author :Lumengru
  **********************************************/
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
	sfd=socket(AF_INET,SOCK_DGRAM,0);//服务器生成一个socket描述符sfd，采用ipv4和udp协议通信
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
	ret=bind(sfd,(struct sockaddr*)&ser,sizeof(ser));//udp进行bind后，端口就打开；将生成的sfd与服务器的socket信息数据绑定
	if(-1==ret)
	{
		perror("bind");
		return -1;
	}
	char buf[128]={0};
	struct sockaddr_in cli;//定义客户端的socket地址结构，用于接收客户端的ip和port信息
	int len;
	while(1)
	{
		memset(&buf,0,sizeof(buf));
		memset(&cli,0,sizeof(cli));
		len=sizeof(cli);
		ret=recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr*)&cli,&len);
		if(-1==ret)
		{
			perror("recvfrom");
			return -1;
		}
		printf("UDP Server:客户端发来的信息是%s\n",buf);
	}
	close(sfd);
	return 0;
}
