#include "BMPReader.h"

#include <string.h>
#include <stdexcept>


BMPReader::BMPReader(char *file_name)
{
	this->file_name = file_name;
	file.open(file_name, fstream::binary);
	if(file.good() != true)
	{
		throw runtime_error("PNG file not open");
	}
	file.close();
}


void BMPReader::setImageColors(Image *image)
{
	int blue, green, red, alpha = 0;

	switch(bits_per_pixel)
	{
		case 24:

			for(int i = 0; i < img_height; i++)
			{
				for(int j = 0; j < img_width; j++)
				{
					blue = readByte(file);
					green = readByte(file);
					red = readByte(file);

					image->setPixel(j, img_height - 1 - i, red, green, blue);
				}

				file.seekg(4, ios::cur);
			}
			break;
	}
}

Image* BMPReader::getImageStruct()
{
	img_width = 0;
	img_height = 0;

	unsigned int data_length = 0;
	char buf[] = "0000";	

	file.open(file_name.c_str(), fstream::binary);

	file.seekg(10, ios::beg);
	int offset = readInt(file);
		
	file.seekg(4, ios::cur);

	img_width = readInt(file);
	img_height = readInt(file);

	//cout<<"img_width = "<<img_width<<endl;
	//cout<<"img_height = "<<img_height<<endl;

	file.seekg(2, ios::cur);

	bits_per_pixel = readTwoBytes(file);
	//cout<<"bits_per_pixel = "<<bits_per_pixel<<endl;

	compresssion = readInt(file);
	//cout<<"compresssion = "<<compresssion<<endl;

	int cur_pos = file.tellg();

	//cout<<"cur_pos = "<<cur_pos<<" offset - cur_pos = "<<offset - cur_pos<<endl;

	file.seekg(offset - cur_pos, ios::cur);

	int color_type;
	if(bits_per_pixel == 24)
	{
		color_type = 2;
	}

	Image *image = new Image(img_width, img_height, color_type);
	setImageColors(image);

	file.close();

	return image;
}
