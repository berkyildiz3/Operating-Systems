#ifndef MEMALLOC_H
#define MEMALLOC_H


int mem_init(void *, int, int);
void *mem_allocate(int);
void mem_free(void *, int);
void mem_print (void *);
#endif
