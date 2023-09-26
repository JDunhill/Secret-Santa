#ifndef __SHARED_H_
#define __SHARED_H_

#define PORT "3490"     // The port used by the client and server
#define MAXDATASIZE 100 // Max number of bytes that we can recieve at once

void *get_in_addr(struct sockaddr *sa);

void recieve_input(int sockfd, size_t used_buffer_bytes);

int send_int(int num, int fd);

#endif