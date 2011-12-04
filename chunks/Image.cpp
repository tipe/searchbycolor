#include "Image.h"


Pixel::Pixel(unsigned int r, unsigned int g, unsigned int b, unsigned int alpha)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->alpha = alpha;
}

Pixel::Pixel()
{
	Pixel(0,0,0,0);
}

void Pixel::setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int alpha)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->alpha = alpha;
}

unsigned int Pixel::getRed()
{
	return r;
}

unsigned int Pixel::getGreen()
{
	return g;
}

unsigned int Pixel::getBlue()
{
	return b;
}

unsigned int Pixel::getAlpha()
{
	return alpha;
}


Image::Image(int width, int height, int color_type)
{
	this->width = width;
	this->height = height;

	pixels = new Pixel*[height];
	for(int i = 0; i < height; i++)
	{
		pixels[i] = new Pixel[width];
	}

	this->color_type = color_type;
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

void Image::setPixel(int i, int j, unsigned int r, unsigned int g, unsigned int b, unsigned int alpha)
{
	pixels[i][j].setColor(r, g, b, alpha);
}

Pixel Image::getPixel(int i, int j)
{
	return pixels[i][j];
}



