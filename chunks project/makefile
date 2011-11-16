all: chunks

chunks: chunks.o PNGReader.o MyDecompressor.o 
	g++ chunks.o PNGReader.o MyDecompressor.o  -lz -o chunks

chunks.o: chunks.cpp PNGReader.h 
	g++ -c chunks.cpp -Wall -o chunks.o

PNGReader.o: PNGReader.cpp PNGReader.h MyDecompressor.h 
	g++ -c PNGReader.cpp -Wall -o PNGReader.o

MyDecompressor.o: MyDecompressor.cpp MyDecompressor.h
	g++ -c -I"lib-1.2.5" MyDecompressor.cpp -Wall -o MyDecompressor.o


clean:
	rm -f *.o chunks

.PHONY: all clean
