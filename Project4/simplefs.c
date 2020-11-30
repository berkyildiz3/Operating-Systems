#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include "simplefs.h"
#define FATSIZE 1024*128


struct File
{
    int startingPoint;
    int size;
    int mode;
};

struct openFileTableEntry
{
    char fileName[32];
    int closeOpenS;
    struct File *file;
    int loc;
    int status;

};

struct directoryEntry
{
    char fileName[32];
    struct File file;
    int status;
};

struct openFileTableEntry openFileTable[56];
struct directoryEntry directory[56];
int FAT[1024*128];
int vdisk_fd;

// This function is simply used to a create a virtual disk
// (a simple Linux file including all zeros) of the specified size.
// You can call this function from an app to create a virtual disk.
// There are other ways of creating a virtual disk (a Linux file)
// of certain size.
// size = 2^m Bytes
int create_vdisk (char *vdiskname, int m)
{
    //printf("create disk called\n");
    char command[BLOCKSIZE];
    int size;
    int num = 1;
    int count;
    size  = num << m;
    count = size / BLOCKSIZE;
    printf ("%d %d", m, size);
    sprintf (command, "dd if=/dev/zero of=%s bs=%d count=%d",
             vdiskname, BLOCKSIZE, count);
    printf ("executing command = %s\n", command);
    system (command);
    return (0);
}

// read block k from disk (virtual disk) into buffer block.
// size of the block is BLOCKSIZE.
// space for block must be allocated outside of this function.
// block numbers start from 0 in the virtual disk.
int read_block (void *block, int k)
{
    int n;
    int offset;

    offset = k * BLOCKSIZE;
    lseek(vdisk_fd, (off_t) offset, SEEK_SET);
    n = read (vdisk_fd, block, BLOCKSIZE);
    if (n != BLOCKSIZE)
    {
        printf ("read error\n");
        return -1;
    }
    return (0);
}

// write block k into the virtual disk.
int write_block (void *block, int k)
{
    int n;
    int offset;

    offset = k * BLOCKSIZE;
    lseek(vdisk_fd, (off_t) offset, SEEK_SET);
    n = write (vdisk_fd, block, BLOCKSIZE);
    if (n != BLOCKSIZE)
    {
        printf ("write error\n");
        return (-1);
    }
    return 0;
}

int sfs_format (char *vdiskname)
{
    //printf("format called\n");
    int block[BLOCKSIZE];
    int FATlocation = 8;

    vdisk_fd = open (vdiskname, O_RDWR);
    if (vdisk_fd == -1)
    {
        printf ("format is not working!\n");
        exit(1);
    }

    int i = 0;
    while(i<FATSIZE)
    {
        memcpy(block, &FAT[i], BLOCKSIZE);
        write_block(block, FATlocation);
        FATlocation++;
        i = i + BLOCKSIZE;
    }

    for (int i=0; i<56; i++)
    {
        directory[i].file.startingPoint = 0;
        directory[i].status = 1;
    }

    memcpy(block, directory, 56*(sizeof(struct directoryEntry)));
    write_block(block,1);
    memcpy(block, directory, 56*(sizeof(struct directoryEntry)));
    write_block(block,2);
    memcpy(block, directory, 56*(sizeof(struct directoryEntry)));
    write_block(block,3);
    memcpy(block, directory, 56*(sizeof(struct directoryEntry)));
    write_block(block,4);
    memcpy(block, directory, 56*(sizeof(struct directoryEntry)));
    write_block(block,5);
    memcpy(block, directory, 56*(sizeof(struct directoryEntry)));
    write_block(block,6);
    memcpy(block, directory, 56*(sizeof(struct directoryEntry)));
    write_block(block,7);
    memcpy(block, directory, 56*(sizeof(struct directoryEntry)));

    fsync (vdisk_fd);
    close (vdisk_fd);

    return (0);
}

