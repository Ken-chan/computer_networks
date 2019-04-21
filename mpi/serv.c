#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
  (void) signal(SIGCHLD, SIG_IGN);
  int  sockfd, clilen, n;
  double b = 0, e = 0, h = 0, tmp, s = 0;
  struct sockaddr_in servaddr, cliaddr;

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family      = AF_INET;
  servaddr.sin_port        = htons(51038);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
     printf("Can\'t create socket, errno = %d\n", errno);
     exit(1);
  }

  if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
     printf("Can\'t bind socket, errno = %d\n", errno);
     close(sockfd);
     exit(1);
  }

  double data[3];

  while(1) {

    socklen_t clilen = sizeof(cliaddr);

    if( ( n = recvfrom(sockfd, data, sizeof(data), 0, (struct sockaddr *) &cliaddr, &clilen)) < 0){
      printf("Can\'t receive request, errno = %d\n", errno);
      close(sockfd);
      exit(1);
    }
    b = data[0];
    e = data[1];
    h = data[2];
    pid_t pid = fork();
    if( pid == 0)
    {
      s = 0;
      while (b + h <= e)
      {
        s += h * (sqrt(4 - b * b) + sqrt(4 - (b+h) * (b+h)))/2;
        b += h;
      }

      if(sendto(sockfd, &s, sizeof(double), 0, (struct sockaddr *) &cliaddr, clilen) < 0){
        printf("Can\'t send answer, errno = %d\n", errno);
        close(sockfd);
        exit(1);
      }
      return 0;

    }
    if (e > 2 - h)
      return 0;

  }    

  return 0;
}
