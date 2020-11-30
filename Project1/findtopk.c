#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#define BUFFERSIZE 5000
#define BUFFERSIZE1 5000

void fileOperations(char *fileInputName, char *fileOutputName)
{

    int fd;
    char a[3];
    int numbers[1000];
    int num;

    fd = open(fileInputName, O_RDONLY);
    if(fd == -1)
    {
        printf(" thisopen() failed.\n");
        exit(1);
    }

    int fd1 = open(fileOutputName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd1 < 0)
    {
        printf("open() for output failed.\n");
        exit(1);
    }

    char* buffer = malloc(BUFFERSIZE+1);

    if (buffer == NULL)
    {
        printf("error ocured buffer");
    }

    size_t sz = read(fd, buffer, BUFFERSIZE);
    if (sz < 0)
    {
        printf("error ocured buffer2");
    }
    else
        buffer[sz] = '\0';

    for(int i=0; i < 4000; i = i+4)
    {

        if(buffer[i] == 10)
        {
            a[0]= buffer[i+1];
            a[1] = buffer[i+2];
            a[2] = buffer[i+3];

            num = atoi(a);
            //printf("String obtained on concatenation: %s from %d\n", a, i);
            //printf("num is %d\n", num);
        }
        else if(buffer[i+1] == 10)
        {
            a[0]= buffer[i];
            a[1] = buffer[i+2];
            a[2] = buffer[i+3];

            num = atoi(a);
            //printf("String obtained on concatenation: %s from %d\n", a, i);
            //printf("num is %d\n", num);
        }

        else if(buffer[i+2] == 10)
        {

            a[0]= buffer[i];
            a[1] = buffer[i+1];
            a[2] = buffer[i+3];

            num = atoi(a);
            //printf("String obtained on concatenation: %s from %d\n", a, i);
            //printf("num is %d\n", num);
        }

        else if(buffer[i+3] == 10)
        {
            a[0]= buffer[i];
            a[1] = buffer[i+1];
            a[2] = buffer[i+2];

            num = atoi(a);
            //printf("String obtained on concatenation: %s from %d\n", a, i);
            //printf("num is %d\n", num);
        }

        numbers[i/4] = num;
        //printf("buffer[%d] is %d \n", i, buffer[i] );
    }

    //avoid affects of junk data to sorting
    for(int i = 0; i < 1000; i++ )
    {
        if(numbers[i] == 987)
            numbers[i] = 0;
        //printf("element %d is %d\n ", i, numbers[i]);
    }
    int temp;
    for(int i=0; i<1000; i++)
    {
        for(int j=i+1; j<1000; j++)
        {
            if(numbers[i] < numbers[j])
            {
                temp     = numbers[i];
                numbers[i] = numbers[j];
                numbers[j] = temp;
            }
        }
    }

    char str[3];

    for(int i=0; i<800; i++)
    {
        sprintf(str, "%d", numbers[i]);
        write(fd1, str, 3);
        write(fd1,"\n", 1);
    }

    close (fd);
    free(buffer);
    close(fd1);
    printf("open close success\n");
}

