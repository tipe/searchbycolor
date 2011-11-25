#ifndef PNGREADER_H
#define PNGREADER_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <string>

#include "MyDecompressor.h"

using namespace std;


struct Pixel
{
private:
	unsigned int r;	
	unsigned int g;
	unsigned int b;

	unsigned int alpha;

public:

	Pixel(unsigned int r, unsigned int g, unsigned int b, unsigned int alpha)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->alpha = alpha;
	}

	Pixel()
	{
		Pixel(0,0,0,0);
	}

	void setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int alpha)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		//cout<<"!!! _alfa = "<<_alfa<<endl;
		this->alpha = alpha;
	}

	unsigned int getRed()
	{
		return r;
	}

	unsigned int getGreen()
	{
		return g;
	}

	unsigned int getBlue()
	{
		return b;
	}

	unsigned int getAlpha()
	{
		return alpha;
	}
};



struct Image
{
private:
	int width;
	int height;
	
	int color_type;
	
	Pixel **pixels;

public:	
	Image(int width, int height, int color_type)
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
	
	int getWidth()
	{
		return width;
	}
	
	int getHeight()
	{
		return height;
	}

	void setPixel(int i, int j, unsigned int r, unsigned int g, unsigned int b, unsigned int alpha=0)
	{
		pixels[i][j].setColor(r, g, b, alpha);
	}

	Pixel getPixel(int i, int j)
	{
		return pixels[i][j];
	}
	
	~Image()
	{
		if(pixels != NULL)
		{
			delete pixels;	//TODO incorrect memory deallocation!!!
		}
	}
};



typedef std::vector<char> V_IDATData;

typedef vector<unsigned int> ScanLine;
typedef vector<ScanLine> V_ScanLines;



class PNGReader
{
private:
	ifstream file;

	int img_width;
	int img_height;

	int bit_depth; 	
	int colour_type; 	
	int compr_method; 	
	int filter_method; 
	int interlace_method;

	V_IDATData v_idat_total_data;	

	void doInitData();
	
	bool is_bigendian();
	int getIntInRightOrder(char *buf);
	int getInt(char buf);
	unsigned int getIntSum(unsigned int i1, unsigned int i2, unsigned int i3);

	int readInt(int count);

	void getIHDRData();
	char* getIDATData(unsigned int &data_length);

	char* getPNGData(unsigned int &data_length);

	void doDeFiltering(V_ScanLines &v_scanlines, Image *image);
	void getScanLines(V_ScanLines &v_scanlines, char *decompr_data, int decompr_data_len);

	void getSizeSmallImage(int cur_small_img, int &col, int &row);
	void getCoordPixel(int cur_red_image, int cur_row, int cur_col, int &i, int &j);

	unsigned int getPaethPredictor(unsigned int prev, unsigned int up, unsigned int diag);

public:
	PNGReader(char *file_name);
	
	Image* getImageStruct();
};

#endif //PNGREADER_H
