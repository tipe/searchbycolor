#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <string>
#include <stdexcept>

#include "PNGReader.h"
#include "BMPReader.h"

using namespace std;


void getImageColors(Image *image, int divider, float ***result)
{
	int R, G, B;

	result = new float**[divider];
	for(int i = 0; i < divider; i++)
	{
		result[i] = new float*[divider];
	}
	for(int i = 0; i < divider; i++)
	{
		for(int j = 0; j < divider; j++)
		{
			result[i][j] = new float[divider];
		}
	}


	for(int i = 0; i < divider; i++)
	{
		for(int j = 0; j < divider; j++)
		{
			for(int k = 0; k < divider; k++)
			{
				result[i][j][k] = 0;
			}
		}		
	}

	
	int border = 256/divider;

	for(int i = 0; i < image->getHeight(); i++)
	{
		for(int j = 0; j < image->getWidth(); j++)
		{
			R = image->getPixel(i,j).getRed();
			G = image->getPixel(i,j).getGreen();
			B = image->getPixel(i,j).getBlue();

			for(int i = 0; i < divider; i++) // red
			{
				for(int j = 0; j < divider; j++) // green
				{
					for(int k = 0; k < divider; k++) // blue
					{
						if(R >= i*border && R < (i+1)*border &&
						   G >= j*border && G < (j+1)*border &&
						   B >= k*border && B < (k+1)*border)

						result[i][j][k]++;
					}
				}
			}
		}
	}

	int size = image->getWidth()*image->getHeight();

	for(int i = 0; i < divider; i++) // red
	{
		for(int j = 0; j < divider; j++) // green
		{
			for(int k = 0; k < divider; k++) // blue
			{
				result[i][j][k] = 100*result[i][j][k]/size;
			}
		}
	}

}


bool isColorBasic(Image *image, unsigned int r, unsigned int g, unsigned int b, unsigned int range, int &percent)
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

	percent = 100*count/(image->getHeight()*image->getWidth());

	if(percent >= 30)
	{
		return 1;
	}
	return 0;
}


int main(int argc, char *argv[])
{	
	if(argc < 2)
	{
		throw runtime_error("Please enter name of PNG file");
	}

	PNGReader reader(argv[1]);

	//BMPReader reader(argv[1]);
	
	Image *image = reader.getImageStruct();
	//cout<<"!!! img height = "<<image->getHeight()<<" img width = "<<image->getWidth()<<endl;

	// //for(int i = 0; i < image->getHeight(); i++)
	// {
		for(int j = 0; j < image->getWidth(); j++)
		{
			//cout<<j<<":("<<image->getPixel(0,j).getRed()<<","<<image->getPixel(0,j).getGreen()<<
			  //        ","<<image->getPixel(0,j).getBlue()<<","<<image->getPixel(0,j).getAlpha()<<")  ";
		}
	// }

	int percent = 0;
	bool res = isColorBasic(image, 255, 0, 0, 0, percent);
	cout<<"percent = "<<percent<<" is basic = "<<res<<endl;

	float ***result;
	int divider = 10;
	getImageColors(image, divider, result);

	for(int i = 0; i < divider; i++) // red
	{
		for(int j = 0; j < divider; j++) // green
		{
			for(int k = 0; k < divider; k++) // blue
			{
				//cout<<result[i][j][k]<<" ";
			}
		}
	}
	

	for(int i = 0; i < divider; i++) 
	{
		for(int j = 0; j < divider; j++) 
		{
			//delete [] result[i][j];????
		}
	}

	for(int i = 0; i < divider; i++) 
	{
		//delete[] result[i];????
	}

	//delete[] result;???



	//delete image;



	return 0;
}