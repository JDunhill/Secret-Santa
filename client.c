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

size_t used_buffer_bytes = 0;
char buf[MAXDATASIZE];

// get sockaddr, IPv4 or IPv6:
// void *get_in_addr(struct sockaddr *sa)
// {
//     if (sa->sa_family == AF_INET)
//     {
//         return &(((struct sockaddr_in *)sa)->sin_addr);
//     }

//     return &(((struct sockaddr_in6 *)sa)->sin6_addr);
// }

// void receive_input(int sockfd)
// {
//     size_t remaining_buffer = MAXDATASIZE - used_buffer_bytes;
//     if (remaining_buffer == 0)
//     {
//         printf("Remaining buffer = 0\n");
//         abort();
//     }
//     int numbytes = recv(sockfd, &buf[used_buffer_bytes], remaining_buffer, 0);
//     if (numbytes == 0)
//     {
//         puts("Numbytes = 0");
//         abort();
//     }
//     if (numbytes == -1)
//     {
//         perror("recv");
//         exit(1);
//     }
//     used_buffer_bytes += numbytes;
//     char *start_ptr = buf;
//     char *end_ptr = NULL;

//     while ((end_ptr = memchr(start_ptr, '\n', (used_buffer_bytes - (start_ptr - buf)))) != NULL)
//     {
//         *end_ptr = '\0';
//         printf("client: received '%s'\n", start_ptr);
//         start_ptr = end_ptr + 1;
//     }
//     used_buffer_bytes -= (start_ptr - buf);
//     memmove(buf, start_ptr, used_buffer_bytes);
// }

// int send_int();
int get_giftee();
int add_giftee(int sockfd);
void menu(int choice, int sockfd);

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

    printf("Welcome to Secret Santa \n");
    while (1)
    {
        // receive_input(sockfd);
        print_menu();
        int choice = user_choice(MENUITEMS);
        int numbytes = send_int(choice, sockfd);
        printf("\nNumber of bytes sent: %d\n", numbytes);
        menu(choice, sockfd);
    }

    close(sockfd);
}

// takes the choice of the user and ensures the correct function is called
void menu(int choice, int sockfd)
{
    if (choice == 1)
    {
        add_giftee(sockfd);
    }
    else if (choice == 2)
    {
        exit(1);
        // draw_names();
    }
    else if (choice == 3)
    {
        exit(1);
        // get_giftee();
    }
    else
    {
        exit(1);
        // quit_program();
    }
}

/*

*/
int add_giftee(int sockfd)
{
    printf("Enter how many names you plan to add \n");
    int choice = user_choice(10);
    send_int(choice, sockfd); // This will let the server run the for loop
    for (int i = 0; i < choice; i++)
    {
        char name[30];
        get_user_string(name);
        int bytes_sent = write(sockfd, name, strlen(name));
        if (bytes_sent < 0)
        {
            perror("ERROR WRITING MESSAGE TO SOCKET");
        }
        printf("%d bytes sent to server!\n", bytes_sent);
    }
}

// int send_int(int num, int fd)
// {
//     int32_t conv = htonl(num);
//     char *data = (char *)&conv;
//     int left = sizeof(conv);
//     int rc;

//     do
//     {
//         rc = write(fd, data, left);
//         // if (rc <= 0)
//         // { /* instead of ret */
//         //     if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
//         //     {
//         //         // use select() or epoll() to wait for the socket to be readable again
//         //     }
//         //     else if (errno != EINTR)
//         //     {
//         //         return -1;
//         //     }
//         // }
//         // else
//         // {
//         data += rc;
//         left -= rc;
//         // }
//     } while (left > 0);
//     return rc;
// }
