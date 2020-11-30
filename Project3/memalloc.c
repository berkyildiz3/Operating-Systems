#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void *chunkptrInit; //only points to initial adress of chunk, no further purpose
int chunkSizeG; //total chunk size in bytes
int methodG; //first fit, best fit, worst fit
char *charptrG; //operational pointer, used for iteration etc.
char *charptrInit; // always return beginning value of chunk, no further purpose
int count = 0; //for test and print purpose
int remainingChunkSize; //keep track of total remaining size of the chunk
pthread_mutex_t mutex;

int mem_init (void *chunkpointer, int chunksize, int method)
{

    printf("------------------------------BURAYA GIRDI----------------------------------------\n");
    chunkptrInit = chunkpointer;
    chunkSizeG = chunksize*1024; //assuming type of objectsize is byte
    remainingChunkSize = chunksize*1024;
    methodG = method;
    charptrInit = (char *)chunkptrInit;
    return (0);
}

void *mem_allocate (int objectsize)
{
	printf("------------------------------BURAYA GIRDI----------------------------------------\n");
    pthread_mutex_lock(&mutex);
    //printf("alloc called\n");
    charptrG = (char *)chunkptrInit;
    char* beginningBlock = (char *)chunkptrInit;
    int count1=0;
    int count3=0;
    int maxBlockSize=0;

    if(remainingChunkSize > objectsize)
    {
        if(methodG==0) //IMPLEMENTATION OF FIRST FIT
        {
            //check if the chunk is empty
            for(int i=0; i < chunkSizeG; i++)
            {
                if(charptrG[i] == 0)
                    count1++;
            }
            if(count1 == chunkSizeG)
            {
                memset(beginningBlock, 1, objectsize);
                remainingChunkSize = remainingChunkSize - objectsize;
                printf("REMAINING SIZE AFTER EMPTY CHECK: %d\n", remainingChunkSize);
                return beginningBlock;
            }//end of check for emptyness

            //if head of chunk is empty but chunk is not empty
            else if(charptrInit[0]==0)
            {
                int i = 0;
                while(charptrG[i] == 0 && i < chunkSizeG)
                {
                    count3++;
                    i++;
                }
                if(count3 >= objectsize)
                {
                    memset(beginningBlock, 1, objectsize);
                    remainingChunkSize = remainingChunkSize - objectsize;
                    printf("COUNT3: %d\n", count3);
                    return beginningBlock;
                }
            }
            else
            {
		
                int count2=0;
                int blockSize=0;
                for(int i=0; i < chunkSizeG; i++)
                {
                    if(charptrG[0] == 0)
                    {
                        blockSize++;
                    }
                    if(charptrG[0] == 1 || count2 == chunkSizeG-1)
                    {
                        beginningBlock = charptrG-blockSize + 1;

                        if(blockSize >= objectsize)
                        {
                            memset(beginningBlock, 1, objectsize);
                            remainingChunkSize = remainingChunkSize - objectsize;
                            blockSize=0;
                            return beginningBlock;
                        }
                    }
                    charptrG++;
                    count2++;
                }
            }
        }
        else if (methodG==2) //IMPLEMENTATION OF WORST FIT
        {
            //check if the chunk is empty
            for(int i=0; i < chunkSizeG; i++)
            {
                if(charptrG[i] == 0)
                    count1++;
            }
            if(count1 == chunkSizeG)
            {
                memset(beginningBlock, 1, objectsize);
                remainingChunkSize = remainingChunkSize - objectsize;
                return beginningBlock;
            }//end of check for emptyness

            else
            {
                int count2=0;
                int blockSize=0;
                for(int i=0; i < chunkSizeG; i++)
                {
                    if(charptrG[0] == 0)
                    {
                        blockSize++;
                    }
                    if(charptrG[0] == 1 || count2 == chunkSizeG-1)
                    {
                        if(blockSize > maxBlockSize)
                        {
                            maxBlockSize = blockSize;
                            beginningBlock = charptrG-blockSize + 1;
                            blockSize = 0;
                        }
                    }
                    charptrG++;
                    count2++;
                }
                if(objectsize <= maxBlockSize)
                {
                    memset(beginningBlock, 1, objectsize);
                    remainingChunkSize = remainingChunkSize - objectsize;
                    return beginningBlock;
                }
            }
        }
        else //IMPLEMENTATION OF BEST FIT
        {
            //check if the chunk is empty
            for(int i=0; i < chunkSizeG; i++)
            {
                if(charptrG[i] == 0)
                    count1++;
            }
            if(count1 == chunkSizeG)
            {
                memset(beginningBlock, 1, objectsize);
                remainingChunkSize = remainingChunkSize - objectsize;
                return beginningBlock;
            }//end of check for emptyness
            else
            {
                int count2=0;
                int blockSize=0;
                int blockObjectDifference;
                int minBlockObjectDifference=9999999999999999;
                for(int i=0; i < chunkSizeG; i++)
                {
                    if(charptrG[0] == 0)
                    {
                        blockSize++;
                    }
                    if(charptrG[0] == 1 || count2 == chunkSizeG-1)
                    {
                        blockObjectDifference = blockSize - objectsize;
                    }
                    if(blockObjectDifference > 0 && blockObjectDifference < minBlockObjectDifference)
                    {
                        minBlockObjectDifference = blockObjectDifference;
                        beginningBlock = charptrG-blockSize +1;
                    }
                    charptrG++;
                    count2++;
                }
                memset(beginningBlock-1, 1, objectsize);
                remainingChunkSize = remainingChunkSize - objectsize;
                return beginningBlock;
            }
        }
    }
    else
    {
        printf("NOT ENOUGH SPACE FOR LAST ALLOCATION!\n");
    }
    return beginningBlock;
    pthread_mutex_unlock(&mutex);
}

