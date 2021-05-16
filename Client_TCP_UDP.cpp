/********************************************************************
*Based on https://blog.csdn.net/qq_28114615/article/details/88421575
*         https://blog.csdn.net/qq_33406883/article/details/107173443
*********************************************************************/
#include "kbhit.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#include<stdio.h>
#include<sys/types.h>
 
using namespace std;

int TCPClient(int,char**);
int UDPClient();

int main(int argc,char** argv)
{
	if(argc == 1)
     {
		 int choosemode;
		 cout<<"请选择连接模式，1.TCP，2.UDP"<<endl;
		 cin >> choosemode;
		 switch (choosemode)
		 {
		 case 1:
			 TCPClient(argc,argv);
			 break;
		 
		 case 2:
			 UDPClient();
			 break;

		 default:
			 break;
		 }
         
     }
     else if (strcmp(argv[1],"tcp")==0)
     {
         cout<<"连接TCP服务器"<<endl;
         TCPClient(argc,argv);
     }
     else if (strcmp(argv[1],"udp")==0)
     {
         cout<<"连接UDP服务器"<<endl;
         UDPClient();
     }
	 else if (strcmp(argv[1],"help")==0)
	 {
		 cout<<"tcp 连接TCP服务器"<<endl;
		 cout<<"udp 连接UDP服务器"<<endl;
		 cout<<"无启动参数，进入程序内选择模式"<<endl;
	 }
	 else
	 {
		 cout<<"无法处理参数,输入help查看帮助"<<endl;
	 }

	 return 0;
}

int TCPClient(int argc,char** argv)
{
	int clitfd;  //文件描述符
	struct sockaddr_in serv_addr;  //目的服务端地址结构体
 
	memset(&serv_addr,0,sizeof(serv_addr));
 
	if(argc!=4)
	{
		cout<<"Input error! Usage should be : "<<argv[0]<<argv[1]<<"  xxx.xxx.xxx.xxx(ip)  1234(port)"<<endl;
		return 0;
	}
 
	if((clitfd = socket(AF_INET,SOCK_STREAM,0)) == -1)  //创建套接字
	{
		cout<<"creat socket failed : "<<strerror(errno)<<endl;
		return 0;
	}
        //将目的服务端的地址信息赋值给地址结构体
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[3]));
	serv_addr.sin_addr.s_addr = inet_addr(argv[2]);
 
	cout<<"try to connect ... "<<endl;
 
        //通过套接字发起连接请求，成功后clitfd套接字则表示此次成功的连接
	if( connect(clitfd,(struct sockaddr*)& serv_addr,sizeof(serv_addr)) == -1)
	{
		cout<<"connet failed : "<<strerror(errno)<<endl;
		return 0;
	}
 
	cout<<"connect success !"<<endl;
 
	while(1)
	{
		char sdbuf[1024];
		char* sdbuf_p;
		sdbuf_p = sdbuf;
		char rvbuf[1024];
		int rdlen,sdlen;

 		cout<<"(Client)send : ";

		//cin.ignore(100, '\n');
		//cin>> sdbuf;
		while((*sdbuf_p = getchar()) != '\n')
		{
			sdbuf_p++;
		}

		*sdbuf_p = '\0';
		if(strcmp(sdbuf,"\n")==0)
		{
			continue; //防止客户端只发一个换行符，此时write不发送数据，服务端就不回回发数据，然后客户端就一直阻塞在read处。
		}
		sdlen = write(clitfd,sdbuf,strlen(sdbuf));//向套接字中写入数据发送

		if(strcmp(sdbuf,"quit")==0)
		{
			break;
		}
        //可能会出现发送端把长度为sdlen的字符串分为多次发送，调用一次read就很有可能不能读取完全，就有以下两种方式进行读取：
        /*1. write了多少字节就读取多少字节长的字符串*/
		rdlen=0;
		rdlen = read (clitfd,&rvbuf[rdlen],sizeof(rvbuf));
		/*while(rdlen<sdlen)//防止发送端将数据分开发送
		{	
			int rdcnt = read(clitfd,&rvbuf[rdlen],sizeof(rvbuf));
			if(rdcnt == -1)
			{
				perror(NULL);
				continue;
			}
			rdlen+=rdcnt;
		}*/
        if(rdlen)
        {
		    rvbuf[rdlen]='\0';
		    cout<<"(Client)recv : "<<rvbuf<<endl;

			if(strcmp(rvbuf,"quit")==0)
			{
				cout<<"服务器准备退出，进行下一步操作"<<endl;
				while(rdlen = read (clitfd,&rvbuf[rdlen],sizeof(rvbuf)))//等待服务器关闭
				{

				}
				cout<<"服务器已经关闭 "<<endl;
		    	cout<<"客户端即将关闭"<<endl;
				break;
			}
        }
        else 
		{
	        cout<<"Server has closed ! "<<endl;
		    cout<<"Client will close..."<<endl;
		break;
		}
 
 
        /*2. 用recv函数中的MSG_WAITALL参数，读到指定长度的数据才返回*/
        /*if(rdlen = recv(clitfd,&rvbuf[rdlen],sdlen,MSG_WAITALL))
		{	
			rvbuf[rdlen]='\0';
	
			cout<<"(Client)recv : "<<rvbuf<<endl;
		}
		else 
		{
			cout<<"Server has closed ! "<<endl;
			cout<<"Client will close..."<<endl;
			break;
		}*/
 
	}

	//cout<<"即将断开连接"<<endl;
	cout<<"释放套接字资源"<<endl;
	close(clitfd);
	
	cout<<"客户端关闭"<< endl;
	return 0;
}

int UDPClient()
{
     //创建socket对象
     int sockfd=socket(AF_INET,SOCK_DGRAM,0);
 
     //创建网络通信对象
     struct sockaddr_in addr;
     addr.sin_family =AF_INET;
     addr.sin_port =htons(1324);
     addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //IP根据server所在主机的IP设定，这里是运行在同一PC端 
 
     while(1)
     {
         printf("请输入一个数字：");
         char buf=0;
         scanf("%hhd",&buf);
         sendto(sockfd,&buf,
         sizeof(buf),0,(struct sockaddr*)&addr,sizeof(addr));
 
         socklen_t len=sizeof(addr);
         recvfrom(sockfd,&buf,sizeof(buf),0,(struct sockaddr*)&addr,&len);
 
         if(66 ==buf)
         {
            printf(" server 成功接受\n");
         }
         else
         {
             printf("server 数据丢失\n");
         }
 
     }
     close(sockfd); 
}
