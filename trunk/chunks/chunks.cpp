#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <string>
#include <stdexcept>
#include <dirent.h>


#include "PNGReader.h"
#include "BMPReader.h"

using namespace std;


float*** getThreeDimensionalMatrix(int size)
{
	float ***result;
	result = new float**[size];
	for(int i = 0; i < size; i++)
	{
		result[i] = new float*[size];
	}
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			result[i][j] = new float[size];
		}
	}

	return result;
}

void deleteThreeDimensionalMatrix(float ***result, int size)
{
	if(result != NULL)
	{
		for(int i = 0; i < size; i++) 
		{
			for(int j = 0; j < size; j++) 
			{
				delete [] result[i][j];
			}
		}

		for(int i = 0; i < size; i++)
		{
			delete[] result[i];
		}

		delete[] result;
	}	
}



void getImageColors(Image *image, int divider, float ***result)
{
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
	
	int border = 255/divider + 1;

	for(int i = 0; i < image->getHeight(); i++)
	{
		for(int j = 0; j < image->getWidth(); j++)
		{
			int ii = image->getPixel(i,j).getRed()/border;
			int jj = image->getPixel(i,j).getGreen()/border;
			int kk = image->getPixel(i,j).getBlue()/border;

			result[ii][jj][kk]++;
					
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


float isColorBasic(Image *image, unsigned int r, unsigned int g, unsigned int b, unsigned int range)
{
	int count = 0;
	
	for(int i = 0; i < image->getHeight(); i++)
	{
		for(int j = 0; j < image->getWidth(); j++)
		{
			if( pow(abs(image->getPixel(i,j).getRed() - r), 2) +
			    pow(abs(image->getPixel(i,j).getGreen() - g), 2) +
			    pow(abs(image->getPixel(i,j).getBlue() - b), 2) <= pow(range, 2))
			{
				count++;				
			}
		}
	}

	return 100*(float)count/(float)(image->getHeight()*image->getWidth());
}


int main(int argc, char *argv[])
{	
	if(argc < 2)
	{
		throw runtime_error("Please enter name of PNG file");
	}

	DIR *dir = opendir(argv[1]);

	if(dir == NULL)
	{
		throw runtime_error("Can not open dir");
	}

	struct dirent *entry;

	int red = 50;
	int green = 100;
	int blue = 250;
	int range = 100;

	while ((entry = readdir(dir)) != NULL)
    {
    	if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
    	{
    		string expansion = (strrchr(entry->d_name, '.'));

    		string file_name = argv[1];
    		file_name += "/";
    		file_name += entry->d_name;

    		Image *image;

    		if(expansion == ".png")
    		{
    			//cout<<"png"<<endl;
    			//cout<<"file = "<<file_name<<endl;
    			PNGReader reader(file_name);

    			image = reader.getImageStruct();    			
    		}
    		else
    		if(expansion == ".bmp")
    		{
    			//cout<<"bmp"<<endl;
    			//cout<<"file = "<<file_name<<endl;
    			BMPReader reader(file_name);

    			image = reader.getImageStruct();	
    			
    		}

    		float percent = isColorBasic(image, red, green, blue, range);
    		//cout<<"percent = "<<percent<<" file = "<<file_name<<endl;

    		   		
    		
    		if(percent >= 5.0)
    		{
    			cout<<file_name<<endl;
    		}
    		
    		delete image;
    	}        
    }
    
    closedir(dir);



	
	
	// 
	// //cout<<"!!! img height = "<<image->getHeight()<<" img width = "<<image->getWidth()<<endl;

	// // //for(int i = 0; i < image->getHeight(); i++)
	// // {
	// 	for(int j = 0; j < image->getWidth(); j++)
	// 	{
	// 		cout<<j<<":("<<image->getPixel(3,j).getRed()<<","<<image->getPixel(3,j).getGreen()<<
	// 		          ","<<image->getPixel(3,j).getBlue()<<","<<image->getPixel(3,j).getAlpha()<<")  ";
	// 	}
	// 	cout<<endl;
	// // }

	// int percent = 0;
	// // bool res = isColorBasic(image, 255, 0, 0, 0, percent);
	// // cout<<"percent = "<<percent<<" is basic = "<<res<<endl;

	// // float ***result;
	// int divider = 8;
	// // 
	
	// // getImageColors(image, divider, result);

	// for(int i = 0; i < divider; i++) // red
	// {
	// 	for(int j = 0; j < divider; j++) // green
	// 	{
	// 		for(int k = 0; k < divider; k++) // blue
	// 		{
	// 			//cout<<result[i][j][k]<<" ";
	// 		}
	// 	}
	// }
	


	return 0;
}