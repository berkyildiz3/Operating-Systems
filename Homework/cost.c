#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{

	struct timeval time_begin;
	struct timeval time_end;

	/*Calculation for first getpid()*/
	gettimeofday(&time_begin, 0);
	pid_t process_id;
	process_id = getpid();

	gettimeofday(&time_end, 0);
	long elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for first getpid(): %ld",elapsed);
	printf(" ms\n");
	/*End of calculation for first getpid()*/
	
	/*Calculation for second getpid()*/
	gettimeofday(&time_begin, 0);
	pid_t process_id2;
	process_id2 = getpid();

	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for second getpid(): %ld",elapsed);
	printf(" ms\n");
	/*End of calculation for second getpid()*/

	/*Calculation for first open()*/
	int fd1;
	int fd2;
	int fd3;
	char buf[14];
	char buf2[72];
	char buf3[562];

	gettimeofday(&time_begin, 0);
	fd1 = open("myfile.txt", O_CREAT | O_WRONLY, 0600);
	
	if(fd1 == -1)
	{
		printf("open() failed.\n");
		exit(1);
	}
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for first open(): %ld",elapsed);
	printf(" ms\n");
	/*End of calculation for first open()*/
	
	/*Calculation for first write()*/
	gettimeofday(&time_begin, 0);
	write(fd1, "Hello World!\n", 13);
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for first write(): %ld",elapsed);
	printf(" ms\n");
	/*End of calculation for first write()*/
	

	/*Calculation for first close()*/
	gettimeofday(&time_begin, 0);
	close(fd1);
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for first close(): %ld",elapsed);
	printf(" ms\n");
	/*End of calculation for first close()*/
	
	/*Calculation for second open()*/
	gettimeofday(&time_begin, 0);

	fd1 = open("myfile.txt", O_RDONLY);	
	if(fd1 == -1)
	{
		printf("open() and read() failed.\n");
		exit(1);
	}
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for second open(): %ld",elapsed);
	printf(" ms\n");	
	/*End of calculation for second open()*/	

	/*Calculation for first read()*/
	gettimeofday(&time_begin, 0);
	
	read(fd1, buf, 13);
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for first read(): %ld",elapsed);
	printf(" ms\n");	
	/*End of calculation for first read()*/
	
	/*Calculation for second close()*/
	gettimeofday(&time_begin, 0);
	close(fd1);
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for second close(): %ld",elapsed);
	printf(" ms\n");
	/*End of calculation for second close()*/


	/*-----------End of file 1-------------*/ 


	/*Calculation for third open()*/
	

	gettimeofday(&time_begin, 0);
	fd2 = open("myfile2.txt", O_CREAT | O_WRONLY, 0600);
	
	if(fd2 == -1)
	{
		printf("open() failed.\n");
		exit(1);
	}
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for third open(): %ld",elapsed);
	printf(" ms\n");
	/*End of calculation for third open()*/
	
	/*Calculation for second write()*/
	gettimeofday(&time_begin, 0);
	write(fd2, "1234567890123456789012345678901234567890123456789012345678901234567890\n", 71);
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for second write(): %ld",elapsed);
	printf(" ms\n");
	/*End of calculation for second write()*/
	

	/*Calculation for third close()*/
	gettimeofday(&time_begin, 0);
	close(fd2);
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for third close(): %ld",elapsed);
	printf(" ms\n");
	/*End of calculation for third close()*/
	
	/*Calculation for fourth open()*/
	gettimeofday(&time_begin, 0);

	fd2 = open("myfile2.txt", O_RDONLY);	
	if(fd2 == -1)
	{
		printf("open() and read() failed.\n");
		exit(1);
	}
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for fourth open(): %ld",elapsed);
	printf(" ms\n");	
	/*End of calculation for fourth open()*/	

	/*Calculation for second read()*/
	gettimeofday(&time_begin, 0);
	
	read(fd2, buf2, 71);
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for second read(): %ld",elapsed);
	printf(" ms\n");	
	/*End of calculation for second read()*/
	
	/*Calculation for fourth close()*/
	gettimeofday(&time_begin, 0);
	close(fd2);
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for fourth close(): %ld",elapsed);
	printf(" ms\n");
	/*End of calculation for fourth close()*/


	
	/*----------End of 2----------------*/



	/*Calculation for fifth open()*/
	

	gettimeofday(&time_begin, 0);
	fd3 = open("myfile3.txt", O_CREAT | O_WRONLY, 0600);
	
	if(fd3 == -1)
	{
		printf("open() failed.\n");
		exit(1);
	}
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for fifth open(): %ld",elapsed);
	printf(" ms\n");
	/*End of calculation for fifth open()*/
	
	/*Calculation for third write()*/
	gettimeofday(&time_begin, 0);
	write(fd3, "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\n", 561);
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for third write(): %ld",elapsed);
	printf(" ms\n");
	/*End of calculation for third write()*/
	

	/*Calculation for fifth close()*/
	gettimeofday(&time_begin, 0);
	close(fd3);
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for fifth close(): %ld",elapsed);
	printf(" ms\n");
	/*End of calculation for fifth close()*/
	
	/*Calculation for sixth open()*/
	gettimeofday(&time_begin, 0);

	fd3 = open("myfile3.txt", O_RDONLY);	
	if(fd3 == -1)
	{
		printf("open() and read() failed.\n");
		exit(1);
	}
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for sixth open(): %ld",elapsed);
	printf(" ms\n");	
	/*End of calculation for sixth open()*/	

	/*Calculation for third read()*/
	gettimeofday(&time_begin, 0);
	
	read(fd3, buf3, 561);
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for third read(): %ld",elapsed);
	printf(" ms\n");	
	/*End of calculation for third read()*/
	
	/*Calculation for sixth close()*/
	gettimeofday(&time_begin, 0);
	close(fd3);
	
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for sixth close(): %ld",elapsed);
	printf(" ms\n");
	/*End of calculation for sixth close()*/

	/*----------------End of file 3------------------ /

	/*Calculation for first mkdir()*/
	gettimeofday(&time_begin, 0);

	mkdir("newFolder", 0777);
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for first mkdir(): %ld",elapsed);
	printf(" ms\n");
	/*End of calculation for first mkdir()*/

	/*Calculation for second mkdir()*/
	gettimeofday(&time_begin, 0);

	mkdir("newFolder2", 0777);
	gettimeofday(&time_end, 0);
	elapsed = time_end.tv_usec - time_begin.tv_usec;

	printf("Time elapsed for second mkdir(): %ld",elapsed);
	printf(" ms\n");
	/*End of calculation for second mkdir()*/
	

return 0;



}

