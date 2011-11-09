#include "PNGReader.h"

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdexcept>



const int count_chunks_type = 20;
const string ChunksType[] = {"IHDR", "PLTE", "IDAT", "IEND",
                 			"bKGD", "cHRM", "gAMA", "gAMA", "hIST", "iCCP", "iTXt", "pHYs",
                 			"sBIT", "sPLT", "sRGB", "sRGB", "tEXt", "tIME", "tRNS", "zTXt"};

bool PNGReader::is_bigendian()
{
	const int ian = 1;
	return ( (*(char*)&ian) == 0 );
} 


int PNGReader::getIntInRightOrder(char *buf)
{
	int result = 0;
	if (!is_bigendian())
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
		result = (int)buf;
	}
	return result;
}

int PNGReader::getIDChunk(char *buf)
{
	for(int i = 0; i < count_chunks_type; i++)
	{
		if(strcmp(buf, ChunksType[i].c_str())==0)
		{
			return i;
		}
	}	
	return -1;
}


void PNGReader::getIHDRData()
{
	int cur_file_pos = ftell(file);
	char buf[] = "0000";

	if(fread(buf, 4, 1, file) != 1)
	{
		throw runtime_error("PNG file not read");
	}	
	file_width = getIntInRightOrder(buf);

	if(fread(buf, 4, 1, file) != 1)
	{
		throw runtime_error("PNG file not read");
	}	
	file_heigth = getIntInRightOrder(buf);

	if(fseek(file, cur_file_pos, SEEK_SET) != 0)
	{
		throw runtime_error("PNG file not read");
	}
}


char* PNGReader::getIDATData()
{
	int cur_file_pos = ftell(file);
	char *idat_tmp_data = new char[data_length];
	if(fread(idat_tmp_data, data_length, 1, file) != 1)
	{
		throw runtime_error("PNG file not read");
	}
	if(fseek(file, cur_file_pos, SEEK_SET) != 0)
	{
		throw runtime_error("PNG file not read");
	}
	return idat_tmp_data;
}


PNGReader::PNGReader(char *file_name)
{
	file = fopen(file_name, "rb");
	if(file == NULL)
	{
		throw runtime_error("PNG file not read");
	}

	if(fseek(file, 0, SEEK_END) != 0)
	{
		throw runtime_error("PNG file not read");
	}
	file_size = ftell(file);
	if(fseek(file, 0, SEEK_SET) != 0)
	{
		throw runtime_error("PNG file not read");
	}
	file_width = 0;
	file_heigth = 0;

	data_length = 0;
	char buf[] = "0000";
	
		
	if(fseek(file, 8, SEEK_SET) != 0)
	{
		throw runtime_error("PNG file not read");
	}		
		
	while(fread(buf, 1, 4, file)==4)
	{
	 	data_length = getIntInRightOrder(buf);
					
		if(fread(buf, 4, 1, file) != 1)
		{
	 		throw runtime_error("PNG file not read");
		}
			
		if(getIDChunk(buf) != -1)
		{
			v_chunks_params.push_back(ChunkParams(ChunksType[getIDChunk(buf)], data_length));
		}

			
		if(strcmp(buf, "IHDR")==0)
		{
			getIHDRData();
		}

		if(strcmp(buf, "IDAT")==0)
		{
			char *idat_tmp_buf = getIDATData();
				
			for(unsigned int i = 0; i < data_length; i++)
			{
				v_idat_total_data.push_back(idat_tmp_buf[i]);
			}
		}
			
			
		if(fseek(file, data_length+4, SEEK_CUR) != 0)
		{
			throw runtime_error("PNG file not read");
		}		
	}		

	fclose(file);
}


unsigned int PNGReader::getFileSize()
{
	return file_size;
}

unsigned int PNGReader::getFileWidth()
{
	return file_width;
}

unsigned int PNGReader::getFileHeigth()
{
	return file_heigth;
}


V_ChunksParams* PNGReader::getPNGChunksParams()
{
	V_ChunksParams *v_chunks = new V_ChunksParams();
	for(int i = 0; i < getChunksCount(); i++)
	{
		v_chunks->push_back(ChunkParams(v_chunks_params[i].chunk_type, v_chunks_params[i].chunk_length));
	}
	
	return v_chunks;
}


char* PNGReader::getPNGData(int &data_length)
{
	data_length = v_idat_total_data.size();
	char *idat_data = new char[data_length];
	for(int i = 0; i < data_length; i++)
	{
		idat_data[i] = v_idat_total_data[i];
	}
	return idat_data;
}


int PNGReader::getChunksCount()
{
	return v_chunks_params.size();
}