#ifndef BMPREADER_H
#define BMPREADER_H

#include <iostream>
#include <fstream>
#include <string>

#include "Image.h"

using namespace std;


class BMPReader
{
private:
	ifstream file;
	string file_name;

	int img_width;
	int img_height;

	int dib_header_size;
	int bits_per_pixel;

	int compresssion;
	int colors_in_color_table;

	int red_channel;
	int green_channel;
	int blue_channel;
	int alpha_channel;

	
	void setImageColors(Image *image);

public:
	BMPReader(char *file_name);
	Image* getImageStruct();
};

#endif //BMPREADER_H