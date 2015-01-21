sourcefiles = ht.h ht.c any.h dictionary.h dictionary.c d_run.c makefile README

all: clean ht.o dictionary.o d_run

clean:
	/bin/rm -f d_run *~ *.o

ht.o: ht.h ht.c
	gcc -g -c ht.c

dictionary.o: dictionary.h dictionary.c
	gcc -g -c dictionary.c

d_run:
	gcc d_run.c -o d_run dictionary.o ht.o

make:
	make all
	
zip:
	/bin/rm -rf dictionarySource
	mkdir dictionarySource
	cp $(sourcefiles) dictionarySource/
	tar cvf .tar dictionarySource
	/bin/rm -f .tar.gz
	gzip .tar
	/bin/rm -rf dictionarySource
	
	

