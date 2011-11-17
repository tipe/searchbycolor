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

	unsigned int alfa;

public:

	Pixel(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _alfa)
	{
		r = _r;
		g = _g;
		b = _b;
		alfa = _alfa;
	}

	Pixel()
	{
		Pixel(0,0,0,0);
	}

	void setColor(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int alfa)
	{
		r = _r;
		g = _g;
		b = _b;
		alfa = alfa;	//TODO assignment to itself
	}

	unsigned int getR()
	{
		return r;
	}

	unsigned int getG()
	{
		return g;
	}

	unsigned int getB()
	{
		return b;
	}
};



struct Image
{
	int size;	//TODO duplicating data!
	int width;
	int height;
	
	int color_type;
	
	Pixel **pixels;
	
	Image(int _width, int _height, int _color_type)
	{
		width = _width;
		height = _height;

		size = width*height;

		pixels = new Pixel*[height];	//TODO no memory deallocation!!!
		for(int i = 0; i < height; i++)
		{
			pixels[i] = new Pixel[width];
		}

		color_type = _color_type;
	}

	void setPixel(int i, int j, unsigned int _r, unsigned int _g, unsigned int _b, unsigned int alfa=0)
	{
		pixels[i][j].setColor(_r, _g, _b, alfa);
	}

	Pixel getPixel(int i, int j)
	{
		return pixels[i][j];
	}
};


struct ChunkParams
{
	string chunk_type;
	unsigned int chunk_length;

	ChunkParams(string type, int lenght)
	{
		chunk_type = type;
		chunk_length = lenght;
	}
};

typedef std::vector<ChunkParams> V_ChunksParams;

typedef std::vector<char> V_IDATData;

class PNGReader
{
private:
	ifstream file;

	bool isInitData;
	unsigned int img_size;
	int img_width;
	int img_heigth;	//TODO correct misprint

	int bit_depth; 	
	int colour_type; 	
	int compr_method; 	
	int filter_method; 
	int interlace_method;

	unsigned int data_length;

	V_IDATData v_idat_total_data;	

	void doInitData();
	
	bool is_bigendian();
	int getIntInRightOrder(char *buf);
	int getInt(char buf);
	unsigned int getIntSum(unsigned int i1, unsigned int i2);

	void getIHDRData();
	char* getIDATData();

	char* getPNGData(int &data_length);

	void doDeFiltering(char *decompr_data, int decompr_data_len, Image *image);
	int getIndexFilteringType(int cur_small_img);

	void getSizeSmallImage(int cur_small_img, int &col, int &row);

public:
	PNGReader(char *file_name);
	
	unsigned int getImgSize();	//TODO duplicating data in Image struct
	unsigned int getImgWidth();
	unsigned int getImgHeigth();
	
	Image* getImageStruct();
};

#endif //PNGREADER_H
