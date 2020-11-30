#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>


int numberOfNodes=0;
struct node *root = NULL;
int sum=0;
const char *inputNames[10];
int N;
int K;
int fd1;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct node
{
    int value;
    struct node *leftChild, *rightChild;
};

struct node *newNode(int key)
{
    struct node *temp =  (struct node *)malloc(sizeof(struct node));
    temp->value = key;
    temp->leftChild = temp->rightChild = NULL;
    return temp;
}

struct node* minimumValueNode(struct node* node)
{
    struct node* temp = node;
    while (temp && temp->leftChild != NULL)
        temp = temp->leftChild;
    return temp;
}

struct node* insert(struct node* node, int value)
{
    if (node == NULL)
        return newNode(value);
    if (value < node->value)
        node->leftChild  = insert(node->leftChild, value);
    else if (value > node->value)
        node->rightChild = insert(node->rightChild, value);
    return node;
}

struct node* deleteNode(struct node* root, int value)
{
    if (root == NULL)
        return root;
    if (value < root->value)
    {
        root->leftChild = deleteNode(root->leftChild, value);
    }
    else if (value > root->value)
    {
        root->rightChild = deleteNode(root->rightChild, value);
    }
    else
    {
        if (root->leftChild == NULL)
        {
            struct node *temp = root->rightChild;
            free(root);
            return temp;
        }
        else if (root->rightChild == NULL)
        {
            struct node *temp = root->leftChild;
            free(root);
            return temp;
        }
        struct node *temp = minimumValueNode(root->rightChild);
        root->value = temp->value;
        root->rightChild = deleteNode(root->rightChild, temp->value);
    }
    return root;
}

int minValue(struct node* node)
{
    struct node *current = node;
    while (current->leftChild != NULL)
    {
        current = current->leftChild;
    }
    return(current->value);
}

void inorderTraverse(struct node *root)
{
    //char str[4];

    if (root != NULL && numberOfNodes < K-4)
    {
        inorderTraverse(root->rightChild);
        printf("%d\n", root->value);
        //sprintf(str, "%d", root->value);
        //write(fd1, str, 4);
        //write(fd1,"\n", 1);
        numberOfNodes++;
        inorderTraverse(root->leftChild);
    }
}

void *processFiles(void *arg)
{
    int k = *(int *) arg;
    int number;
    int size=0;
    int minimumValue;
    FILE *fp;

    for(int i = 0; i < N; i++)
    {
        pthread_mutex_lock(&mutex);
        fp = fopen(inputNames[i], "r");
        if(fp == NULL)
        {
            printf("Error opening file\n");
            exit(1);
        }

        while (fscanf(fp, " %d", &number)==1)
        {
            //directly add first k values to bst
            if(size < k && size >= 0)
            {
                if(root == NULL)
                {
                    root = insert(root, number);
                    //printf("NUMBER INSERTED  %d\n", number);
                    //printf("SIZE IS   %d\n", size);
                    size++;
                }
                else
                {
                    insert(root,number);
                    //printf("NUMBER INSERTED  %d\n", number);
                    //printf("SIZE IS   %d\n", size);
                    size++;
                }
            }
            //for the remaining, check one by one by comparing with minimum number of bst.
            //if current node is bigger than minimum, insert it to bst and delete the minimum one.
            if(size >= k)
            {
                minimumValue = minValue(root);
                //printf("minimum value is %d, %d\n", count, minimumValue);
                if(number >= minimumValue)
                {
                    root=deleteNode(root, minimumValue);
                    insert(root, number);
                    size--;
                }
                else
                {
                    number = NULL;
                }
            }
        }
        fclose(fp);
        pthread_mutex_unlock(&mutex);
    }
    //printf("SIZE %d\n", size);
    pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
    //clock_t start_time = clock();
    int count;
    int k;
    int i=0;

    printf ("This program was called with \"%s\".\n",argv[0]);
    printf ("argc is %d\n",argc);

    k = atoi(argv[1]);
    N = atoi(argv[2]);

    printf ("K=%d , N=%d\n",k,N);

    //2 eleman girersen argc 3 olur
    if (argc > 1)
    {
        for (count = 0; count < argc; count++)
        {
            printf("argv[%d] = %s\n", count, argv[count]);
        }
    }

    K = atoi(argv[1]);

    // create a thread array
    pthread_t *threads = malloc(sizeof(pthread_t) * N);

    for (i = 0; i < N; ++i)
    {
        inputNames[i] = argv[i+3];
        pthread_create(&threads[i], NULL, processFiles, &k);
    }

    for (i = 0; i < N; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    //int fd1 = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    //if (fd1 < 0)
    //{
     //   printf("open() for output failed.\n");
     //   exit(1);
   // }
    inorderTraverse(root);
    //close(fd1);
    //clock_t end_time = clock();
    //printf("Program executed in %zd milliseconds.\n", end_time - start_time);
    return 0;
}

