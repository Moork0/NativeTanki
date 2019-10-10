#include <unistd.h>
#include <sys/syscall.h>
//#include <string.h>
#include "../header/io.h"
#include "../header/std.h"

/*
  return an string lentgh
*/
int mystrlen(const char* s){
  int size = 0;
  while(*s){
    size++;
    s++;
  }
  return size;
}

/*
  copy 'size' number of chars from 's' to 'd'
*/
void mystrcpy(char *d, const char *s, int size){
  if(size == -1){   // if size = -1 then size would be lentgh of s
    size = mystrlen(s);
  }
  if(size == 0){
    return;
  }
  for(size_t i = 0; i < size; i++){
    *(d+i) = *(s+i);
  }
}

/*
  convert an integer to string. return an dynamic memory.
  
  *****
   USE _free() AFTER DONE!
  *****

*/

char* itos(int number){
  int temp = number;
  size_t size = 0;
  char *str = NULL;
  while(temp){
    temp /= 10;
    size++;
  }

  str = (char *)_malloc(size+1);
  if(str == NULL){
    return NULL;
  }
  for(int i = size-1; i >= 0; i--){
    temp = number%10;
    number /= 10;
    str[i] = (char)(temp+48);
  }
  str[size+1] = 0;
  return str;
}

/*
  interface to write syscall. writing str to fd(file descriptor)
*/
int myfwrite(const char* str, int size, int fd){
  return syscall(SYS_write, fd, str, size);
}

/*
  Make a formated string and store it in dest.
  function returns needed size for dest if dest being 0
  fmt is format style. like : "Hello %s"
  %d stands for integer, %c stands for char and %s for string
  from third parameter, there should be an array of integers, characters and strings, respectively
  'sarr' is a array of pointer to pointers. could be initialize like this : char* arr[] = {"Hello"};
*/
int formatstr(char* dest, const char* fmt, int* iarr, char* carr, char** sarr){
  int ints=0, floats=0, strs=0, chrs = 0, newsize, varsize, argstrsize;
  int len = mystrlen(fmt);
  newsize = len;                                          // size of new string
  char *tempfmt = _malloc((len+1)*(sizeof(char)));        // malloc
  if(tempfmt == NULL){
    return -1;
  }
  mystrcpy(tempfmt, fmt, len);                           // create copy of format for manipulating
  int point = 0;
  char* var = NULL;                                      // for storing itos result
  for(int i = 0; i <= len; i++){
    if(fmt[i] == '%'){                                   // true if we see '%' in fmt
      switch (fmt[i+1]){
      case 'd':
        var = itos(iarr[ints]);                         // malloc
        newsize += mystrlen(var)-2;                     // new string size should be increased for new data
        ints++;                                         // integer array counter
        break;
      case 'c':
        --newsize;                                     // new string size sould be decreased by one. (char has one character and %c has two)
        chrs++;
        break;
      case 's':
        newsize += mystrlen(sarr[strs])-2;            // size + size of and string then subtract that %s
        strs++;
        break;
      // case 'f':
      //   floats++;
      //   break;
      default:
        break;
      }
      _free(var);                                   // free 'var' allocated from itos
      var = NULL;
    }
  }
  ++newsize;                                      // increase because of null sign

  /*
    return the needed size if no destination defined
  */
  if(dest == 0){
    _free(tempfmt);
    return newsize;
  }
  ints = chrs = strs = point = 0; // 'point' is a pointer to a location of unformated string. for continueing reading
  int fpoint = 0; // a point to a location of formated string. for continueing writing
  /*
    allocate memory for new formated stirng
  */
  for(int i = 0; i <= len; i++){
    if(i == len){
      mystrcpy(&dest[fpoint], &tempfmt[point], i-point); // copy everything else that remains
    }
    if(fmt[i] == '%'){      
      switch (fmt[i+1]){
      case 'd':
        mystrcpy(&dest[fpoint], &tempfmt[point], i-point);
        //printf("F: %d|%d\n%s\n", fpoint, point,dest);
        fpoint += i-point;
        point = i+2;
        var = itos(iarr[ints]);             // malloc
        varsize = mystrlen(var);
        mystrcpy(&dest[fpoint], var, varsize);
        //printf("S: %d|%d\n%s\n", fpoint, point,dest);
        fpoint += varsize;
        ints++;
        break;
      case 'c':
        mystrcpy(&dest[fpoint], &tempfmt[point], i-point);
        fpoint += i-point;
        point = i+2;
        dest[fpoint] = carr[chrs];
        ++fpoint;
        ++chrs;
        break;
      case 's':
        mystrcpy(&dest[fpoint], &tempfmt[point], i-point);
        fpoint += i-point;
        point = i+2;
        argstrsize = mystrlen(sarr[strs]);
        mystrcpy(&dest[fpoint], sarr[strs], argstrsize);
        fpoint += argstrsize;
        strs++;
        break;
      // case 'f':
      //   floats++;
      //   break;
      default:
        break;
      }
      _free(var);            // free
      var = NULL;
    }
  }
  _free(tempfmt);            // free
  return 1;
}
/*
  print formated data to fd.
  fmt is format style. like : "Hello %s"
  %d stands for integer, %c stands for char and %s for string
  from third parameter, there should be an array of integers, characters and strings, respectively
  'sarr' is a array of pointer to pointers. could be initialize like this : char* arr[] = {"Hello"};
*/
int fprint(int fd, const char* fmt, int* iarr, char* carr, char** sarr){
  int newstrsize = formatstr(0, fmt, iarr, carr, sarr);
  char* newstr = (char* )_malloc(sizeof(char)*newstrsize);
  if(!newstr){
    return -1;
  }
  _memset(newstr, 0, newstrsize);
  formatstr(newstr, fmt, iarr, carr, sarr);
  myfwrite(newstr, newstrsize, fd);
  _free(newstr);
  return 1;
}

