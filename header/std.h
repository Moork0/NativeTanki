#ifndef MYSTD
#define MYSTD 1

#include <unistd.h>
#include <sys/syscall.h>


#define ALLOC_UNIT 3*sysconf(_SC_PAGESIZE)
#define MIN_MEM 1*sysconf(_SC_PAGESIZE)


struct dl{
  struct dl *next;
  struct dl *prev;
  unsigned int size;
  char padding[4];
};

typedef struct dl dl;

#define BLOCK_MEM(ptr) ((void *)((unsigned long)ptr + sizeof(dl)))
#define MEM_BLOCK(ptr) ((void *)((unsigned long)ptr - sizeof(dl)))

void *_sbrk(int size);
void add(dl* block);
void erase(dl *block);
void merge(dl *block);
dl *split(dl *block, unsigned int size);
void *_malloc(unsigned int size);
void _free(void *mem);
void _clean();
void *_memset(void* ptr, int value, size_t n);
#endif