#ifndef BMPREADER_H
#define BMPREADER_H

#include <iostream>
#include <fstream>
#include <string>

#include "ImageReader.h"


using namespace std;


class BMPReader : public ImageReader
{
private:
	int dib_header_size;
	int bits_per_pixel;

	int compresssion;
	int colors_in_color_table;

	int red_channel;
	int green_channel;
	int blue_channel;
	int alpha_channel;

	Pixel *color_table;

	
	void setImageColors(Image *image);
	int getIntInRightOrder(char *buf);
	unsigned int readInt();

public:
	BMPReader(string file_name);
	Image* getImageStruct();
};

#endif //BMPREADER_H