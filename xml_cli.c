#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_PORT 6666
#define BUFFER_SIZE 2000

int main(int argc, const char *argv[])
{
  struct sockaddr_in client_addr;
  bzero(&client_addr, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = htons(INADDR_ANY);
  client_addr.sin_port = htons(0);

  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(client_socket < 0){
    printf("create socket failed!\n");
    exit(1);
  }
  if(bind(client_socket, (struct sockaddr*)&client_addr, sizeof(client_addr)) == -1){
    printf("client bind port failed !\n");
    exit(1);
  }

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0){
    printf("inet_pton error!");
    exit(1);
  }
  server_addr.sin_port = htons(SERVER_PORT);
  if(connect(client_socket,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
    printf("connect error");
    exit(1);
  }
//  while(1){

    char file_name[32] = "dataxml";
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);
    strncpy(buffer, file_name, strlen(file_name));
    write(client_socket,buffer,strlen(buffer));

    FILE *fp = fopen(file_name, "r");
    if(NULL == fp){
      printf("FILE not found");
      exit(1);
    }
    bzero(buffer, BUFFER_SIZE);
    int file_block_length = 0;
    while((file_block_length = fread(buffer,sizeof(char), BUFFER_SIZE, fp))>0){
      printf("file_block_length = %d\n", file_block_length);
      if(send(client_socket,buffer,file_block_length,0) < 0){
      printf("send file content failed\n");
      break;
      }
      bzero(buffer, BUFFER_SIZE);
    }
    fclose(fp);
    printf("file:/t %s transfer finished\n", file_name);
    sleep(5);
  //}
  close(client_socket);

  return 0;
}
