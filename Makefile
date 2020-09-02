ppmcvt: ppmcvt.o pbm.o
	gcc $(DEBUG) -o ppmcvt ppmcvt.o pbm.o

ppmcvt.o: pbm.h ppmcvt.c
	gcc $(DEBUG) -c ppmcvt.c

pbm.o: pbm.h pbm.c
	gcc $(DEBUG) -c pbm.c

clean:
	rm ppmcvt ppmcvt.o pbm.o
