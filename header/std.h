#ifndef MYSTD
#define MYSTD 1

#include <unistd.h>
#include <sys/syscall.h>


#define ALLOC_UNIT 3*sysconf(_SC_PAGESIZE)		/* Minimum number of bytes required for asking from OS */
#define MIN_MEM 1*sysconf(_SC_PAGESIZE)			/* Minimum number of bytes required for releasing to OS */


struct dl{
  struct dl *next;
  struct dl *prev;
  unsigned int size;
  char padding[4];
};

typedef struct dl dl;

#define BLOCK_MEM(ptr) ((void *)((unsigned long)ptr + sizeof(dl)))		/* convert a struct dl pointer to a pure memory pointer */
#define MEM_BLOCK(ptr) ((void *)((unsigned long)ptr - sizeof(dl)))		/* convert pure memory pointer to strcut dl pointer */

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