all: chunks

chunks: chunks.o PNGReader.o BMPReader.o MyDecompressor.o ImageReader.o Image.o Distribution.o ImagesComparison.o Graph.o
	g++ chunks.o PNGReader.o BMPReader.o MyDecompressor.o ImageReader.o Image.o Distribution.o ImagesComparison.o Graph.o -lz -o chunks

chunks.o: chunks.cpp PNGReader.h BMPReader.h ImagesComparison.h
	g++ -c chunks.cpp -Wall -o chunks.o

ImageReader.o: ImageReader.h
	g++ -c ImageReader.cpp -Wall -o ImageReader.o

PNGReader.o: PNGReader.cpp PNGReader.h MyDecompressor.h ImageReader.h
	g++ -c PNGReader.cpp -Wall -o PNGReader.o

BMPReader.o: BMPReader.cpp BMPReader.h ImageReader.h
	g++ -c BMPReader.cpp -Wall -o BMPReader.o

MyDecompressor.o: MyDecompressor.cpp MyDecompressor.h
	g++ -c -I"lib-1.2.5" MyDecompressor.cpp -Wall -o MyDecompressor.o

Image.o: Image.cpp Image.h Distribution.h
	g++ -c Image.cpp -Wall -o Image.o

Distribution.o: Distribution.cpp Distribution.h
	g++ -c Distribution.cpp -Wall -o Distribution.o

Graph.o: Graph.cpp Graph.h
	g++ -c Graph.cpp -Wall -o Graph.o

ImagesComparison.o: ImagesComparison.cpp ImagesComparison.h Graph.h Distribution.h
	g++ -c ImagesComparison.cpp -Wall -o ImagesComparison.o

clean:
	rm -f *.o chunks

.PHONY: all clean
