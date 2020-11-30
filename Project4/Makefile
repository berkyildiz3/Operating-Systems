all: libsimplefs.a app

#rule to obtain the library object file
simplefs.o: simplefs.c
		gcc -Wall -g -c simplefs.c

#rule to obtain the library
libsimplefs.a: simplefs.o
		ar cr libsimplefs.a	simplefs.o
		ranlib libsimplefs.a

#rule to obtain the application object file
app.o: app.c
		gcc -Wall -g -c -I. app.c

#rule to obtain the application executable file
app: app.o libsimplefs.a
		gcc -Wall -g -o app app.o -I. -L. -lsimplefs

clean:

	rm -fr *.a *.o *~	app libsimplefs.a
