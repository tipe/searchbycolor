#include "MyDecompressor.h"
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

#include "../zlib-1.2.5/zlib.h"

MyDecompressor::MyDecompressor()
{
	
}


char* MyDecompressor::getDecomprData(unsigned int &decompr_data_length, char *compr_data, unsigned int compr_data_length)
{
	vector<int> data;

	if(compr_data_length > 0)
	{
		char *decompr_data = new char[decompr_data_length];
 		
		if(uncompress((Bytef*)decompr_data, (uLong*)&decompr_data_length, (const Bytef*)compr_data, (uLong)compr_data_length) == 0)
		{
			return decompr_data;
		}
	}
	return NULL;
}
