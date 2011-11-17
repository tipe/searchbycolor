#include "MyDecompressor.h"
#include <iostream>
#include <stdexcept>

using namespace std;

#include "../zlib-1.2.5/zlib.h"

MyDecompressor::MyDecompressor(char *_compr_data, int _compr_data_length)
{
	compr_data = _compr_data;
	compr_data_length = _compr_data_length;
}


char* MyDecompressor::getDecomprData(int &_decompr_data_length)
{
	if(compr_data_length > 0)
	{
		decompr_data_length = 1000*compr_data_length;	//TODO remove magic number
		decompr_data = new char[decompr_data_length];

		if(uncompress((Bytef*)decompr_data, (uLong*)&decompr_data_length, (const Bytef*)compr_data, (uLong)compr_data_length) == 0)
		{
			_decompr_data_length = decompr_data_length;
			return decompr_data;
		}
	}
	return NULL;
}
