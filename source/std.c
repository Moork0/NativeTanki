#include "../header/std.h"

static dl *head = NULL;

void *_sbrk(int size){
    void *program_break = (void *)syscall(SYS_brk, 0);
    if(!size){
        return program_break;
    }
    int inc = syscall(SYS_brk, program_break+size);
    if(inc == -1){
        return (void *)-1;
    }
    return program_break;
}

void add(dl* block){
  dl *curr = head;
  if(curr == NULL){
    curr = block;
    curr->next = NULL;
    curr->prev = curr;
    head = curr;
    return;
  }

  if(block <= curr){
    dl *temp = curr;
    curr = block;
    curr->next = temp;
    curr->prev = temp->prev;
    curr->next->prev = curr; // temp->prev = curr;
    head = curr;
    return;
  }

  while (curr->next != NULL)
  {
    if(block <= curr->next){
      dl *temp = curr->next;
      curr->next = block;
      curr->next->next = temp;
      curr->next->next->prev = curr->next; // temp->prev = curr->next;
      curr->next->prev = curr;
      //*head = curr;
      return;
    }
    curr = curr->next;
  }
  //printf("%d\n", curr->size);
  curr->next = block;
  curr->next->next = NULL;
  curr->next->prev = curr;
  head->prev = curr->next;
}

void erase(dl *block){
  dl *curr = head;
  if(block == NULL || head == NULL){
    return;
  }
  if(block == head){
    curr->next->prev = curr->prev;
    head = curr->next;
    //free(curr);
    return;
  }
  while(curr->next != NULL && block >= curr){
    if(curr == block){
      curr->next->prev = curr->prev;
      curr->prev->next = curr->next;
      //free(curr);
      return;
    }
    curr = curr->next;
  }
  if(curr == block){
    curr->prev->next = NULL;
    head->prev = curr->prev;
    //free(curr);
  }

}

void merge(dl *block){
  dl *curr = head, *prev = NULL, *base = NULL, *end = head->prev;
  void *program_break = _sbrk(0);
  while(curr != NULL && block >= curr){
    if(curr == block){
      break;
    }
    curr = curr->next;
  }
  prev = curr->prev;
  base = curr;
  if(prev->next != NULL && (BLOCK_MEM(prev)+prev->size) == curr){
    prev->size += sizeof(dl) + curr->size;
    prev->next = curr->next;
    if(curr->next){
        curr->next->prev = prev;
    }else{
        head->prev = prev;
        end = head->prev;
    }
    base = prev;
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

//  while(curr->next != NULL){
//    curr = curr->next;
//  }

  if(end->size >= MIN_MEM && (end + end->size + sizeof(dl)) == program_break){
    int brkresult = syscall(SYS_brk, end);
    if(brkresult == 0){
      erase(end);
    }
  }
}

dl *split(dl *block, unsigned int size){
  unsigned int alloc_size = size + sizeof(dl);
  dl *newptr = BLOCK_MEM(block) + block->size - alloc_size;
  newptr->size = size;
  block->size = block->size - alloc_size;
  return newptr;
}

void *_malloc(unsigned int size){
  dl *curr = head, *sbrkptr = NULL, *splittedptr = NULL;
  size = (size + 3) & ~3;
  unsigned int brk_size;
  if(size <= ALLOC_UNIT){
    brk_size = ALLOC_UNIT;
  }else{
    brk_size = ((size + sizeof(dl)) - 3) & ~3;
  }

  while (curr && curr->next != NULL){
    if(curr->size == size){
      erase(curr);
      return BLOCK_MEM(curr);
    }
    if(curr->size > size){
      splittedptr = split(curr, size);
      return BLOCK_MEM(splittedptr);
    }
    curr = curr->next;
  }

  sbrkptr = _sbrk((int)brk_size);
  if(sbrkptr == (void *)-1){
    return NULL;
  }
  sbrkptr->size = brk_size - sizeof(dl);
  if(sbrkptr->size > size){
    splittedptr = split(sbrkptr, size);
    add(sbrkptr);
    return BLOCK_MEM(splittedptr);
  }
  return BLOCK_MEM(sbrkptr);
}

void _free(void *mem){
  if(mem == NULL){
    return;
  }
  dl *block = MEM_BLOCK(mem);
  add(block);
  merge(block);
  mem = NULL;
}

void _clean(){
  if(head){
    syscall(SYS_brk, head);
    head = NULL;
  }
}

void *_memset(void* ptr, int value, size_t n){
  unsigned char *mem = ptr, castvalue = value;
  for(size_t i = 0; i < n; i++){
    mem[i] = castvalue;
  }
  return mem;
}

