all: a.out

a.out: main.c
	gcc main.c -lttc_c -lc

clean:
	rm -f main.o
	rm -f a.out
