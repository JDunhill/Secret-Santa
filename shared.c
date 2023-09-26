#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "userIO.h"
#include <arpa/inet.h>
#include "shared.h"

#define PORT "3490"     // the port client will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once
#define MENUITEMS 4     // Number of items within the menu

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void receive_input(int sockfd, size_t used_buffer_bytes, char buf[MAXDATASIZE])
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

int send_int(int num, int fd)
{
    int32_t conv = htonl(num);
    char *data = (char *)&conv;
    int left = sizeof(conv);
    int rc;

    do
    {
        rc = write(fd, data, left);
        // if (rc <= 0)
        // { /* instead of ret */
        //     if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
        //     {
        //         // use select() or epoll() to wait for the socket to be readable again
        //     }
        //     else if (errno != EINTR)
        //     {
        //         return -1;
        //     }
        // }
        // else
        // {
        data += rc;
        left -= rc;
        // }
    } while (left > 0);
    return rc;
}