#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <string>
#include <stdexcept>

#include "PNGReader.h"



using namespace std;



int getPercentColor(Image *image, unsigned int r, unsigned int g, unsigned int b, unsigned int range)
{
	int count = 0;

	for(int i = 0; i < image->height; i++)
	{
		for(int j = 0; j < image->width; j++)
		{
			if( pow(image->getPixel(i,j).getR() - r, 2) +
			    pow(image->getPixel(i,j).getG() - g, 2) +
			    pow(image->getPixel(i,j).getB() - b, 2) <= pow(range, 2))
			{
				count++;
			}
		}
	}

	return 100*count/(image->height*image->width);
}


int main(int argc, char *argv[])
{
	
	if(argc < 2)
	{
		throw runtime_error("Please enter name of PNG file");
	}

	PNGReader reader(argv[1]);
	
	Image *image = reader.getImageStruct();

	int percent = getPercentColor(image, 255, 0, 0, 0);
	cout<<"percent = "<<percent<<endl;


	return 0;
}