all: chunks

chunks: chunks.o PNGReader.o BMPReader.o MyDecompressor.o ImageReader.o Image.o
	g++ chunks.o PNGReader.o BMPReader.o MyDecompressor.o ImageReader.o Image.o  -lz -o chunks

chunks.o: chunks.cpp PNGReader.h BMPReader.h
	g++ -c chunks.cpp -Wall -o chunks.o

ImageReader.o: ImageReader.h
	g++ -c ImageReader.cpp -Wall -o ImageReader.o

PNGReader.o: PNGReader.cpp PNGReader.h MyDecompressor.h ImageReader.h
	g++ -c PNGReader.cpp -Wall -o PNGReader.o

BMPReader.o: BMPReader.cpp BMPReader.h ImageReader.h
	g++ -c BMPReader.cpp -Wall -o BMPReader.o

MyDecompressor.o: MyDecompressor.cpp MyDecompressor.h
	g++ -c -I"lib-1.2.5" MyDecompressor.cpp -Wall -o MyDecompressor.o

Image.o: Image.cpp Image.h
	g++ -c Image.cpp -Wall -o Image.o


clean:
	rm -f *.o chunks

.PHONY: all clean
