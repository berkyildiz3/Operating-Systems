all: libmemalloc.a app

#rule to obtain the library object file
memamlloc.o: memalloc.c
		gcc -Wall -g -c memalloc.c

#rule to obtain the library
libmemalloc.a: memalloc.o
		ar cr libmemalloc.a	memalloc.o
		ranlib libmemalloc.a

#rule to obtain the application object file
app.o: app.c
		gcc -Wall -g -c -I. app.c

#rule to obtain the application executable file
app: app.o libmemalloc.a
		gcc -Wall -g -o app app.o -I. -L. -lmemalloc

clean:

	rm -fr *.a *.o *~	app libmemalloc.a
