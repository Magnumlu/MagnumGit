/************************************************
  Project:Socket communication demo server codes
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
	sfd=socket(AF_INET,SOCK_STREAM,0);//服务器端生成一个socket描述符
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;//该结构体变量存储ip、port等socket信息数据
	memset(&ser,0,sizeof(ser));//清空结构体
	ser.sin_family=AF_INET;//采用ipv4协议
	ser.sin_port=htons(atoi(argv[2]));//端口号，将主机字节序的port转换为网络字节序的port
	ser.sin_addr.s_addr=inet_addr(argv[1]);//ip，将点分十进制ip转换为网络字节序的32位二进制数值
	int ret;
	ret=bind(sfd,(struct sockaddr*)&ser,sizeof(ser));//将socket描述符与服务器的ip、port绑定
	if(-1==ret)
	{
		perror("bind");
		return -1;
	}
	listen(sfd,10);
	//使服务器的这个端口和ip处于监听状态，等待客户机的连接请求;10:同时能处理的最大连接请求数
	int new_fd=-1;
	struct sockaddr_in cli;
	int len=sizeof(cli);
	char buf[128]={0};
	fd_set rdset;
	fd_set tmpset;//tmpset记录我们要监控的描述符
	FD_ZERO(&tmpset);//清空描述符集
	FD_SET(0,&tmpset);//将标准输入注册为要监控的描述符
	FD_SET(sfd,&tmpset);//将sfd注册为要监控的描述符
	while(1)
	{
		FD_ZERO(&rdset);
		memcpy(&rdset,&tmpset,sizeof(fd_set));//将需要监控的描述符集拷贝到rdset
		ret=select(11,&rdset,NULL,NULL,NULL);//select函数监控rdset描述符集中相关文件的读变化
		if(ret>0)
		{
			if(FD_ISSET(sfd,&rdset))//检查sfd是否发生读变化，即是否侦听到客户端的链接请求
			{
				new_fd=accept(sfd,(struct sockaddr*)&cli,&len);
				//接收客户端的连接请求，并返回一个新的socket描述符，用于标识服务器与这个特定客户端的连接
				if(-1==new_fd)
				{
					perror("accept");
					return -1;
				}
				FD_SET(new_fd,&tmpset);//将该新的描述符加入监控集
				printf("Server:已成功连接到客户端%s:%d,您可以向客户端发送信息了!\n",inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
			}
			if(FD_ISSET(new_fd,&rdset))//检查new_fd是否发生读变化，即客户端是否发送信息过来
			{
				memset(buf,0,sizeof(buf));
				ret=recv(new_fd,buf,sizeof(buf),0);//接收客户端发来的信息
				if(ret>0)
				{
					printf("Client:%s\n",buf);
				}else if(0==ret)
				{
					printf("byebye,客户端已下线!\n");
					//close(new_fd);
					FD_CLR(new_fd,&tmpset);//从集合当中移除该new_fd
				}
			}
			if(FD_ISSET(0,&rdset))//检查服务器的标准输入是否发生读变化，即服务器是否有信息要发送给客户端
			{
				memset(buf,0,sizeof(buf));
				ret=read(0,buf,sizeof(buf));//读取标准输入的输入信息
				if(ret>0)
				{
					ret=send(new_fd,buf,strlen(buf)-1,0);//将标准输入的信息发送给客户端
					if(ret<=0)
					{
						printf("Server:您未输入任何信息，请重新输入!\n");//输入为空的处理
					}
				}else{
					printf("byebye,服务器准备退出!\n");
					break;
				}
			}
		}
	}
	close(new_fd);
	close(sfd);
	return 0;
}
	
