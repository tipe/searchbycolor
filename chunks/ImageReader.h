#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include <string>
#include "Image.h"

using namespace std;


class ImageReader
{
protected:
	ifstream file;
	string file_name;

	int img_width;
	int img_height;
	
	bool is_bigendian();
	//int getIntInRightOrder(char *buf);
	unsigned int getUnsignedInt(char buf);
	unsigned int getIntSum(unsigned int i1, unsigned int i2, unsigned int i3=0);
	//unsigned int readInt();
	unsigned int readByte();
	unsigned int readTwoBytes();


public:
	virtual Image* getImageStruct()=0;
};

#endif //IMAGEREADER_H