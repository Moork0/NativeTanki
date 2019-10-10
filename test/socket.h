#define IPV4 2
#define TCP 1

struct sockaddr {
    unsigned short sa_family;
    char sa_data[14];
};

struct in_addr {
    unsigned long s_addr;        /* Internet address (32 bits) */
};

struct sockaddr_in{
    unsigned short sin_family;  /* Internet protocol (AF_INET) */
    unsigned short sin_port;    /* Address port (16 bits) */
    struct in_addr sin_addr;    /* Internet address (32 bits) */
    char sin_zero[8];           /* Not used. Just Padding */
};

int ip2n(const char* ip);
int h2n(int number);