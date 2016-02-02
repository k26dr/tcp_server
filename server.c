// Credit:
// http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdlib.h>

void error(char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

int main (int argc, char *argv[]) {
    int sockfd, newsockfd, portno, clilen, n;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2) 
        error("ERROR: No port provided");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));

    // set server-side socket address
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // bind to socket
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on socket binding."); 
    listen(sockfd, 5);
    printf("Listening on port %s\n", argv[1]);

    // wait on client connection
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");
    
    // read message from socket
    bzero(buffer, 256);
    n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    printf("Here is the message: %s\n", buffer);

    // Write out acknowledgement
    n = write(newsockfd, "I got your message", 18);
    if (n < 0) error("ERROR writing to socket");

    return 0;
}
