#include "socket.h"
#include <sys/syscall.h>
#include <unistd.h>
#include "../header/io.h"
#include "../header/std.h"

/*       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
*/

struct sockaddr_in addr;

void setip(const char* ip){
    addr.sin_addr.s_addr = ip2n(ip); // 0x0100007f 
}

void setport(int port){
//    addr.sin_port = 0xd207;
    addr.sin_port = h2n(port);
}

int socket(){
    int fd = syscall(SYS_socket, IPV4, TCP, 0);
    _memset(&addr, 0, sizeof(addr));    
    addr.sin_family = IPV4;
    return fd;
}

int listen(int sock, const char* ipaddr, int port){
    int bind, listen;
    if(ipaddr == 0){
        addr.sin_addr.s_addr = 0;
    }else{
        addr.sin_addr.s_addr = ip2n(ipaddr);
    }
//  servaddr.sin_port = (htons(port));
    addr.sin_port = h2n(port);
    bind = syscall(SYS_bind, sock, &addr, sizeof(addr));
    listen = syscall(SYS_listen, sock, 0);
    return listen;
}

int accept(int sockfd){
    int acc;
    acc = syscall(SYS_accept, sockfd, 0, 0);
    return acc;
}

int connect(int sockfd){
    int conn;
    conn = syscall(SYS_connect, sockfd, &addr, sizeof(addr));
    return conn;
}


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


int ip2n(const char* ip){
    int size = mystrlen(ip);
    char* tempstr;
    int counter = 0;
    int result = 0;
    tempstr = (char*)_malloc(sizeof(char)*size);         // malloc
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
    _free(tempstr);                         // free
    for(int i = 0; i < 4; i++){
        result = result << 8;
        result += numbers[i];
    }
    return result;
}
