#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <string>
#include <stdexcept>

#include "PNGReader.h"

#define BLACK "black"
#define RED "red"
#define YELLOW "yellow"
#define GREEN "green"
#define BLUE "blue"
#define MAGENTA "magenta"
#define WHITE "white"
#define CYAN "cyan"

const int shades_count = 8;
const string shades[] = {"black", "red", "yellow", "green", "blue", "magenta", "white", "cyan"};


using namespace std;


void getImageColors(Image *image, int *result)
{
	int /*Y, U, V,*/ R, G, B;
	int my_shades[shades_count];
	for(int i = 0; i < shades_count; i++)
	{
		my_shades[i] = 0;
	}
	// int black = 0, red = 0, yellow = 0, green = 0;
	// int blue = 0, magenta = 0, white = 0, cyan = 0;

	for(int i = 0; i < image->getHeight(); i++)
	{
		for(int j = 0; j < image->getWidth(); j++)
		{
			R = image->getPixel(i,j).getRed();
			G = image->getPixel(i,j).getGreen();
			B = image->getPixel(i,j).getBlue();

			// Y = R * 0.299 + G * 0.587 + B * 0.114;
			// U = R * (-0.169) + G * (-0.332) + B * 0.500 + 128.;
			// V = R * 0.500 + G * (-0.419) + B * (-0.0813) + 128.;

			if(R <= 128 && G <= 128 && B <= 128) // black
			{
				my_shades[0]++;//black++;
			}
			else
			if(R > 128 && G <= 128 && B <= 128) // red
			{
				my_shades[1]++;//red++;
			}
			else
			if(R > 128 && G > 128 && B <= 128) // yellow
			{
				my_shades[2]++;//yellow++;
			}
			else
			if(R <= 128 && G > 128 && B <= 128) // green
			{
				my_shades[3]++;//green++;
			}
			else
			if(R > 128 && G <= 128 && B > 128) // blue
			{
				my_shades[4]++;//blue++;
			}
			else
			if(R > 128 && G <= 128 && B > 128) // magenta
			{
				my_shades[5]++;//magenta++;
			}
			else
			if(R > 128 && G > 128 && B > 128) // white
			{
				my_shades[6]++;//white++;
			}
			else
			{
				my_shades[7]++;//cyan++;
			}
		}
	}

	int size = image->getWidth()*image->getHeight();
	for(int i = 0; i < shades_count; i++)
	{
		result[i] = 100*my_shades[i]/size;
	}
	
}


int getPercentColor(Image *image, unsigned int r, unsigned int g, unsigned int b, unsigned int range)
{
	int count = 0;

	for(int i = 0; i < image->getHeight(); i++)
	{
		for(int j = 0; j < image->getWidth(); j++)
		{
			if( pow(image->getPixel(i,j).getRed() - r, 2) +
			    pow(image->getPixel(i,j).getGreen() - g, 2) +
			    pow(image->getPixel(i,j).getBlue() - b, 2) <= pow(range, 2))
			{
				count++;
			}
		}
	}

	return 100*count/(image->getHeight()*image->getWidth());
}


int main(int argc, char *argv[])
{
	
	if(argc < 2)
	{
		throw runtime_error("Please enter name of PNG file");
	}

	PNGReader reader(argv[1]);
	
	Image *image = reader.getImageStruct();

	//for(int i = 0; i < image->getHeight(); i++)
	{
		for(int j = 0; j < image->getWidth(); j++)
		{
			//<<j<<":("<<image->getPixel(60,j).getRed()<<","<<image->getPixel(60,j).getGreen()<<
			  //        ","<<image->getPixel(60,j).getBlue()<<","<<image->getPixel(60,j).getAlpha()<<")  ";
		}
	}

	int percent = getPercentColor(image, 255, 0, 0, 0);
	cout<<"percent = "<<percent<<endl;

	int my_shades[shades_count];
	getImageColors(image, my_shades);
	for(int i = 0; i < shades_count; i++)
	{
		cout<<shades[i]<<" "<<my_shades[i]<<"%"<<endl;
	}

	delete image;

	return 0;
}