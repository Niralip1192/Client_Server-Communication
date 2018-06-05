#include <sys/types.h>
#include<signal.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
 int socket_desc , client_sock ,c , read_size,file;
    struct sockaddr_in server , client;
    char client_message[2000],buff,ch;
    int fd,fd1,c1=0,b;
  
    pid_t childpid;
void  serviceClient();
int main(int argc , char *argv[])
{
    int status;
    if(argc!=2) 
    {
	printf("run program like: program_name port_numebr");
    }
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( atoi(argv[1]) );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
while(1)
{     

	memset(&client, '\0', sizeof(client));
  
  //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
       exit(1);  
    printf("connection accepted from %s: %d \n ", inet_ntoa(client.sin_addr),ntohs(client.sin_port));

     if((childpid=fork())==0)
		{
			dup2(client_sock,socket_desc);
                       serviceClient();	
		//		alarm(1);
		//	signal(SIGALRM, serviceClient);
              }
      else
           wait(&status);
	
      
  
     close(client_sock);
   }  

}

void serviceClient()
{
      int savestdin;
      
     printf("process is %d",getpid());
	while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    			{
        //Send the message back to client
printf("pid %d",getpid())                                 ;
		                    savestdin=dup(1);
                                   puts(client_message);
				   if(strcmp(client_message,"quit")==0)
                                       exit(1);
			              dup2(client_sock,fileno(stdout));
                                     
						system(client_message);
        				write(client_sock,client_message,strlen(client_message));
			//		memset(&client_message, '\0', sizeof(client_message));

						fflush(stdout);
					dup2(savestdin,1);
                                       dup2(client_sock,savestdin);
                              //     puts(client_message);
					memset(client_message,'\0',sizeof(client_message));
      				}
			
}