int main()
{

    int numberOfInput;
    int number;
    int finalNumbers[1000];
    const char *inputNames[5];
    const char *outputNames[5];
    char b[3];

     clock_t start_time = clock();

    printf("Enter the number of input files (min 1, max 5): ");
    scanf("%d", &numberOfInput);

    if(numberOfInput==1)
    {
        inputNames[0] = "test1.txt";

        outputNames[0] = "out1.txt";
    }
    else if(numberOfInput==2)
    {
        inputNames[0] = "test1.txt";
        inputNames[1] = "test2.txt";

        outputNames[0] = "out1.txt";
        outputNames[1] = "out2.txt";
    }
    else if(numberOfInput==3)
    {
        inputNames[0] = "test1.txt";
        inputNames[1] = "test2.txt";
        inputNames[2] = "test3.txt";

        outputNames[0] = "out1.txt";
        outputNames[1] = "out2.txt";
        outputNames[2] = "out3.txt";
    }
    else if(numberOfInput==4)
    {
        inputNames[0] = "test1.txt";
        inputNames[1] = "test2.txt";
        inputNames[2] = "test3.txt";
        inputNames[3] = "test4.txt";

        outputNames[0] = "out1.txt";
        outputNames[1] = "out2.txt";
        outputNames[2] = "out3.txt";
        outputNames[3] = "out4.txt";
    }
    else
    {
        inputNames[0] = "test1.txt";
        inputNames[1] = "test2.txt";
        inputNames[2] = "test3.txt";
        inputNames[3] = "test4.txt";
        inputNames[4] = "test5.txt";

        outputNames[0] = "out1.txt";
        outputNames[1] = "out2.txt";
        outputNames[2] = "out3.txt";
        outputNames[3] = "out4.txt";
        outputNames[4] = "out5.txt";
    }

    for (int i = 0; i < numberOfInput; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            printf("\nError occured while creating child!\n");
            exit(0);
        }
        else if (pid == 0)
        {
            fileOperations(inputNames[i], outputNames[i]);
            exit(0);
        }
        else
            wait(NULL); //wait for all children process to exit
        if(numberOfInput==1)
        {
            int fd2 = open("out1.txt", O_RDONLY);
            if(fd2 == -1)
            {
                printf(" this 1 open() failed.\n");
                exit(1);
            }

            char* buffer1 = malloc(BUFFERSIZE1+1);

            if (buffer1 == NULL)
            {
                printf("error ocured buffer1");
            }

            size_t sz1 = read(fd2, buffer1, BUFFERSIZE1);
            if (sz1 < 0)
            {
                printf("error ocured buffer1");
            }
            else
                buffer1[sz1] = '\0';

            int fd3 = open("finalOutput.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            write(fd3, buffer1, 3000);
            close(fd2);
            close(fd3);
            free(buffer1);


        }

        else if(numberOfInput==2)
        {

            int fd2 = open("out1.txt", O_RDONLY);
            if(fd2 == -1)
            {
                printf(" this2  open() failed.\n");
                exit(1);
            }

            char* buffer1 = malloc(BUFFERSIZE1+1);

            if (buffer1 == NULL)
            {
                printf("error ocured buffer1");
            }

            size_t sz1 = read(fd2, buffer1, BUFFERSIZE1);
            if (sz1 < 0)
            {
                printf("error ocured buffer1");
            }
            else
                buffer1[sz1] = '\0';


            fd2 = open("out2.txt", O_RDONLY);
            if(fd2 == -1)
            {
                printf("this 3 open() failed.\n");
                exit(1);
            }

            char* buffer2 = malloc(BUFFERSIZE1+1);

            if (buffer2 == NULL)
            {
                printf("error ocured buffer1");
            }

            size_t sz2 = read(fd2, buffer2, BUFFERSIZE1);
            if (sz2 < 0)
            {
                printf("error ocured buffer1");
            }
            else
                buffer2[sz2] = '\0';

            int fd3 = open("outputFinal.txt", O_WRONLY | O_CREAT | O_TRUNC|O_RDONLY, 0644);
            write(fd3, buffer1, 3000);
            write(fd3, buffer2, 3000);



            close(fd2);
            close(fd3);

            free(buffer1);
            free(buffer2);

            /*      My unsuccessfull try for merging the outputs


                        int fd4 = open("finalOutput.txt", O_RDONLY | O_WRONLY);
                        if(fd4 == -1)
                        {
                            printf("open() failed.\n");
                            exit(1);
                        }

                        char* buffer3 = malloc(BUFFERSIZE1+1);

                        if (buffer3 == NULL)
                           { printf("error ocured buffer1");}

                           size_t sz3 = read(fd2, buffer3, BUFFERSIZE1);
                         if (sz3 < 0)
                           { printf("error ocured buffer1"); }
                         else
                           buffer3[sz3] = '\0';


                        for(int i=0; i < 4000; i = i+4)
                        {

                            if(buffer3[i] == 10){

                            b[0]= buffer3[i+1];
                            b[1] = buffer3[i+2];
                            b[2] = buffer3[i+3];

                            number = atoi(b);
                            //printf("String obtained on concatenation: %s from %d\n", a, i);
                            //printf("num is %d\n", num);
                            }
                            else if(buffer3[i+1] == 10){

                            b[0]= buffer3[i];
                            b[1] = buffer3[i+2];
                            b[2] = buffer3[i+3];

                            number = atoi(b);

                            //printf("String obtained on concatenation: %s from %d\n", a, i);
                            //printf("num is %d\n", num);
                            }

                            else if(buffer3[i+2] == 10){


                            b[0]= buffer3[i];
                            b[1] = buffer3[i+1];
                            b[2] = buffer3[i+3];

                            number = atoi(b);
                            //printf("String obtained on concatenation: %s from %d\n", a, i);
                            //printf("num is %d\n", num);
                            }

                            else if(buffer3[i+3] == 10){

                            b[0]= buffer3[i];
                            b[1] = buffer3[i+1];
                            b[2] = buffer3[i+2];

                            number = atoi(b);
                            //printf("String obtained on concatenation: %s from %d\n", a, i);
                            //printf("num is %d\n", num);
                            }

                            finalNumbers[i/4] = number;

                            //printf("buffer[%d] is %d \n", i, buffer[i] );
                        }

                        //avoid affects of junk data to sorting
                        for(int i = 0; i < 1000; i++ ){

                        if(finalNumbers[i] == 987) finalNumbers[i] = 0;

                        }
                        int temp;
                         for(int i=0; i<800; i++)
                            {
                                for(int j=i+1; j<800; j++)
                                {
                                    if(finalNumbers[i] < finalNumbers[j])
                                    {
                                        temp     = finalNumbers[i];
                                        finalNumbers[i] = finalNumbers[j];
                                        finalNumbers[j] = temp;
                                    }}}

                        char str[3];


                        for(int i=0; i<800; i++){

                        sprintf(str, "%d", finalNumbers[i]);
                        write(fd4, str, 3);
                        write(fd4,"\n", 1);

                        }


                        close(fd2);
                        close(fd3);
                        close(fd4);
                        free(buffer1);
                        free(buffer2);
                        free(buffer3);*/


        }

        else if(numberOfInput==3)
        {

            int fd2 = open("out1.txt", O_RDONLY);
            if(fd2 == -1)
            {
                printf("open() failed.\n");
                exit(1);
            }

            char* buffer1 = malloc(BUFFERSIZE1+1);

            if (buffer1 == NULL)
            {
                printf("error ocured buffer1");
            }

            size_t sz1 = read(fd2, buffer1, BUFFERSIZE1);
            if (sz1 < 0)
            {
                printf("error ocured buffer1");
            }
            else
                buffer1[sz1] = '\0';


            fd2 = open("out2.txt", O_RDONLY);
            if(fd2 == -1)
            {
                printf("open() failed.\n");
                exit(1);
            }

            char* buffer2 = malloc(BUFFERSIZE1+1);

            if (buffer2 == NULL)
            {
                printf("error ocured buffer1");
            }

            size_t sz2 = read(fd2, buffer2, BUFFERSIZE1);
            if (sz2 < 0)
            {
                printf("error ocured buffer1");
            }
            else
                buffer2[sz2] = '\0';

            fd2 = open("out3.txt", O_RDONLY);
            if(fd2 == -1)
            {
                printf("open() failed.\n");
                exit(1);
            }

            char* buffer3 = malloc(BUFFERSIZE1+1);

            if (buffer3 == NULL)
            {
                printf("error ocured buffer1");
            }

            size_t sz3 = read(fd2, buffer3, BUFFERSIZE1);
            if (sz3 < 0)
            {
                printf("error ocured buffer1");
            }
            else
                buffer3[sz3] = '\0';


            int fd3 = open("outputFinal.txt", O_WRONLY | O_CREAT | O_TRUNC|O_RDONLY, 0644);
            write(fd3, buffer1, 3000);
            write(fd3, buffer2, 3000);
            write(fd3, buffer3, 3000);



            close(fd2);
            close(fd3);

            free(buffer1);
            free(buffer2);
            free(buffer3);



        }

        else if(numberOfInput==4)
        {

            int fd2 = open("out1.txt", O_RDONLY);
            if(fd2 == -1)
            {
                printf("open() failed.\n");
                exit(1);
            }

            char* buffer1 = malloc(BUFFERSIZE1+1);

            if (buffer1 == NULL)
            {
                printf("error ocured buffer1");
            }

            size_t sz1 = read(fd2, buffer1, BUFFERSIZE1);
            if (sz1 < 0)
            {
                printf("error ocured buffer1");
            }
            else
                buffer1[sz1] = '\0';


            fd2 = open("out2.txt", O_RDONLY);
            if(fd2 == -1)
            {
                printf("open() failed.\n");
                exit(1);
            }

            char* buffer2 = malloc(BUFFERSIZE1+1);

            if (buffer2 == NULL)
            {
                printf("error ocured buffer1");
            }

            size_t sz2 = read(fd2, buffer2, BUFFERSIZE1);
            if (sz2 < 0)
            {
                printf("error ocured buffer1");
            }
            else
                buffer2[sz2] = '\0';

            fd2 = open("out3.txt", O_RDONLY);
            if(fd2 == -1)
            {
                printf("open() failed.\n");
                exit(1);
            }

            char* buffer3 = malloc(BUFFERSIZE1+1);

            if (buffer3 == NULL)
            {
                printf("error ocured buffer1");
            }

            size_t sz3 = read(fd2, buffer3, BUFFERSIZE1);
            if (sz3 < 0)
            {
                printf("error ocured buffer1");
            }
            else
                buffer3[sz3] = '\0';

            fd2 = open("out4.txt", O_RDONLY);
            if(fd2 == -1)
            {
                printf("open() failed.\n");
                exit(1);
            }

            char* buffer4 = malloc(BUFFERSIZE1+1);

            if (buffer4 == NULL)
            {
                printf("error ocured buffer1");
            }

            size_t sz4 = read(fd2, buffer4, BUFFERSIZE1);
            if (sz4 < 0)
            {
                printf("error ocured buffer1");
            }
            else
                buffer4[sz4] = '\0';

            int fd3 = open("outputFinal.txt", O_WRONLY | O_CREAT | O_TRUNC|O_RDONLY, 0644);
            write(fd3, buffer1, 3000);
            write(fd3, buffer2, 3000);
            write(fd3, buffer3, 3000);
            write(fd3, buffer4, 3000);


            close(fd2);
            close(fd3);

            free(buffer1);
            free(buffer2);
            free(buffer3);
            free(buffer4);


        }
        else
        {

            int fd2 = open("out1.txt", O_RDONLY);
            if(fd2 == -1)
            {
                printf("open() failed.\n");
                exit(1);
            }

            char* buffer1 = malloc(BUFFERSIZE1+1);

            if (buffer1 == NULL)
            {
                printf("error ocured buffer1");
            }

            size_t sz1 = read(fd2, buffer1, BUFFERSIZE1);
            if (sz1 < 0)
            {
                printf("error ocured buffer1");
            }
            else
                buffer1[sz1] = '\0';


            fd2 = open("out2.txt", O_RDONLY);
            if(fd2 == -1)
            {
                printf("open() failed.\n");
                exit(1);
            }

            char* buffer2 = malloc(BUFFERSIZE1+1);

            if (buffer2 == NULL)
            {
                printf("error ocured buffer1");
            }

            size_t sz2 = read(fd2, buffer2, BUFFERSIZE1);
            if (sz2 < 0)
            {
                printf("error ocured buffer1");
            }
            else
                buffer2[sz2] = '\0';

            fd2 = open("out3.txt", O_RDONLY);
            if(fd2 == -1)
            {
                printf("open() failed.\n");
                exit(1);
            }

            char* buffer3 = malloc(BUFFERSIZE1+1);

            if (buffer3 == NULL)
            {
                printf("error ocured buffer1");
            }

            size_t sz3 = read(fd2, buffer3, BUFFERSIZE1);
            if (sz3 < 0)
            {
                printf("error ocured buffer1");
            }
            else
                buffer3[sz3] = '\0';

            fd2 = open("out4.txt", O_RDONLY);
            if(fd2 == -1)
            {
                printf("open() failed.\n");
                exit(1);
            }

            char* buffer4 = malloc(BUFFERSIZE1+1);

            if (buffer4 == NULL)
            {
                printf("error ocured buffer1");
            }

            size_t sz4 = read(fd2, buffer4, BUFFERSIZE1);
            if (sz4 < 0)
            {
                printf("error ocured buffer1");
            }
            else
                buffer4[sz4] = '\0';



            fd2 = open("out5.txt", O_RDONLY);
            if(fd2 == -1)
            {
                printf("open() failed.\n");
                exit(1);
            }

            char* buffer5 = malloc(BUFFERSIZE1+1);

            if (buffer5 == NULL)
            {
                printf("error ocured buffer1");
            }

            size_t sz5 = read(fd2, buffer5, BUFFERSIZE1);
            if (sz5 < 0)
            {
                printf("error ocured buffer1");
            }
            else
                buffer5[sz5] = '\0';

            int fd3 = open("outputFinal.txt", O_WRONLY | O_CREAT | O_TRUNC|O_RDONLY, 0644);
            write(fd3, buffer1, 3000);
            write(fd3, buffer2, 3000);
            write(fd3, buffer3, 3000);
            write(fd3, buffer4, 3000);
            write(fd3, buffer5, 3000);


            close(fd2);
            close(fd3);

            free(buffer1);
            free(buffer2);
            free(buffer3);
            free(buffer4);
            free(buffer5);
        }
    }

    clock_t end_time = clock();

    printf("Program executed in %zd milliseconds.", end_time - start_time);
    return 0;
}
