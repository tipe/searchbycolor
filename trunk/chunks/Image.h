#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdexcept>
#include "Image.h"

using namespace std;

class Pixel
{
private:
	unsigned int R;	
	unsigned int G;
	unsigned int B;

	unsigned int A;

public:

	Pixel(unsigned int R, unsigned int G, unsigned int B, unsigned int A=0);
	Pixel();
	void setColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A=0);
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
	
	Pixel **pixels;

public:	
	Image(int width, int height);	
	~Image();

	int getWidth();
	int getHeight();
	void setPixel(int i, int j, unsigned int R, unsigned int G, unsigned int B, unsigned int A=0);
	Pixel getPixel(int i, int j);
};

#endif //IMAGE_H