#include "ImageReader.h"

using namespace std;


bool ImageReader::is_bigendian()
{
	const int ian = 1;
	return ( (*(char*)&ian) == 0 );
} 


int ImageReader::getIntInRightOrder(bool is_bigendian, char *buf)
{
	int result = 0;
	if(!is_bigendian)
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



unsigned int ImageReader::getUnsignedInt(int buf)
{
	unsigned int res = buf < 0 ? 256 + buf : buf;
	return res;
}


unsigned int ImageReader::getIntSum(unsigned int i1, unsigned int i2, unsigned int i3)
{
	return (i1 + i2 + i3) % 256;
}


unsigned int ImageReader::readByte(ifstream &file)
{
	char buf[] = "0";
	
	file.read(buf, 1);
	int count = file.gcount();
	if(count != 1)
	{
		throw runtime_error("222 File not read");
	}

	//cerr<<"!!! buf[0] = "<<getUnsignedInt(buf[0])<<endl;
	
	return getUnsignedInt((int)buf[0]);

}

unsigned int ImageReader::readTwoBytes(ifstream &file)
{
	char buf[] = "00";
	
	file.read(buf, 2);
	if(file.gcount() != 2)
	{
		throw runtime_error("333 File not read");
	}

	//cout<<"buf[0] = "<<(int)buf[0]<<" buf[1] = "<<(int)buf[1]<<endl;

	int result = 0;

	if (is_bigendian())
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