#include "Image.h"


Pixel::Pixel(unsigned int R, unsigned int G, unsigned int B, unsigned int A)
{
	this->R = R;
	this->G = G;
	this->B = B;
	this->A = A;
}

Pixel::Pixel()
{
	Pixel(0,0,0,0);
}

void Pixel::setColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A)
{
	this->R = R;
	this->G = G;
	this->B = B;
	this->A = A;
}

unsigned int Pixel::getRed()
{
	return R;
}

unsigned int Pixel::getGreen()
{
	return G;
}

unsigned int Pixel::getBlue()
{
	return B;
}

unsigned int Pixel::getAlpha()
{
	return A;
}


Image::Image(int width, int height)
{
	this->width = width;
	this->height = height;

	pixels = new Pixel*[height];
	for(int i = 0; i < height; i++)
	{
		pixels[i] = new Pixel[width];
	}
}

Image::~Image()
{
	if(pixels != NULL)
	{
		for(int i = 0; i < height; i++)
		{
			delete[] pixels[i];
		}
		delete[] pixels;
	}
}

int Image::getWidth()
{
	return width;
}

int Image::getHeight()
{
	return height;
}

void Image::setPixel(int i, int j, unsigned int R, unsigned int G, unsigned int B, unsigned int A)
{
	pixels[i][j].setColor(R, G, B, A);	
}

Pixel Image::getPixel(int i, int j)
{
	return pixels[i][j];
}



