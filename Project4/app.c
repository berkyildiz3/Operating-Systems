#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "simplefs.h"

#define DISKNAME "vdisk1.bin"

int main()
{
    int ret;
    int fd1, fd2, fd;
    int i;
    char buffer[1024];
    char buffer2[8] = {50, 50, 50, 50, 50, 50, 50, 50};
    int size;
    char c;

    printf ("started\n");

    // ****************************************************
    // if this is the first running of app, we can
    // create a virtual disk and format it as below
    ret  = create_vdisk (DISKNAME, 24); // size = 16 MB
   if (ret != 0) {
	printf ("there was an error while creating disk\n");
	exit(1);
    }

    ret = sfs_format (DISKNAME);
    if (ret != 0) {
	printf ("there was an error in format\n");
	exit(1);
    }
    // ****************************************************

    ret = sfs_mount (DISKNAME);
    if (ret != 0) {
	printf ("could not mount \n");
	exit (1);
    }

    printf ("creating files\n");
    sfs_create ("file1.bin");
    sfs_create ("file2.bin");
    sfs_create ("file3.bin");
    sfs_create ("file4.bin");
    sfs_create ("file5.bin");



    fd1 = sfs_open ("file1.bin", 1);
    fd2 = sfs_open ("file2.bin", 1);
    int fd10=sfs_open("file4.bin", 1);


    for (i = 0; i < 10000; ++i) {
	buffer[0] =   (char) 65;
    sfs_append (fd1, (void *) buffer, 1);
    }


    for (i = 0; i < 10000; ++i) {
	buffer[0] = (char) 70;
	buffer[1] = (char) 71;
	buffer[2] = (char) 72;
	buffer[3] = (char) 73;
	sfs_append(fd2, (void *) buffer, 4);
    }

    for (i = 0; i < 10000; ++i) {
	buffer[0] = (char) 70;
	buffer[1] = (char) 71;
	buffer[2] = (char) 72;
	buffer[3] = (char) 73;
	sfs_append(fd10, (void *) buffer, 4);
    }

    printf("D = %d bytes\n", sfs_getsize(fd10));

    sfs_close(fd1);
    sfs_close(fd2);

    fd = sfs_open("file3.bin", 1);
    for (i = 0; i < 10000; ++i) {
	memcpy (buffer, buffer2, 8);
	sfs_append(fd, (void *) buffer, 8);

    }
    sfs_close (fd);

    fd = sfs_open("file3.bin", 1);

    //clock_t start_time = clock();
    for (i = 0; i < 10000; ++i) {
	 sfs_read (fd, (void *) buffer, 1);
	 c = (char) buffer[0];
    }


    //clock_t end_time = clock();
    sfs_close (fd);


    //sfs_delete("file3.bin");

	printf("A = %d bytes\n", sfs_getsize(fd2));


	printf("B = %d bytes\n", sfs_getsize(fd1));


	printf("C = %d bytes\n", sfs_getsize(fd));



    //printf("Program executed in %zd milliseconds.\n", end_time - start_time);


    sfs_umount();
	//printf("reached end!!\n");

}
