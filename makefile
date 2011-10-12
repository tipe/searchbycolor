all: chunks

chunks: chunks.cpp
	g++ -c chunks.cpp -Wall
	g++ chunks.o -o chunks

clean:
	rm -F *.o chunks

.PHONY: all clean
