#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#define SERVER_PORT 6666
#define BUFFER_SIZE 2000

void sig_chld(int signo)
{
  pid_t pid;
  int stat;
  while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    printf("child %d terminated\n", pid);
  return;
}

int main(int argc, const char *argv[])
{
  struct sockaddr_in server_addr;
  int server_socket;
  pid_t childpid;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(SERVER_PORT);

  if((server_socket=socket(AF_INET,SOCK_STREAM,0)) < 0){
    printf("Create Socket Failed!");
    exit(1);
  }
  if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
    printf("Server Bind Port Failed!");
    exit(1);
  }
  if(listen(server_socket, 10) == -1){
    printf("listen error!");
    exit(1);
  }
  int optval = 1;
  int optlen = sizeof(optval);
  if(setsockopt(server_socket, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0){
    printf("keepalive error");
  }
  signal(SIGCHLD, sig_chld);
  while(1){
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    int new_server_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
    if(new_server_socket < 0){
      if(errno == EINTR)
        continue;
      else
        printf("Server Accept Failed !/n");

    }

   if((childpid = fork()) == 0)
   {
       close(server_socket);
        char buffer[BUFFER_SIZE];
        bzero(buffer, BUFFER_SIZE);
        length = recv(new_server_socket,buffer,BUFFER_SIZE,0);
        if(length < 0){
          printf("Server Recieve Data Failed!/n");
          break;
        }
        char file_name[32];
        bzero(file_name,32);
        strncpy(file_name,buffer,strlen(buffer));

        FILE *fp = fopen(file_name,"w");
        if(NULL == fp){
          printf("File:/t%s Can Not open to write/n", file_name);
          break;
        }
        bzero(buffer, BUFFER_SIZE);
        while(length = recv(new_server_socket,buffer,BUFFER_SIZE,0))
        {
          if(length < 0){
            printf("Recieve data from client failed!/n");
            break;
          }
          int write_length = fwrite(buffer,sizeof(char),length,fp);
          if(write_length<length){
            printf("File:/t%s write failed\n",file_name);
            break;
          }
          bzero(buffer,BUFFER_SIZE);
        }
        printf("Rececive File:/t %s finished\n", file_name);
        fclose(fp);
       close(new_server_socket);
       exit(0);
   }
   close(new_server_socket);

  }
  return 0;
}
