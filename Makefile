ALL: proy
.PHONY= clean
CFLAGS= -g -fopenmp

clean:
	\rm -rf proy

proy: main.c
	gcc $(CFLAGS) main.c -o proy
