#include <sys/syscall.h>
#include <unistd.h>
#include "../header/socket.h"
#include "../header/io.h"
#include "../header/std.h"


/*
    addr is main struct for handle socket connection
*/
struct sockaddr_in addr;

/*
    set ip for handler
*/
void setip(const char* ip){
    addr.sin_addr.s_addr = ip2n(ip);  
}

/*
    set port for handler
*/
void setport(int port){
    addr.sin_port = h2n(port);
}

/*
    setup a socket handler and return a socket file descriptor
*/
int socket(){
    int fd = syscall(SYS_socket, IPV4, TCP, 0);
    _memset(&addr, 0, sizeof(addr));    
    addr.sin_family = IPV4;
    return fd;
}

/*
    listen on ip and port
*/
int listen(int sock, const char* ipaddr, int port){
    int bind, listen;
    if(ipaddr == 0){
        /* loopback address */
        addr.sin_addr.s_addr = 0;
    }else{
        addr.sin_addr.s_addr = ip2n(ipaddr);
    }
    addr.sin_port = h2n(port);
    /*
        bind socket on defined ip and port then start listen
    */
    bind = syscall(SYS_bind, sock, &addr, sizeof(addr));
    listen = syscall(SYS_listen, sock, 0);
    return listen;
}

/*
    accept connection for defined socket file descriptor
*/
int accept(int sockfd){
    int acc;
    acc = syscall(SYS_accept, sockfd, 0, 0);
    return acc;
}

/*
    connect through a socket fd
*/
int connect(int sockfd){
    int conn;
    conn = syscall(SYS_connect, sockfd, &addr, sizeof(addr));
    return conn;
}

/*
    convert host byte-ordered integer to network byte-ordered mode
*/
int h2n(int number){
    int result = 0;
    int temp = 0;
    while (number){
        result = result << 8;
        result += number & 0xff;
        number = number >> 8;
    }
    return result;
}

/*
    convert host byte-ordered ip address string
     to network byte-ordered integer
*/
int ip2n(const char* ip){
    int size = mystrlen(ip);
    char* tempstr;
    int counter = 0;
    int result = 0;
    tempstr = (char*)_malloc(sizeof(char)*size);         
    mystrcpy(tempstr, ip, size);
    int numbers[4];
    for(int i = size-1; i >= 0; i--){
        if(tempstr[i] == '.'){
            numbers[counter] = stoi(&tempstr[i+1]);
            tempstr[i] = 0;
            ++counter;
        }else if(i == 0){
            numbers[counter] = stoi(tempstr);    
        }
    }
    _free(tempstr);                         
    for(int i = 0; i < 4; i++){
        result = result << 8;
        result += numbers[i];
    }
    return result;
}
