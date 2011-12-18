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
	int file_size;

	int img_size;

	int dib_header_size;
	int bits_per_pixel;

	int compresssion;
	int colors_in_color_table;

	int shift;

	int red_channel;
	int green_channel;
	int blue_channel;
	int alpha_channel;

	Pixel *color_table;

	
	void setImageColors(Image *image, ifstream &file);
	void getUncomprColors(Image *image, ifstream &file);

	int getIntInRightOrder(char *buf);
	unsigned int readInt(ifstream &file);

public:
	BMPReader(string &file_name);
	~BMPReader();

	Image* getImageStruct();
};

#endif //BMPREADER_H