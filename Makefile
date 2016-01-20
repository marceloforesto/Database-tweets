all:
	gcc -o main main.c tweet.c index.c

clean: 
	rm -rf *.o main

run:
	./main

full: clean all run