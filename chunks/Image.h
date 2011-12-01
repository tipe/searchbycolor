#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdexcept>

using namespace std;


bool is_bigendian();
int getIntInRightOrder(char *buf);
unsigned int getUnsignedInt(char buf);
unsigned int getIntSum(unsigned int i1, unsigned int i2, unsigned int i3=0);
unsigned int readInt(ifstream &file);
unsigned int readByte(ifstream &file);
unsigned int readTwoBytes(ifstream &file);



class Pixel
{
private:
	unsigned int r;	
	unsigned int g;
	unsigned int b;

	unsigned int alpha;

public:

	Pixel(unsigned int r, unsigned int g, unsigned int b, unsigned int alpha);
	Pixel();
	void setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int alpha);
	unsigned int getRed();
	unsigned int getGreen();
	unsigned int getBlue();
	unsigned int getAlpha();
};



class Image
{
private:
	int width;
	int height;
	
	int color_type;
	
	Pixel **pixels;

public:	
	Image(int width, int height, int color_type);	
	~Image();

	int getWidth();
	int getHeight();
	void setPixel(int i, int j, unsigned int r, unsigned int g, unsigned int b, unsigned int alpha=0);
	Pixel getPixel(int i, int j);
};

#endif //IMAGE_H