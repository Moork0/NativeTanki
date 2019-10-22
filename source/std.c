#include "../header/std.h"

/* A head for double linked list of Free memory chunks */
static dl *head = NULL;

/*
  sbrk function. view manual sbrk(2)

*/
void *_sbrk(int size){
    void *program_break = (void *)syscall(SYS_brk, 0);    /* Current memory break */
    
    /* return current break if size == 0*/
    if(!size){
        return program_break;
    }

    /* increase/decrease program memory break */
    int inc = syscall(SYS_brk, program_break+size);
    if(inc == -1){
        return (void *)-1;
    }
    return program_break;
}

/*
  Add block to "head" ordered by block address
*/
void add(dl* block){
  dl *curr = head;

  /* the block is first block */
  if(curr == NULL){
    curr = block;
    curr->next = NULL;
    curr->prev = curr;
    head = curr;
    return;
  }

  /* block is lower that the first block */
  if(block <= curr){
    dl *temp = curr;
    curr = block;
    curr->next = temp;
    curr->prev = temp->prev;
    curr->next->prev = curr; // temp->prev = curr;
    head = curr;
    return;
  }

  /* search for suitable place */
  while (curr->next != NULL)
  {
    if(block <= curr->next){
      dl *temp = curr->next;
      curr->next = block;
      curr->next->next = temp;
      curr->next->next->prev = curr->next; // temp->prev = curr->next;
      curr->next->prev = curr;
      return;
    }
    curr = curr->next;
  }

  /* block should be the last */
  curr->next = block;
  curr->next->next = NULL;
  curr->next->prev = curr;
  head->prev = curr->next;
}

/* erase the block from double linked list */
void erase(dl *block){
  dl *curr = head;
  if(block == NULL || head == NULL){
    return;
  }
  /* block is head. */
  if(block == head){
    curr->next->prev = curr->prev;
    head = curr->next;
    return;
  }
  while(curr->next != NULL && block >= curr){
    if(curr == block){
      curr->next->prev = curr->prev;
      curr->prev->next = curr->next;
      return;
    }
    curr = curr->next;
  }
  /* block is last block in our double linked list */
  if(curr == block){
    curr->prev->next = NULL;
    head->prev = curr->prev;
  }

}

/*
  search the block and check the integrity of
  memory chunks. merge memory blocks and check
  if the last block is corresponding to program break
  then release that block to OS  
*/
void merge(dl *block){
  dl *curr = head, *prev = NULL, *base = NULL, *end = head->prev;
  void *program_break = _sbrk(0); /* current break */
  /* Search for block */
  while(curr != NULL && block >= curr){
    if(curr == block){
      break;
    }
    curr = curr->next;
  }
  /*
    check if block not found, then exit the function
  */
  if(curr == NULL){
    return;
  }
  prev = curr->prev;
  base = curr; // this is a first block of the merging blocks.
  /* 
    Check if previous block is integratable with current block
     then merge that two blocks.
  */
  if((BLOCK_MEM(prev)+prev->size) == curr){
    prev->size += sizeof(dl) + curr->size;
    prev->next = curr->next;
    if(curr->next){
        curr->next->prev = prev;
    }else{
        head->prev = prev;
        end = head->prev;
    }
    base = prev; // prev should be first block of merging list
  }

  curr = curr->next;
  while ((BLOCK_MEM(base)+base->size) == curr){
    base->size += sizeof(dl) + curr->size;
    base->next = curr->next;
    if(curr->next){
        curr->next->prev = prev;
    }
    curr = curr->next;
  }
  /*
    check if last block is correspound to the program break. 
    check if last block is bigger than minimum amount of memory page size
    then release that block to OS
  */
  if(end->size >= MIN_MEM && (end + end->size + sizeof(dl)) == program_break){
    int brkresult = syscall(SYS_brk, end);
    if(brkresult == 0){
      erase(end);
    }
  }
}

/*
  split the block by size and return a new block with new size
*/
dl *split(dl *block, unsigned int size){
  unsigned int alloc_size = size + sizeof(dl);
  dl *newptr = BLOCK_MEM(block) + block->size - alloc_size;
  newptr->size = size;
  block->size = block->size - alloc_size;
  return newptr;
}

/*
  search in the list of free memory blocks and 
  return the suitable memory.
  if could not find suitable chunk, ask OS to give some space
*/
void *_malloc(unsigned int size){
  dl *curr = head, *sbrkptr = NULL, *splittedptr = NULL;
  unsigned int brk_size;
  /* make size a factor of 4 */
  size = (size + 3) & ~3;
  /*
    check if size > minimum amount of memory size for asking OS
  */
  if(size <= ALLOC_UNIT){
    brk_size = ALLOC_UNIT;
  }else{
    brk_size = ((size + sizeof(dl)) - 3) & ~3;
  }

  while (curr && curr->next != NULL){
    /* 
      suitable memory found. return that
    */
    if(curr->size == size){
      erase(curr);
      return BLOCK_MEM(curr);
    }
    /* 
      split the block size and return suitable memory chunk
    */    
    if(curr->size > size){
      splittedptr = split(curr, size);
      return BLOCK_MEM(splittedptr);
    }
    curr = curr->next;
  }
  /* 
    no block found. ask OS
  */
  sbrkptr = _sbrk((int)brk_size);
  if(sbrkptr == (void *)-1){
    return NULL;
  }
  sbrkptr->size = brk_size - sizeof(dl);
  /* 
    if allocated size > asked size, split the block and return new block.
  */
  if(sbrkptr->size > size){
    splittedptr = split(sbrkptr, size);
    add(sbrkptr);
    return BLOCK_MEM(splittedptr);
  }
  return BLOCK_MEM(sbrkptr);
}

/*
  bring back mem to the list of free memory blocks
*/
void _free(void *mem){
  if(mem == NULL){
    return;
  }
  dl *block = MEM_BLOCK(mem);
  add(block);
  merge(block);
  mem = NULL;
}

/*
  cleanup memory
*/
void _clean(){
  if(head){
    syscall(SYS_brk, head);
    head = NULL;
  }
}

/*
  implementation of c standard library of memset.
  set each byte of ptr to value.
*/
void *_memset(void* ptr, int value, size_t n){
  unsigned char *mem = ptr, castvalue = value;
  for(size_t i = 0; i < n; i++){
    mem[i] = castvalue;
  }
  return mem;
}

