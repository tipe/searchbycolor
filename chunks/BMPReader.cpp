#include "BMPReader.h"

#include <string.h>
#include <stdexcept>


BMPReader::BMPReader(string file_name)
{
	this->file_name = file_name;
	file.open(file_name.c_str(), fstream::binary);
	if(file.good() != true)
	{
		throw runtime_error("PNG file not open");
	}
	file.close();
}


int BMPReader::getIntInRightOrder(char *buf)
{
	int result = 0;
	
	int tmp = 0;
	for(int i = 0; i < 4; i++)
	{
		if(buf[i] < 0)
		{
			tmp = 256 + buf[i];
		}
		else
		{
			tmp = buf[i];
		}
		result += tmp*pow(256, i);
	}
	
	return result;
}

unsigned int BMPReader::readInt()
{
	char buf[] = "0000";
	
	file.read(buf, 4);
	if(file.gcount() != 4)
	{
		throw runtime_error("111 File not read");
	}

	return getIntInRightOrder(buf);
}


void BMPReader::setImageColors(Image *image)
{
	int blue, green, red, alpha = 0;

	switch(bits_per_pixel)
	{
		case 1:

			break;

		case 24:

			for(int i = 0; i < img_height; i++)
			{
				for(int j = 0; j < img_width; j++)
				{

					blue = readByte();
					green = readByte();
					red = readByte();

					image->setPixel(img_height - 1 - i, j, red, green, blue);
				}

				file.seekg(3, ios::cur);
			}
			break;

		case 32:

			for(int i = 0; i < img_height; i++)
			{
				for(int j = 0; j < img_width; j++)
				{
					blue = readByte();
					green = readByte();
					red = readByte();
					alpha = readByte();

					image->setPixel(img_height - 1 - i, j, red, green, blue, alpha);
				}
			}
			break;
	}
}

Image* BMPReader::getImageStruct()
{
	img_width = 0;
	img_height = 0;

	file.open(file_name.c_str(), fstream::binary);

	file.seekg(10, ios::beg);
	int offset = readInt();
	//cout<<"offset = "<<offset<<endl;
		
	file.seekg(4, ios::cur);

	img_width = readInt();
	img_height = readInt();

	//cout<<"img_width = "<<img_width<<endl;
	//cout<<"img_height = "<<img_height<<endl;

	file.seekg(2, ios::cur);

	bits_per_pixel = readTwoBytes();
	//cout<<"bits_per_pixel = "<<bits_per_pixel<<endl;

	compresssion = readInt();
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
