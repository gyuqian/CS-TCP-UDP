/********************************************************************
*Based on https://blog.csdn.net/qq_28114615/article/details/88421575
*         https://blog.csdn.net/qq_33406883/article/details/107173443
*********************************************************************/
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#include <iostream>
#include <string.h>
#include <errno.h>

#include <cstring>

using namespace std;

//TCP 
#define SERV_PORT 8888        //服务器端口
#define SERV_IP "127.1.1.1"   //服务器ip

int UDP_Server();
int TCP_Server(int,char**);

int main(int argc,char** argv)
{
     if(argc == 1)
     {
		 int choosemode;
		 cout<<"请选择工作模式，1.TCP，2.UDP"<<endl;
		 cin >> choosemode;
		 switch (choosemode)
		 {
		 case 1:
			 TCP_Server(argc,argv);
			 break;
		 
		 case 2:
			 UDP_Server();
			 break;

		 default:
			 break;
		 }
         
     }
     else if (strcmp(argv[1],"tcp")==0)
     {
         cout<<"手动选择TCP模式"<<endl;
         TCP_Server(argc,argv);
     }
     else if (strcmp(argv[1],"udp")==0)
     {
         cout<<"手动选择UDP模式"<<endl;
         UDP_Server();
     }
	 else if (strcmp(argv[1],"help")==0)
	 {
		 cout<<"tcp 选择TCP工作模式"<<endl;
		 cout<<"udp 选择UDP工作模式"<<endl;
		 cout<<"无启动参数，进入程序内选择模式"<<endl;
	 }
	 else
	 {
		 cout<<"无法处理参数,输入help查看帮助"<<endl;
	 }

	 return 0;
}

int UDP_Server()
{
    //创建socket对象
     int sockfd=socket(AF_INET,SOCK_DGRAM,0);
 
     //创建网络通信对象
     struct sockaddr_in addr;
     addr.sin_family =AF_INET;
     addr.sin_port =htons(1324);
     addr.sin_addr.s_addr=inet_addr("127.0.0.1");

     //绑定socket对象与通信链接
     int ret =bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
     if(ret<0)
     {
         printf("bind\n");
         return -1;
 
     }
     struct sockaddr_in cli;
     socklen_t len=sizeof(cli);
 
     while(1)
     {
         char buf =0;
         recvfrom(sockfd,&buf,sizeof(buf),0,(struct sockaddr*)&cli,&len);
         printf("recv num =%hhd\n",buf);
 
         buf =66;
         sendto(sockfd,&buf,sizeof(buf),0,(struct sockaddr*)&cli,len);
 
     }
     close(sockfd);
}

int TCP_Server(int argc,char** argv)
{
	int servfd,clitfd;   //创建两个文件描述符，servfd为监听套接字，clitfd用于数据传输
	struct sockaddr_in serv_addr,clit_addr; //创建地址结构体，分别用来存放服务端和客户端的地址信息
 
	memset(&serv_addr,0,sizeof(serv_addr));  //初始化
	memset(&clit_addr,0,sizeof(clit_addr));  //初始化
 
	if((servfd = socket(AF_INET,SOCK_STREAM,0)) == -1)  //创建套接字
	{
		cout<<"creat socket failed : "<<strerror(errno)<<endl;//如果出错则打印错误
		return 0;
	}
        
        //给服务端的地址结构体赋值
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT); //将主机上的小端字节序转换为网络传输的大端字节序（如果主机本身就是大端字节序就不用转换了）
	serv_addr.sin_addr.s_addr = inet_addr(SERV_IP); //将字符串形式的ip地址转换为点分十进制格式的ip地址
 
        //绑定地址信息到监听套接字上，第二个参数强转是因为形参类型为sockaddr ，而实参类型是sockaddr_in 型的
	if(bind(servfd,(sockaddr *)& serv_addr,sizeof(serv_addr)) == -1)
	{
		cout<<"bind failed : "<<strerror(errno)<<endl;
		return 0;
	}
        
        //将servfd套接字置为监听状态
	if(listen(servfd,1024) == -1)
	{
		cout<<"listen failed : "<<strerror(errno)<<endl;
		return 0;
	}
 
	cout<<"Init Success ! "<<endl;
	cout<<"ip : "<<inet_ntoa(serv_addr.sin_addr)<<"  port : "<<ntohs(serv_addr.sin_port)<<endl;
	cout<<"Waiting for connecting ... "<<endl;
 
	socklen_t clit_size = 0; //用于accept函数中保存客户端的地址结构体大小
 
        //accept成功后，clitfd则指向了这条服务端与客户端成功连接的”通路“
	if((clitfd = accept(servfd,(sockaddr *)& clit_addr,&clit_size)) == -1)
	{
		cout<<"accept failed : "<<strerror(errno)<<endl;
		return 0;
	}
 
	cout<<"Client access : "<<inet_ntoa(clit_addr.sin_addr)<<"  "<<ntohs(clit_addr.sin_port)<<endl;
 
 
	char buf[1024]; //用于读写数据
 
	while(1)
	{
		int rdstate;
		rdstate = read(clitfd,buf,sizeof(buf));//通过clitfd来读取数据，返回值为读取的长度
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
		if(rdstate > 0 )
		{
			int i=0;
			cout<<"(Server)recv : ";
			for(i=0;i<rdstate;i++)
			{
				cout<<buf[i];
				buf[i] = toupper(buf[i]); //转换为大写
			}
			buf[i]='\0';
			cout<<endl;

			if(strcmp(buf,"QUIT")==0) //检测到客户端准备退出
			{
				cout<<"客户端准备退出，进行下一步操作"<<endl;
				continue;
			}
			else
			{
				char sdbuf[100];
				char* sdbuf_p;
				sdbuf_p = sdbuf;
				cout<<"(Server)send : ";
				while(( *sdbuf_p= getchar()) != '\n')
				{
					sdbuf_p++;
				}
				*sdbuf_p = '\0';
				//cout<<sdbuf<<endl;

				if(strcmp(sdbuf,"\n")==0)
				{
					continue; //防止只发一个换行符，此时write不发送数据，一直阻塞在read处。
				}

				write(clitfd,sdbuf,strlen(sdbuf));
				if(strcmp(sdbuf,"quit")==0)
				{
					break;
				}
				//write(clitfd,buf,strlen(buf)); //发回客户端
			}
		}

        else if(rdstate == 0)  //客户端退出
		{
			cout<<"客户端退出 ! "<<endl;
			break;
		}	
	}

	cout<<"即将关闭套接字"<<endl;
	close(servfd);  //关闭套接字
	close(clitfd);  
	cout<<"关闭套接字"<<endl;

	cout<<"服务器关闭"<<endl;
	return 0;
}
