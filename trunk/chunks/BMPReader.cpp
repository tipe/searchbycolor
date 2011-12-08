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

	//cout<<"buf[0] = "<<getUnsignedInt(buf[0])<<" buf[1] = "<<getUnsignedInt(buf[1])<<" buf[2] = "<<getUnsignedInt(buf[2])
	  //    <<" buf[3] = "<<getUnsignedInt(buf[3])<<endl;

	return getIntInRightOrder(buf);
}


void BMPReader::setImageColors(Image *image)
{
	int B, G, R, A, index;

	switch(bits_per_pixel)
	{
		case 1:
			for(int i = 0; i < img_height; i++)
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

				file.seekg(shift, ios::cur);
			}

			break;

		case 2:
			for(int i = 0; i < img_height; i++)
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

				file.seekg(shift, ios::cur);
			}

			break;

		case 4:
			for(int i = 0; i < img_height; i++)
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

				file.seekg(shift, ios::cur);
			}

			break;

		case 8:
			for(int i = 0; i < img_height; i++)
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

				file.seekg(shift, ios::cur);
			}

			break;

		case 16:
			for(int i = 0; i < img_height; i++)
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

				file.seekg(shift, ios::cur);
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

				file.seekg(shift, ios::cur);
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

				file.seekg(shift, ios::cur);
			}
			break;
	}
}


void BMPReader::getUncomprColors(Image *image)
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
				int count = readByte();
				int index = readByte();

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
			

			break;
	}	
}


Image* BMPReader::getImageStruct()
{
	img_width = 0;
	img_height = 0;

	file.open(file_name.c_str(), fstream::binary);

	file.seekg(2, ios::beg);
	file_size = readInt();
	//cout<<"file_size = "<<file_size<<endl;

	file.seekg(4, ios::cur);
	int offset = readInt();
	//cout<<"offset = "<<offset<<endl;
		
	dib_header_size = readInt();
	//cout<<"dib_header_size = "<<dib_header_size<<endl;


	img_width = readInt();
	img_height = readInt();

	//cout<<"img_width = "<<img_width<<endl;
	//cout<<"img_height = "<<img_height<<endl;

	file.seekg(2, ios::cur);

	bits_per_pixel = readTwoBytes();
	//cout<<"bits_per_pixel = "<<bits_per_pixel<<endl;

	compresssion = readInt();
	//cout<<"compresssion = "<<compresssion<<endl;

	img_size = readInt();

	file.seekg(2*4, ios::cur);
	colors_in_color_table = readInt();
	

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
			// 	cout<<"i = "<<i<<"  ";
			R = readByte();
			G = readByte();
			B = readByte();
			A = readByte();


			// 	cout<<"  R = "<<R<<" G = "<<G<<" B = "<<B<<" A = "<<color_table<<"  ";

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

	if(compresssion == 0)
	{
		setImageColors(image);	
	}
	else
	{
		getUncomprColors(image);
	}
	

	file.close();

	return image;
}
