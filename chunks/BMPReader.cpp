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
	int B, G, R, A, index;

	switch(bits_per_pixel)
	{
		case 1:
			for(int i = 0; i < img_height; i+=8)//???????
			{
				for(int j = 0; j < img_width; j+=8)
				{
					char buf[] = "00000000";
					file.read(buf, 1);

					for(int k = 0; k < 8; k++)
					{
						index = (int)buf[k];

						R = color_table[index].getRed();
						G = color_table[index].getGreen();
						B = color_table[index].getBlue();
						A = color_table[index].getAlpha();

						image->setPixel(img_height - 1 - i, j, R, G, B, A);
					}
					
				}

				file.seekg(3, ios::cur);//????????????
			}

			break;

		case 2:
			for(int i = 0; i < img_height; i+=4)//???????
			{
				for(int j = 0; j < img_width; j+=4)
				{
					char buf[] = "00000000";
					file.read(buf, 1);

					for(int k = 0; k < 8; k+=2)
					{
						index = 2*(int)buf[k] + (int)buf[k+1];

						R = color_table[index].getRed();
						G = color_table[index].getGreen();
						B = color_table[index].getBlue();
						A = color_table[index].getAlpha();

						image->setPixel(img_height - 1 - i, j, R, G, B, A);
					}
					
				}

				file.seekg(3, ios::cur);//????????????
			}

			break;

		case 4:
			for(int i = 0; i < img_height; i+=2)//???????
			{
				for(int j = 0; j < img_width; j+=2)
				{
					char buf[] = "00000000";
					file.read(buf, 1);

					for(int k = 0; k < 8; k+=4)
					{
						index = 8*(int)buf[k] + 4*(int)buf[k+1] + 2*(int)buf[k+2] + (int)buf[k+3];

						R = color_table[index].getRed();
						G = color_table[index].getGreen();
						B = color_table[index].getBlue();
						A = color_table[index].getAlpha();

						image->setPixel(img_height - 1 - i, j, R, G, B, A);
					}
					
				}

				file.seekg(3, ios::cur);//????????????
			}

			break;

		case 8:
			for(int i = 0; i < img_height; i++)//???????
			{
				for(int j = 0; j < img_width; j++)
				{
					index = readByte();

					R = color_table[index].getRed();
					G = color_table[index].getGreen();
					B = color_table[index].getBlue();
					A = color_table[index].getAlpha();

					image->setPixel(img_height - 1 - i, j, R, G, B, A);
				}

				file.seekg(3, ios::cur);//????????????
			}

			break;

		case 16:
			for(int i = 0; i < img_height; i++)//???????
			{
				for(int j = 0; j < img_width; j++)
				{
					index = readTwoBytes();

					R = color_table[index].getRed();
					G = color_table[index].getGreen();
					B = color_table[index].getBlue();
					A = color_table[index].getAlpha();

					image->setPixel(img_height - 1 - i, j, R, G, B, A);
				}

				file.seekg(3, ios::cur);//????????????
			}

			break;

		case 24:

			for(int i = 0; i < img_height; i++)
			{
				for(int j = 0; j < img_width; j++)
				{

					B = readByte();
					G = readByte();
					R = readByte();
					
					image->setPixel(img_height - 1 - i, j, R, G, B);
				}

				file.seekg(3, ios::cur);
			}
			break;

		case 32:

			for(int i = 0; i < img_height; i++)
			{
				for(int j = 0; j < img_width; j++)
				{
					B = readByte();
					G = readByte();
					R = readByte();
					A = readByte();

					image->setPixel(img_height - 1 - i, j, R, G, B, A);
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
		
	dib_header_size = readInt();

	//file.seekg(4, ios::cur);

	img_width = readInt();
	img_height = readInt();

	//cout<<"img_width = "<<img_width<<endl;
	//cout<<"img_height = "<<img_height<<endl;

	file.seekg(2, ios::cur);

	bits_per_pixel = readTwoBytes();
	//cout<<"bits_per_pixel = "<<bits_per_pixel<<endl;

	compresssion = readInt();
	//cout<<"compresssion = "<<compresssion<<endl;

	file.seekg(3*4, ios::cur);
	colors_in_color_table = readInt();
	//cout<<"colors_in_color_table = "<<colors_in_color_table<<endl;

	if(colors_in_color_table > 0)
	{
		int cur_pos = file.tellg();
		file.seekg(14 + dib_header_size - cur_pos, ios::cur);

		color_table = new Pixel[colors_in_color_table];

		int R, G, B, A;

		for(int i = 0; i < colors_in_color_table; i++)
		{
			R = readInt();
			G = readInt();
			B = readInt();
			A = readInt();

			color_table[i].setColor(R, G, B, A);
		}

	}
	else
	{
		int cur_pos = file.tellg();

		//cout<<"cur_pos = "<<cur_pos<<" offset - cur_pos = "<<offset - cur_pos<<endl;

		file.seekg(offset - cur_pos, ios::cur);
	}

	Image *image = new Image(img_width, img_height);
	setImageColors(image);

	file.close();

	return image;
}
