all:
	gcc -g -c parsing.c
	gcc -o main main.c parsing.o

run:
	./main
	dot -Tsvg -O graph.dot
	xdg-open graph.dot.svg
	xdg-open graph.dot.2.svg
	xdg-open graph.dot.3.svg
clean:
	rm *.o
