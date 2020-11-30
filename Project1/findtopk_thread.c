
#include <pthread.h>
#include <stdio.h>

int s;

void *runner (void *param);

int main(int argc, char *argv[]){

pthread_t tid;
pthread_attr_t attr;

if (argc != 2) {

fprintf (stderr, “usage: a.out <value>\n”);
return -1;

}

if (atoi(argv[1]) < 0) {

fprintf (stderr, “%d must be >= 0\n”, atoi(argv[1]);

return -1;

}
pthread_attr_init (&attr);
pthread_create (&tid, &attr, runner, argv[1]);
pthread_join (tid, NULL);


}
