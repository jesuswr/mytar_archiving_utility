OPTS=-ansi -Wpedantic

packtest: tar.o utilities.o packtest.o
	gcc $(OPTS)  -o packtest tar.o utilities.o packtest.o

packtest.o: packtest.c tar.h utilities.h
	gcc $(OPTS) -c packtest.c 

tar.o: tar.c tar.h utilities.h
	gcc $(OPTS) -c tar.c

utilities.o: utilities.c utilities.h
	gcc $(OPTS) -c utilities.c

clean:
	if find *.o; then rm *.o; fi
	if find packtest; then rm packtest; fi
