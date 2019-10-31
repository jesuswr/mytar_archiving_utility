OPTS=-ansi -Wpedantic

mytar: tar.o utilities.o mytar.o
	gcc $(OPTS)  -o mytar tar.o utilities.o mytar.o

mytar.o: mytar.c tar.h utilities.h
	gcc $(OPTS) -c mytar.c 

tar.o: tar.c tar.h utilities.h
	gcc $(OPTS) -c tar.c

utilities.o: utilities.c utilities.h
	gcc $(OPTS) -c utilities.c

clean:
	if find *.o; then rm *.o; fi
	if find mytar; then rm mytar; fi
