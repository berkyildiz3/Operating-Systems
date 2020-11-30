all: topk_process_synchron topk_thread_synchron

topk_process_synchron: topk_process_synchron.c
	gcc -g -Wall -o topk_process_synchron topk_process_synchron.c -lrt -lpthread

topk_thread_synchron: topk_thread_synchron.c
	gcc -g -Wall -o topk_thread_synchron topk_thread_synchron.c -pthread

clean:
	rm -fr *~ *.o topk_process_synchron topk_thread_synchron
