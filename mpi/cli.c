#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{

    int  sockfd, n, len;
    double b = 0, e = 0, h = 0.0001, tmp, s = 0, S = 0;
    struct sockaddr_in servaddr, cliaddr;
    unsigned short port;


    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;

    port = 51038;
    servaddr.sin_port   = htons(port);

    if(inet_aton("10.55.165.200", &servaddr.sin_addr) == 0){
       printf("Invalid IP address\n");
       exit(1);
    }

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
       printf("Can\'t create socket, errno = %d\n", errno);
       exit(1);
    }

    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family      = AF_INET;
    cliaddr.sin_port        = htons(0);
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0){
       printf("Can\'t bind socket, errno = %d\n", errno);
       close(sockfd);
       exit(1);
    }

    double data[3];
    double N =  atof(argv[1]);
    double H = 2 / N;
    e = H;

    socklen_t servlen = sizeof(servaddr);

    while (e <= 2)
    {
      data[0] = b;
      data[1] = e;
      data[2] = h;
      sendto(sockfd, data, sizeof(data), 0, (struct sockaddr *) &servaddr, servlen);
      b = e;
      e += H;
    }

    while (N >= 0)
    {
      if((n = recvfrom(sockfd, &s, sizeof(double), 0, (struct sockaddr *) &servaddr, &servlen)) < 0){
        printf("Can\'t receive answer, errno = %d\n", errno);
        close(sockfd);
        exit(1);
      }
      //printf("%f\n", s);
      S += s;
      N --;
    }

    printf("%f\n", S);
    close(sockfd);
    return 0;
}


