#include "BMPReader.h"

#include <string.h>
#include <stdexcept>


BMPReader::BMPReader(string &file_name)
{
	this->file_name = file_name;
	color_table = NULL;
}

BMPReader::~BMPReader()
{
	if(color_table != NULL)
	{
		delete[] color_table;
	}
}



int BMPReader::getIntInRightOrder(char *buf)
{
	return ImageReader::getIntInRightOrder(1, buf);
}

unsigned int BMPReader::readInt(ifstream &file)
{
	char buf[] = "0000";
	
	file.read(buf, 4);
	if(file.gcount() != 4)
	{
		throw runtime_error("111 File not read");
	}

	//cout<<"buf[0] = "<<getUnsignedInt(buf[0])<<" buf[1] = "<<getUnsignedInt(buf[1])<<" buf[2] = "<<getUnsignedInt(buf[2])
	  //    <<" buf[3] = "<<getUnsignedInt(buf[3])<<endl;

	return getIntInRightOrder(buf);
}


void BMPReader::setImageColors(Image *image, ifstream &file)
{
	int B, G, R, A=0, index;	

	if(bits_per_pixel <= 16)
	{
		int step = 8/bits_per_pixel;

		for(int i = 0; i < img_height; i++)
		{
			for(int j = 0; j < img_width; j+=step)
			{
				if(bits_per_pixel <= 8)
				{
					if(j < img_width - img_width % 8 + 1)
					{
						char buf = readByte(file);

						for(int k = 0; k < 8; k+=bits_per_pixel)
						{
							if(j+k/bits_per_pixel < img_width)
							{
								index  = 0;
								
								for(int s = 0; s < bits_per_pixel; ++s)
								{
									int d = ( buf >> (s+k) ) & 1;
									index += d*pow(2, s);
								}
								

								R = color_table[index].getRed();
								G = color_table[index].getGreen();
								B = color_table[index].getBlue();
								A = color_table[index].getAlpha();

								image->setPixel(img_height - 1 - i, j+k/bits_per_pixel, R, G, B, A);
							}
						}	
					}						
				}
				else
				{
					index = readTwoBytes(file);
					
					R = color_table[index].getRed();
					G = color_table[index].getGreen();
					B = color_table[index].getBlue();
					A = color_table[index].getAlpha();

					image->setPixel(img_height - 1 - i, j, R, G, B, A);
				}
			}

			file.seekg(shift, ios::cur);
		}

	}
	else
	{
		for(int i = 0; i < img_height; i++)
		{
			for(int j = 0; j < img_width; j++)
			{
				B = readByte(file);
				G = readByte(file);
				R = readByte(file);

				if(bits_per_pixel == 32)
					A = readByte(file);
				
				image->setPixel(img_height - 1 - i, j, R, G, B, A);
			}

			file.seekg(shift, ios::cur);
		}
	}	
}


void BMPReader::getUncomprColors(Image *image, ifstream &file)
{
	int R, G, B, A;

	switch(compresssion)
	{
		case 1:
			int img_pos = 0;
			int i = 0;
			int j = 0;

			while(img_pos < img_size)
			{
				int count = readByte(file);
				int index = readByte(file);

				if(count > 0)
				{
					for(int k = 0; k < count; k++)
					{
						R = color_table[index].getRed();
						G = color_table[index].getGreen();
						B = color_table[index].getBlue();
						A = color_table[index].getAlpha();

						i += k;

						if(i >= img_width)
						{
							i = 0;
							j++;
						}

						image->setPixel(i, j, R, G, B, A);
					}	
				}				

				img_pos += 2;
			}

	}	
}


Image* BMPReader::getImageStruct()
{
	img_width = 0;
	img_height = 0;

	ifstream file;
	file.open(file_name.c_str(), fstream::binary);
	
	if(!file.good())
	{
		throw runtime_error("PNG file not open");
	}

	file.seekg(2, ios::beg);
	file_size = readInt(file);
	//cout<<"file_size = "<<file_size<<endl;

	file.seekg(4, ios::cur);
	int offset = readInt(file);
	//cout<<"offset = "<<offset<<endl;
		
	dib_header_size = readInt(file);
	//cout<<"dib_header_size = "<<dib_header_size<<endl;


	img_width = readInt(file);
	img_height = readInt(file);

	//cout<<"img_width = "<<img_width<<endl;
	//cout<<"img_height = "<<img_height<<endl;

	file.seekg(2, ios::cur);

	bits_per_pixel = readTwoBytes(file);
	//cout<<"bits_per_pixel = "<<bits_per_pixel<<endl;

	compresssion = readInt(file);
	//cout<<"compresssion = "<<compresssion<<endl;

	img_size = readInt(file);

	file.seekg(2*4, ios::cur);
	colors_in_color_table = readInt(file);
	

	if(dib_header_size == 40)
	{
		shift = ( (file_size - offset)*8 - bits_per_pixel*img_height*img_width )/img_height;
		shift /= 8;

		// switch(bits_per_pixel)
		// {
		// 	case 1:

		// 		shift = (file_size - offset - img_height*img_width)/img_height;
		// }
	}
	else
	{
		throw runtime_error("Unknown data");
	}

	
	
	if(bits_per_pixel < 24)//(colors_in_color_table > 0)
	{
		int cur_pos = file.tellg();
		file.seekg(14 + dib_header_size - cur_pos, ios::cur);

		colors_in_color_table = (offset - 14 - dib_header_size)/4;
		//cout<<"! colors_in_color_table = "<<colors_in_color_table<<endl;


		color_table = new Pixel[colors_in_color_table];

		int R, G, B, A;
		

		
		for(int i = 0; i < colors_in_color_table; i++)
		{
			R = readByte(file);
			G = readByte(file);
			B = readByte(file);
			A = readByte(file);

			color_table[i].setColor(R, G, B, A);
		}

	}
	else
	{
		int cur_pos = file.tellg();

		file.seekg(offset - cur_pos, ios::cur);
	}
	
	Image *image = new Image(img_width, img_height);

	if(compresssion == 0)
	{
		setImageColors(image, file);	
	}
	else
	{
		getUncomprColors(image, file);
	}
	

	file.close();

	return image;
}
