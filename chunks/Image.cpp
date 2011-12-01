#include "Image.h"



bool is_bigendian()
{
	const int ian = 1;
	return ( (*(char*)&ian) == 0 );
} 


int getIntInRightOrder(char *buf)
{
	int result = 0;
	if(!is_bigendian())
	{
		int tmp = 0;
		for(int i = 3; i >= 0; i--)
		{
			if(buf[i] < 0)
			{
				tmp = 256 + buf[i];
			}
			else
			{
				tmp = buf[i];
			}
			result += tmp*pow(256, 3-i);
		}
	}
	else
	{
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
	}
	return result;
}

unsigned int getUnsignedInt(char buf)
{
	unsigned int res = buf < 0 ? 256 + buf : (int)buf;
	return res;
}

unsigned int getIntSum(unsigned int i1, unsigned int i2, unsigned int i3)
{
	return (i1 + i2 + i3) % 256;
}


unsigned int readInt(ifstream& file)
{
	char buf[] = "0000";
	
	file.read(buf, 4);
	if(file.gcount() != 4)
	{
		throw runtime_error("111 File not read");
	}

	//cout<<"buf[0] = "<<(int)buf[0]<<" buf[1] = "<<(int)buf[1]<<" buf[2] = "<<(int)buf[2]<<" buf[3] = "<<(int)buf[3]<<endl;
	
	return getIntInRightOrder(buf);
}

unsigned int readByte(ifstream& file)
{
	char buf[] = "0";
	
	file.read(buf, 1);
	if(file.gcount() != 1)
	{
		throw runtime_error("222 File not read");
	}
	
	return getUnsignedInt(buf[0]);
}

unsigned int readTwoBytes(ifstream &file)
{
	char buf[] = "00";
	
	file.read(buf, 2);
	if(file.gcount() != 2)
	{
		throw runtime_error("333 File not read");
	}

	//cout<<"buf[0] = "<<(int)buf[0]<<" buf[1] = "<<(int)buf[1]<<endl;

	int result = 0;

	if (!is_bigendian())
	{
		//cout<<"1"<<endl;
		int tmp = 0;
		for(int i = 1; i >= 0; i--)
		{
			if(buf[i] < 0)
			{
				tmp = 256 + buf[i];
			}
			else
			{
				tmp = buf[i];
			}
			result += tmp*pow(256, 1-i);
		}
	}
	else
	{
		//cout<<"2"<<endl;
		int tmp = 0;
		for(int i = 0; i < 2; i++)
		{
			if(buf[i] < 0)
			{
				tmp = 256 + buf[i];
			}
			else
			{
				tmp = buf[i];
			}
			//cout<<"tmp = "<<tmp<<endl;
			result += tmp*pow(256, i);
			//cout<<"result = "<<result<<endl;
		}
	}

	result = result < 0 ? 256 + result : result;
	
	return result;
}


Pixel::Pixel(unsigned int r, unsigned int g, unsigned int b, unsigned int alpha)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->alpha = alpha;
}

Pixel::Pixel()
{
	Pixel(0,0,0,0);
}

void Pixel::setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int alpha)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->alpha = alpha;
}

unsigned int Pixel::getRed()
{
	return r;
}

unsigned int Pixel::getGreen()
{
	return g;
}

unsigned int Pixel::getBlue()
{
	return b;
}

unsigned int Pixel::getAlpha()
{
	return alpha;
}


Image::Image(int width, int height, int color_type)
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

Image::~Image()
{
	if(pixels != NULL)
	{
		for(int i = 0; i < height; i++)
		{
			delete[] pixels[i];
		}
		delete[] pixels;
	}
}

int Image::getWidth()
{
	return width;
}

int Image::getHeight()
{
	return height;
}

void Image::setPixel(int i, int j, unsigned int r, unsigned int g, unsigned int b, unsigned int alpha)
{
	pixels[i][j].setColor(r, g, b, alpha);
}

Pixel Image::getPixel(int i, int j)
{
	return pixels[i][j];
}



