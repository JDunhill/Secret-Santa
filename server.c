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
#include "shared.h"
#include "userIO.h"


#define BACKLOG 10   // how many pending connections queue will hold
#define MAX_SIZE 100 // max buffer size for receiving and sending, unless otherwise specified


size_t used_buffer_bytes = 0;
char buf[MAX_SIZE];
char name[MAX_SIZE];
const char names[10][MAX_SIZE];




int add_giftee(node_n** head);
int draw_names(int count);
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
        
        data += rc;
        left -= rc;
       
    } while (left > 0);
    printf("\nNumber of bytes received: %d\n", rc);
    *num = ntohl(ret);
    return *num;
}

void receive_server_input(int sockfd)
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
        static int count;
        

        if (!fork())
        {                  // this is the child process
            close(sockfd); // child doesn't need the listener
            node_n * head;
            // while loop to allow for repeated inputs until they terminate their connection
            while (1)
            {
                int user_input = receive_int(&user_input, new_fd);
                int number_of_giftees;
                printf("RECIEVED %d From client \n", user_input);
                
                switch (user_input) {

                    case 1: 
                    {
                        number_of_giftees = receive_int(&number_of_giftees, new_fd);
                        
                        printf("User input is: %d\n", number_of_giftees);
                        for (int i = 0; i < number_of_giftees; i++) {
                            receive_server_input(new_fd);
                            add_giftee(&head);
                            
                            // make sure we aren't overflowing array
                            if(count < 10) {
                                count++;
                                strlcpy(names[i], name, sizeof(name));
                            } 
                            strlcpy(buf, "", sizeof(buf)); // clearing the name array
                            strlcpy(name, "", sizeof(name));
                        }
                        // print_list(head);

                        break;
                    }
                    case 2: 
                        printf("Count = %d\n", count);
                        draw_names(count);
                        break;
                    case 3:
                        receive_server_input(new_fd);
                        for(int i = 0; i < count - 1; i++) {
                            // look for the name in the array. If is equal to inputted name, give the next name, unless 
                            // they are at the end of the array, in which case loop back to the start
                            if(strcmp(buf, names[i]) == 0) {
                                if(i != count - 1) {
                                    write(sockfd, names[i + 1], sizeof(names[i + 1]));
                                } else {
                                    write(sockfd, names[0], sizeof(names[0]));
                                }
                            }
                        }
                        break;
                } 
                if (user_input == 4) {
                    quit_connection();
                    free_list(head);
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

int add_giftee(node_n** head)
{
    printf("\nAdding giftee!\n");
    printf("\nName = %s\n", name);
    // add_to_end(head, &name);

    return 0;
}

int draw_names(int count)
{
    printf("\nDrawing names!\n");

    for (int i = 0; i < count; i++) {
        printf("Names: %s\n", names[i]);
    }
    if (count > 1)
    {
        int i;
        //step through each index of the name array
        for (i = 0; i < (count - 1); i++)
        {
            
            int swap_index = rand() % (count - 1);

             while (i == swap_index) {
                swap_index = rand() % count;
            }
            
            if(strcmp(names[i], names[swap_index]) != 0 && strcmp(names[i], names[swap_index]) != 1 && strcmp(names[i], names[swap_index]) != -1) {
                
               
                char temp[MAX_SIZE];
                printf("temp is %s\n", temp);
                strcpy(temp, names[i]);
                strcpy(names[i], names[swap_index]);
                strcpy(names[swap_index], temp);
                printf("Assigned %s to %s\n", names[i], names[swap_index]);
            } else {
                printf("%s and %s are the same!\n", names[i], names[swap_index]);
                i--;
            }

            
        
        }
    }
    for (int i = 0; i < count; i++) {
        printf("Random names: %s\n", names[i]);
    }
    
    return 0;
}

int get_giftee()
{
    printf("\nYour giftee is: \n");
    return 0;
}

int quit_connection()
{
    printf("\nTerminating connection!\n");
    return 0;
}
