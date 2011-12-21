#include <stdlib.h>
#include <iostream>

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
	//std::cerr<<"R = "<<R<<" ";
	pixels[i][j].setColor(R, G, B, A);	
}

Pixel Image::getPixel(int i, int j)
{
	return pixels[i][j];
}

float Image::getPercentColor(unsigned int r, unsigned int g, unsigned int b, unsigned int range)
{
	int count = 0;
	
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if( pow(abs(pixels[i][j].getRed() - r), 2) +
			    pow(abs(pixels[i][j].getGreen() - g), 2) +
			    pow(abs(pixels[i][j].getBlue() - b), 2) <= pow(range, 2))
			{
				count++;				
			}
		}
	}

	return 100*(float)count/(float)(height*width);
}

void Image::getDistribution(Distribution *distribution)
{
	int border = 256/distribution->getDivider();
	int img_size = height*width;

	//std::cerr<<"size = "<<img_size<<" height = "<<height<<" width"<<width<<std::endl;

	int flag; flag =0; if(img_size == 16384) flag = 1;

	for(int i = 0; i < height; i++)
	{
		//std::cerr<<"i = "<<i<<std::endl;
		for(int j = 0; j < width; j++)
		{
			if(j >= 175)
			{
				// std::cerr<<"j = "<<j<<std::endl;
				// std::cerr<<"getPixel(i,j).getRed() = "<<getPixel(i,j).getRed()<<std::endl;
				// std::cerr<<"getPixel(i,j).getG() = "<<getPixel(i,j).getGreen()<<std::endl;
				// std::cerr<<"getPixel(i,j).getB() = "<<getPixel(i,j).getBlue()<<std::endl;
				// std::cerr<<"getPixel(i,j).getA() = "<<getPixel(i,j).getAlpha()<<std::endl;
			}
			int ii = pixels[i][j].getRed()/border;
			int jj = pixels[i][j].getGreen()/border;
			int kk = pixels[i][j].getBlue()/border;

			//std::cout<<" ii = "<<ii<<" jj = "<<jj<<" kk = "<<kk<<std::endl;;

			distribution->addValue(ii, jj, kk, 100.0/img_size);

			//result[ii][jj][kk]++;
					
		}
	}	
}