int sfs_mount (char *vdiskname)
{
    //printf("mount called\n");
    vdisk_fd = open (vdiskname, O_RDWR);
    int FATlocation = 8;
    int block[BLOCKSIZE];

    if (vdisk_fd == -1)
    {
        printf ("could not mounted\n");
        exit(1);
    }

    for (int i=0; i<56; i++)
    {
        openFileTable[i].status = 1;
    }


    for(int i=0; i<FATSIZE; i=i+BLOCKSIZE)
    {
        read_block(block, FATlocation);
        memcpy(&FAT[i], block, BLOCKSIZE);
        FATlocation++;
    }


    read_block(block, 1);
    memcpy(directory, block, 56*(sizeof(struct directoryEntry)));
    read_block(block, 2);
    memcpy(directory, block, 56*(sizeof(struct directoryEntry)));
    read_block(block, 3);
    memcpy(directory, block, 56*(sizeof(struct directoryEntry)));
    read_block(block, 4);
    memcpy(directory, block, 56*(sizeof(struct directoryEntry)));
    read_block(block, 5);
    memcpy(directory, block, 56*(sizeof(struct directoryEntry)));
    read_block(block, 6);
    memcpy(directory, block, 56*(sizeof(struct directoryEntry)));
    read_block(block, 7);
    memcpy(directory, block, 56*(sizeof(struct directoryEntry)));


    return 0;
}

int sfs_umount ()
{
    //printf("unmount called\n");
    int block[BLOCKSIZE];
    int FATlocation = 8;

    for(int i=0; i<FATSIZE; i=i+BLOCKSIZE)
    {
        memcpy(block, &FAT[i], BLOCKSIZE);
        write_block(block, FATlocation);
        FATlocation++;
    }

    memcpy(block, directory, 56*(sizeof(struct directoryEntry)));
    write_block(block, 1);
    memcpy(block, directory, 56*(sizeof(struct directoryEntry)));
    write_block(block,2);
    memcpy(block, directory, 56*(sizeof(struct directoryEntry)));
    write_block(block, 3);
    memcpy(block, directory, 56*(sizeof(struct directoryEntry)));
    write_block(block,4);
    memcpy(block, directory, 56*(sizeof(struct directoryEntry)));
    write_block(block, 5);
    memcpy(block, directory, 56*(sizeof(struct directoryEntry)));
    write_block(block,6);
    memcpy(block, directory, 56*(sizeof(struct directoryEntry)));
    write_block(block, 7);

    fsync (vdisk_fd);
    close (vdisk_fd);
    return (0);
}


int sfs_create(char *filename)
{
    //printf("create called\n");
    for(int i=0; i<56; i++)
    {
        if(directory[i].status == 1)
        {
            directory[i].file.size = 0;
            directory[i].status = 0;
            strcpy(directory[i].fileName, filename);
            int a = sfs_open(filename, 0);
            return a;
        }
    }
    return -1;
}

int sfs_open(char *file, int mode)
{
    //printf("open called\n");
    int index;

    for(int i = 0; i<56; i++)
    {
        if(strcmp(openFileTable[i].fileName,file)==0)
        {
            openFileTable[i].closeOpenS=1;
            openFileTable[i].loc = 0;
            openFileTable[i].file->mode = mode;
            return i;
        }
    }
    for(int i = 0; i<56 ; i++)
    {
        if(strcmp(directory[i].fileName,file)==0)
        {
            int flag=0;
            int k=0;

            while(k < 56 && flag==0)
            {
                if (openFileTable[k].status == 1)
                {
                    flag = 1;
                    openFileTable[i].closeOpenS = 1;
                    openFileTable[k].loc = 0;
                    openFileTable[k].status = 0;
                    openFileTable[k].file = &directory[i].file;
                    openFileTable[i].file->mode = mode;
                    strcpy(openFileTable[k].fileName, file);
                    index = k;
                }
                k++;
            }
        }
    }
    return index;
}

