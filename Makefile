ALL: secuencial omp thread
.PHONY= clean
CFLAGS= -g -fopenmp -lpthread

clean:
	@echo 'Porque Kenny!'
	\rm -rf secuencial omp thread

secuencial: secuencial.c
	gcc $(CFLAGS) secuencial.c -lm -o secuencial
	
omp: omparalelo.c
	gcc $(CFLAGS) omparalelo.c -lm -o omp

thread: threadparalelo.c
	gcc $(CFLAGS) threadparalelo.c -lm -o thread
