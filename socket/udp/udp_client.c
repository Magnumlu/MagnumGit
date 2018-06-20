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
	ser.sin_addr.s_addr=inet_addr(argv[1]);//udp进行bind后，端口就打开；将生成的sfd与服务器的socket信息数据绑定
	int ret;
	int len=sizeof(ser);
	ret=sendto(sfd,"I am udpclient",14,0,(struct sockaddr*)&ser,len);
	if(-1==ret)
	{
		perror("sendto");
		return -1;
	}
	char buf[128]={0};
	struct sockaddr_in cli;
	memset(&cli,0,sizeof(cli));
	ret=recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr*)&cli,&len);
	if(-1==ret)
	{
		perror("recvfrom");
		return -1;
	}
	//printf("client recvfrom %s\n",buf);
	close(sfd);
	return 0;
}
