#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <string>

#include "zlib-1.2.5/zlib.h"

using namespace std;



const int ian = 1;
#define is_bigendian() ( (*(char*)&ian) == 0 )

int getIntInRightOrder(char *buf)
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

//==============================

int getFileSize(FILE *file)
{
	int cur_file_pos = ftell(file);

	if( fseek(file, 0, SEEK_END) != 0)
	{
		printf("File not read\n");
		fseek(file, cur_file_pos, SEEK_SET);
		return 1;
	}

	fseek(file, cur_file_pos, SEEK_SET);
	return ftell(file);
}

//==============================

const int count_chunks_type = 20;
const string chunks[] = {"IHDR", "PLTE", "IDAT", "IEND",
                   "bKGD", "cHRM", "gAMA", "gAMA", "hIST", "iCCP", "iTXt", "pHYs",
                   "sBIT", "sPLT", "sRGB", "sRGB", "tEXt", "tIME", "tRNS", "zTXt"};

bool isChunk(char *buf)
{
	for(int i = 0; i < count_chunks_type; i++)
	{
		if(strcmp(buf, chunks[i].c_str())==0)
		{
			return 1;
		}
	}	
	return 0;
}

int getIHDRData(FILE *file, int &width, int &heigth)
{
	int cur_file_pos = ftell(file);
	char buf[] = "0000";

	if(fread(buf, 4, 1, file) != 1)
	{
		printf("PNG file not read\n");
		return 1;
	}	
	width = getIntInRightOrder(buf);

	if(fread(buf, 4, 1, file) != 1)
	{
		printf("PNG file not read\n");
		return 1;
	}	
	heigth = getIntInRightOrder(buf);

	fseek(file, cur_file_pos, SEEK_SET);
	return 0;
}

bool isIHDRChunk(char *buf)
{
	if(strcmp(buf, "IHDR")==0)
	{
		return 1;
	}
	return 0;
}

bool isIDATChunk(char *buf)
{
	if(strcmp(buf, "IDAT")==0)
	{
		return 1;
	}
	return 0;
}

int getIDATData(char *idat_buf, FILE *file, int data_length)
{
	int cur_file_pos = ftell(file);
	int i_read = fread(idat_buf, data_length, 1, file); 
	if(i_read != 1)
	{
		printf("PNG file not read\n");
		return 1;
	}
	fseek(file, cur_file_pos, SEEK_SET);
	return 0;
}

int getUncompData(char *uncompr_buf, int &uncomp_data_length, char *idat_buf, int idat_data_length)
{
	if(idat_data_length > 0)
	{
		uncomp_data_length = 1000*idat_data_length;
		uncompr_buf = new char[uncomp_data_length];

		int uncompr_result = uncompress((Bytef*)uncompr_buf, (uLong*)&uncomp_data_length, (const Bytef*)idat_buf, (uLong)idat_data_length);
		return uncompr_result;
	}
	return 0;
}

//==============================



int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("Please enter name of PNG file\n");
		return 1;
	}

	FILE *file = fopen(argv[1], "rb");

	if(file == NULL)
	{
		printf("PNG file not found\n");
		return 1;
	}

	char buf[] = "0000";
	unsigned int length_chunk_data = 0;
	int count_chunks = 0;
	int idat_data_length = 0;
	char *idat_buf = NULL;
	char *idat_tmp_buf = NULL;

	int file_width = 0;
	int file_heigth = 0;

		
	if(fseek(file, 8, SEEK_SET) != 0)
	{
		printf("PNG file not read\n");
		return 1;
	}
	
	
	while(fread(buf, 1, 4, file)==4)
	{
		length_chunk_data = getIntInRightOrder(buf);
				
		if(fread(buf, 4, 1, file) != 1)
		{
			printf("PNG file not read\n");
			return 1;
		}
		
		if(isChunk(buf))
		{
			count_chunks++;
			printf("%s (%d)\n", buf, length_chunk_data);
		}

		
		if(isIHDRChunk(buf))
		{
			if(getIHDRData(file, file_width, file_heigth) != 0)
			{
				cout<<"PNG file not read"<<endl;
				return 1;
			}
		}

		if(isIDATChunk(buf))
		{
			if(idat_tmp_buf != NULL)
			{
				delete idat_tmp_buf;
				idat_tmp_buf = NULL;
			}
			idat_tmp_buf = new char[length_chunk_data];


			getIDATData(idat_tmp_buf, file, length_chunk_data);
			
			if(idat_buf == NULL)
			{
				idat_buf = new char[getFileSize(file)];
			}

			for(unsigned int i = 0; i < length_chunk_data; i++)
			{
				idat_buf[idat_data_length + i] = idat_tmp_buf[i];
			}

			idat_data_length += length_chunk_data;
		}
		
		
		if(fseek(file, length_chunk_data+4, SEEK_CUR) != 0)
		{
			printf("PNG file not read\n");
			return 1;
		}
		
	}		

	fclose(file);

	
	char *uncompr_buf;
	int uncomp_data_length = 0;
	int uncompr_result = getUncompData(uncompr_buf, uncomp_data_length, idat_buf, idat_data_length);
	cout<<"uncopr res = "<<uncompr_result<<endl;

	
	for(int i = 0; i < file_heigth; i++)
	{
		for(int j = 0; j < file_width; j+=4)
		{
			
		}
	}
	

	
    

	return 0;
}