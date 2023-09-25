#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once

size_t used_buffer_bytes = 0;
char buf[MAXDATASIZE];

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void receive_input(int sockfd)
{
    size_t remaining_buffer = MAXDATASIZE - used_buffer_bytes;
    if (remaining_buffer == 0)
    {
        printf("Remaining buffer = 0\n");
        abort();
    }
    int numbytes = recv(sockfd, &buf[used_buffer_bytes], remaining_buffer, 0);
    if (numbytes == 0)
    {
        puts("Numbytes = 0");
        abort();
    }
    if (numbytes == -1)
    {
        perror("recv");
        exit(1);
    }
    used_buffer_bytes += numbytes;
    char *start_ptr = buf;
    char *end_ptr = NULL;

    while ((end_ptr = memchr(start_ptr, '\n', (used_buffer_bytes - (start_ptr - buf)))) != NULL)
    {
        *end_ptr = '\0';
        printf("client: received '%s'\n", start_ptr);
        start_ptr = end_ptr + 1;
    }
    used_buffer_bytes -= (start_ptr - buf);
    memmove(buf, start_ptr, used_buffer_bytes);
}

void printMenu();

int main(int argc, char *argv[])
{
    int sockfd;

    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2)
    {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    while (1)
    {
        receive_input(sockfd);
    }

    close(sockfd);
}

void printMenu();