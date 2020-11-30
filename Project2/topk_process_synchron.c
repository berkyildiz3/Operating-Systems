#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <time.h>

const char *inputNames[10];
int N;
int K;
const char *name = "process_sync";


int* arraySort(int* intArray)
{
    int i,j, a;
    for (i = 0; i < K; ++i)
    {
        for (j = i + 1; j < K; ++j)
        {
            if (intArray[i] < intArray[j])
            {
                a = intArray[i];
                intArray[i] = intArray[j];
                intArray[j] = a;
            }
        }
    }
    return intArray;
}

void fileOperations(int k, int *numbers)
{
    FILE *fp;
    int number;
    int size=0;

    for(int i = 0; i < N; i++)
    {
        fp = fopen(inputNames[i], "r");

        while (fscanf(fp, " %d", &number)==1)
        {
            if(size < k)
            {
                numbers[size] = number;
                size++;
            }
            if(size >= k)
            {
                arraySort(numbers);
                if(number > numbers[k-1])
                {
                    numbers[k-1] = number;
                }
            }
        }
    }
    fclose(fp);
}

int main (int argc, char *argv[])
{


    int count;
    int k;
    int i=0;
    int shm_fd;
    int* shared_memory;
    int msize;

    sem_t mutex;
    //clock_t start_time = clock();
    if(sem_init(&mutex,1,1) < 0)
    {
        printf("semaphore initilization");
        exit(0);
    }

    printf ("This program was called with \"%s\".\n",argv[0]);
    printf ("argc is %d\n",argc);

    k = atoi(argv[1]);
    N = atoi(argv[2]);

    printf ("K=%d , N=%d\n",k,N);

    if (argc > 1)
    {
        for (count = 0; count < argc; count++)
        {
            printf("argv[%d] = %s\n", count, argv[count]);
        }
    }

    K = atoi(argv[1]);

    msize = (k+2)*sizeof(int);
    shm_fd = shm_open (name, O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG);

    if (shm_fd < 0)
    {
        fprintf(stderr,"Shared memory couldnt open");
        return -3;
    }

    printf("Shared memory created\n");

    ftruncate(shm_fd, msize);
    printf("ftruncate works\n");

    shared_memory = (int *) mmap(NULL, msize, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (shared_memory == NULL)
    {
        fprintf(stderr,"Error in mmap()");
        return -3;
    }

    printf("Everything is fine with shared memory! %d bytes allocated\n", msize);

    shared_memory[0] = k;

    for (i = 0; i < N; ++i)
    {
        inputNames[i] = argv[i+3];
        pid_t pid = fork();

        if (pid < 0)
        {
            printf("\nError occured while creating child!\n");
            exit(0);
        }
        else if (pid == 0)
        {
            //while(sem_wait(&mutex)!=0){
            fileOperations(shared_memory[0],shared_memory+1);
            exit(0);
            //}
            //sem_post(&mutex);
        }
        else
        {
            wait(NULL);
            //sleep(1);
            for(int i = 0; i < k; i++)
            {
                printf("%d\n", shared_memory[i]);
            }
            shm_unlink(name);
        }
    }
    //clock_t end_time = clock();
    //printf("Program executed in %zd milliseconds.\n", end_time - start_time);

    return 0;
}