int sfs_close(int fd)
{
    //printf("close called\n");

        openFileTable[fd].closeOpenS=0;


    return -1;
}

int sfs_getsize (int  fd)
{
    //printf("size called\n");
    int fileSize;
    fileSize = openFileTable[fd].file->size;
    return fileSize;
}

int sfs_read(int fd, void *buf, int n)
{
    //printf("read called\n");
    int readPortion = 0;
    int isCompleted = 0;
    int count = 0;
    int block[BLOCKSIZE];
    int address;
    int displacement;
    int buff[n];


    if(openFileTable[fd].file->mode == 0)
    {

        address = openFileTable[fd].file->startingPoint;

        if(openFileTable[fd].loc > BLOCKSIZE)
            displacement = openFileTable[fd].loc%BLOCKSIZE;
        else
            displacement = openFileTable[fd].loc;

        read_block(block, address);

        while(isCompleted==0 && readPortion<n)
        {
            count++;
            displacement++;
            openFileTable[fd].loc++;
            readPortion++;
            memcpy(&buff[count],&block[displacement],1);

            if(displacement >= BLOCKSIZE)
            {
                if(FAT[address] != -1)
                {
                    displacement = 0;
                    address = FAT[address];
                    read_block(block, address);
                }
                else
                {
                    isCompleted = -1;
                }
            }
        }
        memcpy(buf, buff, n);
        return readPortion;
    }
    else
    {
        printf("Sorry, can't read '%s'. File is write only :(\n", openFileTable[fd].fileName);
        exit(-1);
    }
}

int sfs_append(int fd, void *buf, int n)
{
    //printf("append called\n");
    int buff[n];
    int block[BLOCKSIZE];
    int written;
    int address;
    int displacement;
    int count = 0;
    int isCompleted=0;
    written = 0;

    if(openFileTable[fd].file->mode == 1)
    {
        memcpy(buff, buf, n);

        if(openFileTable[fd].file->startingPoint==0)
        {
            int flag=0;
            int i=0;
            while(flag==0 && i < FATSIZE)
            {
                if (FAT[i] == 0)
                {
                    flag = 1;
                    openFileTable[fd].file->startingPoint = i;
                    FAT[i] = -1;
                    for(int k=0; k<n; k++)
                    {
                        memcpy(&block[k], &buff[k], 1);
                        openFileTable[fd].file->size++;
                    }
                    openFileTable[fd].loc = n;
                    write_block(block, i);
                    written = n;
                }
                i++;
            }
        }
        else
        {

            address = openFileTable[fd].file->startingPoint;


            if(openFileTable[fd].loc > BLOCKSIZE)
                displacement = openFileTable[fd].loc % BLOCKSIZE;
            else
                displacement = openFileTable[fd].loc;

            read_block(block, address);

            while(isCompleted==0 && written<n)
            {
                memcpy(&block[displacement],&buff[count], 1);
                openFileTable[fd].loc++;
                openFileTable[fd].file->size += sizeof(buff[count])  ;
                written++;
                count++;
                displacement++;


                if(displacement >= BLOCKSIZE)
                {
                    int flag2 = 0;
                    int i=0;
                    while(flag2==0 && i < FATSIZE)
                    {
                        if(FAT[i] == 0)
                        {
                            flag2 = 1;
                            displacement = 0;
                            write_block(block, address);
                            FAT[address] = i;
                            FAT[i] = 0;
                            address++;
                            read_block(block, address);
                        }
                        i++;
                    }
                    if(flag2 == 0)
                    {
                        isCompleted = -1;
                    }
                }
            }
            write_block(block, address);
        }
        return count;
    }
    else
    {
        printf("Sorry, can't append '%s'. File is read only :(\n", openFileTable[fd].fileName);
        exit(-1);
    }

}

int sfs_delete(char *filename)
{
    //printf("delete called\n");
    return -1;
}