/*
  print a formated string to stdout
*/
void printout(const char* fmt, int* iarr, char* carr, char** sarr){
  fprint(1, fmt, iarr, carr, sarr);
}

/*
  print a constant string to stdout
*/
void myputs(const char* str){
    int size = mystrlen(str);
    myfwrite(str, size, 1);
}

/*
  convert an string to integer
*/
int stoi(const char* str){
  int number = 0;
  int size = mystrlen(str);
  for(int i = 0; i < size; i++){
    number *= 10;
    number += ((int)(str[i]))-48;
  }
  return number;
} 

/*
  interface for read syscall.
*/
int myfread(int fd, char* input){
  int result = syscall(SYS_read, fd, input, 1024);
  return result;
}

/*
  interface for read syscall. with lentgh
*/
int mynfread(int fd, char* input, int len){
  int result = syscall(SYS_read, fd, input, len);
  return result;
}

/*
  read a data from stdin and put data's to correct variables by format specified by 'fmt'
  %d stand for integer, %c for character and %s for string
  iarr, carr are arrays of integers and characters respectively
  sarr is an array of pointer to pointers. could be initialized like this:
  char* arr[2];
  arr[0] = (char*)malloc(sizeof(char)+10); // a place for storing 10 characters
*/

void scan(const char* fmt, int* iarr, char* carr, char** sarr){
  char input[1024];
  int res, size;
  int ints = 0, chars = 0, strs = 0, breakpoint = 0, currentpoint;
  size = mystrlen(fmt);
  res = myfread(0, input); // read 1024 bytes. 
  input[res] = 0; 
  input[res-1] = ' '; // make the last character to be a space. because delimiter is space

  for(size_t i = 0; i < size; i++){
    if(fmt[i] == '%'){
 /*
   current point is a place in 'input' that now we are processing.
   breakpoint is the place of string that we started to proccess
 */
      currentpoint = breakpoint; 
      switch (fmt[i+1]){
      case 'd':
    
        while(input[currentpoint]){
          if(input[currentpoint] == ' '){
            input[currentpoint] = 0;
            iarr[ints] = stoi(&input[breakpoint]);
            breakpoint = currentpoint + 1;
            break;
          }
          ++currentpoint;
        }
        ++ints;
        break;
      case 'c':
        while(input[currentpoint]){
          if(input[currentpoint] == ' '){
            input[currentpoint] = 0;
            carr[chars] = input[breakpoint];
            breakpoint = currentpoint + 1;
            break;
          }
          ++currentpoint;
        }
        ++chars;
        break;
      case 's':
        while(input[currentpoint]){
          if(input[currentpoint] == ' '){
            input[currentpoint] = 0;
            mystrcpy(sarr[strs], &input[breakpoint], currentpoint-breakpoint);
            breakpoint = currentpoint + 1;
            break;
          }
          ++currentpoint;
        }
        ++strs;
        break;
      default:
        break;
      }
    }
  }
}

/*
  Open a file and return file descriptor
*/
int myfopen(const char* pathname, const char flag){
  int flagnum;
  if(flag ==  'r'){
    flagnum = 0;
  }else if(flag == 'w'){
    flagnum = 1;
  }else{
    return -1;
  }
  return syscall(SYS_open, pathname, flagnum);
}

int myfclose(int fd){
  return syscall(SYS_close, fd);
}

void myexit(int code){
  syscall(SYS_exit_group, code);
}