void mem_free(void *objectptr, int size)
{
    pthread_mutex_lock(&mutex);
    //printf("free called\n");
    memset(objectptr, 0, size);
    remainingChunkSize = remainingChunkSize + size;
    return;
    pthread_mutex_unlock(&mutex);
}

void mem_print (void *chunkpointer)
{
printf("------------------------------BURAYA GIRDI----------------------------------------\n");
    int blockSizePrint=0;
    int i=0;

    if(charptrInit[0]==1)
    {
        while(i<chunkSizeG)
        {
            while(charptrInit[i]==1)
            {
                blockSizePrint++;
                i++;
                chunkptrInit++;
                if(charptrInit[i]==0 || i==chunkSizeG)
                    break;
            }
            printf("ALLOCATED -- Beginning Address: %lu, End Address: %lu, Size: %d byte(s)\n", chunkptrInit-blockSizePrint, (unsigned long)chunkptrInit, blockSizePrint);
            blockSizePrint=0;

            while(charptrInit[i]==0)
            {
                blockSizePrint++;
                i++;
                chunkptrInit++;
                if(charptrInit[i]==1 || i== chunkSizeG)
                    break;
            }
            printf("FREE      -- Beginning Address: %lu, End Address: %lu, Size: %d byte(s)\n", chunkptrInit-blockSizePrint, (unsigned long)chunkptrInit, blockSizePrint);
            blockSizePrint=0;
        }
    }
    else
    {
        while(i<chunkSizeG)
        {
            while(charptrInit[i]==0)
            {
                blockSizePrint++;
                i++;
                chunkptrInit++;
                if(charptrInit[i]==1 || i==chunkSizeG)
                    break;
            }
            printf("FREE      -- Beginning Address: %lu, End Address: %lu, Size: %d byte(s)\n", chunkptrInit-blockSizePrint, (unsigned long)chunkptrInit, blockSizePrint);
            blockSizePrint=0;

            while(charptrInit[i]==1)
            {
                blockSizePrint++;
                i++;
                chunkptrInit++;
                if(charptrInit[i]==0 || i==chunkSizeG)
                    break;
            }
            printf("ALLOCATED -- Beginning Address: %lu, End Address: %lu, Size: %d byte(s)\n", chunkptrInit-blockSizePrint, (unsigned long)chunkptrInit, blockSizePrint);
            blockSizePrint=0;
        }
    }
    printf("\n");
    printf("Remaining Total Size in Chunk: %d byte(s)\n", remainingChunkSize);
    return;
}
