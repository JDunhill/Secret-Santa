#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include "server.h"


#define PORT "3490" // the port users will be connecting to

#define BACKLOG 10 // how many pending connections queue will hold
#define MAX_SIZE 100 // max buffer size for receiving and sending, unless otherwise specified

size_t used_buffer_bytes = 0;
char buf[MAX_SIZE];
char name[MAX_SIZE];

int add_giftee(list_n list_start);
int draw_names();
int get_giftee();
int quit_connection();

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

// function for recieving int from server and parsing it
int receive_int(int *num, int fd)
{
    int32_t ret;
    char *data = (char *)&ret;
    int left = sizeof(ret);
    int rc;
    
    // ensuring the whole message is read
    do
    {
        rc = read(fd, data, left);
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
    printf("\nNumber of bytes received: %d\n", rc);
    *num = ntohl(ret);
    return *num;
}

void receive_input(int sockfd)
{
    size_t remaining_buffer = MAX_SIZE - used_buffer_bytes;
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
        printf("Server: received '%s'\n", start_ptr);
        strcpy(name, start_ptr);
        start_ptr = end_ptr + 1;
    }
    used_buffer_bytes -= (start_ptr - buf);
    memmove(buf, start_ptr, used_buffer_bytes);
}

int main(void)
{
    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while (1)
    { // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }

        // convert address from binary to text
        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);

        printf("server: got connection from %s\n", s);

        char buffer[MAX_SIZE];
        

        if (!fork())
        {                  // this is the child process
            close(sockfd); // child doesn't need the listener
            list_n list_start = create_list();
            // while loop to allow for repeated inputs until they terminate their connection
            while(1) {
                int user_input = receive_int(&user_input, new_fd);
                printf("RECIEVED %d From client \n", user_input);
                switch (user_input) {

                    case 1: 
                        receive_input(new_fd);
                        sleep(2);
                        add_giftee(list_start);
                        print_list(list_start);
                        break;
                    case 2: 
                        draw_names();
                        break;
                    case 3:
                        get_giftee();
                        break;
                } 
                if (user_input == 4) {
                    quit_connection();
                    free_list(&list_start);
                    break;
                }
            }
        
            close(new_fd);
            exit(0);
        }

        

        close(new_fd); // parent doesn't need this
    }
    
    return 0;
}

int add_giftee(list_n list_start) {
    printf("\nAdding giftee!\n");
    add_to_front(&list_start, &name);
    print_list(list_start);
    return 0;
}

int draw_names() {
    printf("\nDrawing names!\n");
    return 0;
}

int get_giftee() {
    printf("\nYour giftee is: \n");
    return 0;
}

int quit_connection() {
    printf("\nTerminating connection!\n");
    return 0;
}


