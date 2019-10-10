#ifndef OWNIO
    
#define OWNIO 1

#include <unistd.h>
#include <sys/syscall.h>
//#include <string.h>
#include "std.h"
#define CLEAR() myputs("\e[1;1H\e[2J")  /* clear the terminal */
#define EXIT_SUCCESS 0

int mystrlen(const char* s);
void mystrcpy(char *d, const char *s, int size);
char* itos(int number);
int myfopen(const char* pathname, const char flag);
int myfclose(int fd);
int myfwrite(const char* str, int size, int fd);
int myfread(int fd, char* input);
int mynfread(int fd, char* input, int len);
int formatstr(char* dest, const char* fmt, int* iarr, char* carr, char** sarr);
int fprint(int fd, const char* fmt, int* iarr, char* carr, char** sarr);
void printout(const char* fmt, int* iarr, char* carr, char** sarr);
void myputs(const char* str);
int stoi(const char* str);
void scan(const char* fmt, int* iarr, char* carr, char** sarr);
void myexit(int code);

#endif