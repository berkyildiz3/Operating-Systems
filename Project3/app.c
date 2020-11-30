#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "memalloc.h"
#include <time.h>


int main(int argc, char *argv[])
{
    void *chunkptr;
    void *endptr;
    char *charptr;
    int ret;
    int i;
    int size;
    void *x1, *x2, *x3, *x4, *x5, *x6, *x7;
    // object pointers
    //clock_t start_time = clock();
    if (argc != 2)
    {
        printf("usage: app <size in KB>\n");
        exit(1);
    }
    size = atoi(argv[1]); // unit is in KB

    // allocate a chunk
    chunkptr = sbrk(0); // end of data segment
    sbrk(size * 1024); // extend data segment by indicated amount (bytes)
    endptr = sbrk(0);
    // new end of data segment
    printf("chunkstart=%lu, chunkend=%lu, chunksize=%lu bytes\n",
           (unsigned long)chunkptr,
           (unsigned long)endptr, (unsigned long)(endptr - chunkptr));
    //test the chunk
    //printf("---starting testing chunk\n");
    charptr = (char *)chunkptr;
    for (i = 0; i < size; ++i)
        charptr[i] = 0;
    //printf("---chunk test ended - success\n");
    printf("\n");

    ret = mem_init(chunkptr, size, 0);
    if (ret == -1)
    {
        printf("could not initialize \n");
        exit(1);
    }
    // below we allocate and deallocate memory dynamically
    x1 = mem_allocate(500);
    x1 = mem_allocate(500);
    x1 = mem_allocate(500);
    
    //x5 = mem_allocate(4096*1024);

    
    //mem_free(x5,4096*1024);



   

    mem_print(chunkptr);
    //clock_t end_time = clock();
    //printf("Program executed in %zd milliseconds.\n", end_time - start_time);
    return 0;
}
