#ifndef SOCK_H
#define SOCK_H 1
#define IPV4 2
#define TCP 1

struct sockaddr {
    unsigned short sa_family;
    char sa_data[14];
};

/*
    structure for internet address
*/
struct in_addr {
    unsigned long s_addr;        /* Internet address (32 bits) */
};

/*
    structure of socket-internet address and port
*/
struct sockaddr_in{
    unsigned short sin_family;  /* Internet protocol (AF_INET) */
    unsigned short sin_port;    /* Address port (16 bits) */
    struct in_addr sin_addr;    /* Internet address (32 bits) */
    char sin_zero[8];           /* Not used. Just Padding */
};

void setip(const char* ip);
void setport(int port);
int socket();
int listen(int sock, const char* ipaddr, int port);
int accept(int sockfd);
int connect(int sockfd);
int h2n(int number);
int ip2n(const char* ip);
#endif