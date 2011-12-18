#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include <string>
#include "Image.h"



class ImageReader
{
protected:
	std::string file_name;

	int img_width;
	int img_height;
	
	bool is_bigendian();
	int getIntInRightOrder(bool is_bigendian, char *buf);
	unsigned int getUnsignedInt(int buf);
	unsigned int getIntSum(unsigned int i1, unsigned int i2, unsigned int i3=0);
	unsigned int readByte(std::ifstream &file);
	unsigned int readTwoBytes(std::ifstream &file);


public:
	virtual Image* getImageStruct()=0;
	//????virtual ~ImageReader()=0;
};

#endif //IMAGEREADER_H