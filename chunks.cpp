#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <string>

using namespace std;


const int ian = 1;
#define is_bigendian() ( (*(char*)&ian) == 0 )


#include "zlib-1.2.5/zlib.h"

int count_type_chunks = 20;
char* chunks[] = {"IHDR", "PLTE", "IDAT", "IEND",
                   "bKGD", "cHRM", "gAMA", "gAMA", "hIST", "iCCP", "iTXt", "pHYs",
                   "sBIT", "sPLT", "sRGB", "sRGB", "tEXt", "tIME", "tRNS", "zTXt"};

char buf[256];
unsigned int length = 0;

int count_chunks = 0;


int  count_uncompr = 256;
char data_uncompr[256];

int  count_compr;
char *data_compr;

int file_size = 0;
int cur_pos = 0;

int i_read = 0;
int i_seek = 0;


bool isCriticalChunk(char *buf)
{
	//cout<<"buf = "<<buf<<endl;
	for(int i = 0; i < count_type_chunks; i++)
	{
		//cout<<"chunk = "<<chunks[i]<<endl;
		if(strcmp(buf, chunks[i])==0)
		{
			return 1;
		}
	}	
	return 0;
}


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

	

	// get file size
	// i_seek = fseek(file, 0, SEEK_END);
	// if(i_seek != 0)
	// {
	// 	printf("1 PNG file not read\n");
	// 	return 1;
	// }
	// file_size = ftell(file);
	// printf("file size = %d\n", file_size);

	int flag = 0; // watch cur file pos

	
	i_seek = fseek(file, 8, SEEK_SET);
	if(i_seek != 0)
	{
		printf("2 PNG file not read\n");
		return 1;
	}
	flag+=8;
	cur_pos =  ftell(file);

	// printf("!!!! cur file pos = %d\n", ftell(file));

	// printf("file cur pos = %ld\n", ftell(file));


	// for(int i = 9; i < file_size-1; i++)
	// {
	// 	i_read = fread(buf, 1, 1, file);
	// 	if(i_read != 1)
	// 	{
	// 		printf("22 PNG file not read\n");
	// 		return 1;
	// 	}

	// 	if(i == 84 || i == 85 || i == 86 || i == 87)
	// 	{
	// 		printf("!!!! buf[%d] = %d\n", i, (int)buf[0]);
	// 	}

	// 	if(buf[0] == 's' || buf[0] == 'R' || buf[0] == 'G' || buf[0] == 'B')
	// 	{
	// 		printf("buf = %s\n", buf[0]);
	// 		printf("file cur pos = %ld\n", ftell(file));
	// 		continue;
	// 	}
	// 	if(buf[0] == 'g' || buf[0] == 'A' || buf[0] == 'M' || buf[0] == 'A')
	// 	{
	// 		printf("buf = %s\n", buf[0]);
	// 		printf("file cur pos = %ld\n", ftell(file));
	// 		continue;
	// 	}
	// 	if(buf[0] == 'I' || buf[0] == 'D' || buf[0] == 'A' || buf[0] == 'T')
	// 	{
	// 		printf("buf = %s\n", buf[0]);
	// 		printf("file cur pos = %ld\n", ftell(file));
	// 		continue;
	// 	}
	// 	if(buf[0] == 'p' || buf[0] == 'H' || buf[0] == 'Y' || buf[0] == 's')
	// 	{
	// 		printf("buf = %s\n", buf[0]);
	// 		printf("file cur pos = %ld\n", ftell(file));
	// 		continue;
	// 	}
	// 	if(buf[0] == 'I' || buf[0] == 'E' || buf[0] == 'N' || buf[0] == 'D')
	// 	{
	// 		printf("buf = %s\n", buf[0]);
	// 		printf("file cur pos = %ld\n", ftell(file));
	// 		continue;
	// 	}
	// }
	
	// i_seek = fseek(file, 8, SEEK_SET);
	// if(i_seek != 0)
	// {
	// 	printf("2 PNG file not read\n");
	// 	return 1;
	// }
	// flag=8;
	
	while(!feof(file))
	{
		// if(cur_pos > file_size)
		// {
		// 	break;
		// }

		length = 0;

		//printf("cur pos file = %d\n", ftell(file));
		
		i_read = fread(buf, 1, 4, file); // read chunk data length
		if(i_read != 4)
		{
			printf("3 PNG file not read %d\n", i_read);
			return 1;
		}
		flag+=4;

		printf("buf[0] = %d  ", buf[0]);
		printf("buf[1] = %d  ", buf[1]);
		printf("buf[2] = %d  ", buf[2]);
		printf("buf[3] = %d  ", buf[3]);
		
		//printf("file cur pos after len = %ld\n", ftell(file));

		if (!is_bigendian())
		{
			int tmp = 0;
			for(int i = 3; i >= 0; i--)
			{
				//printf("buf[%d] = %d  ", i, (int)buf[i]);

				//buf[i] = (int)buf[i] - 48;

				if((int)buf[i] < 0)
				{
					tmp = (-2)*((int)buf[i]);
					if((int)buf[i] == -128)
					{
						tmp--;
					}
				}
				else
				{
					tmp = (int)buf[i];
				}
				
				//printf("tmp = %d  ", tmp);
				length += tmp*pow(256, i-3);
			}
		}
		else
		{
			length = (int)buf;
		}

		printf("length = %d\n", length);
		

		i_read = fread(buf, 4, 1, file); // read chunk type		
		if(i_read != 1)
		{
			printf("4 PNG file not read, i_read = %d\n", i_read);
			return 1;
		}
		flag+=4;
		//printf("chunk type = %s\n", buf);
		
		
		if(isCriticalChunk(buf))
		{
			count_chunks++;
			printf("%dth chunk = %s\n", count_chunks, buf);
		}


		if(strcmp(buf, "IDAT")==0)
		{
			count_compr = length;

			if(count_compr > 0)
			{
				data_compr = new char[count_compr];

				i_read = fread(buf, length, 1, file);

				if(i_read != 1)
				{
					printf("5 PNG file not read\n");
					return 1;
				}
				flag+=length;


				//for(int i = 0; i < count_compr; i++)
				//{
				if (!is_bigendian())
				{
					for(int i = 0; i < count_compr; i+=4)
					{
						swap(buf[i], buf[i+3]);
						swap(buf[i+1], buf[i+2]);

						for(int j = 0; j < 4; j++)
						{
							data_compr[i+j] = buf[i+j];	
						}
					}
				}
				else
				{
					for(int i = 0; i < count_compr; i++)
					{
						data_compr[i] = buf[i];
					}
				}

				i_seek = fseek(file, 4, SEEK_CUR);	
				if(i_seek != 0)
				{
					printf("6 PNG file not read\n");
					return 1;
				}
				flag+=4;			
			}
		}
		else
		{
			
			i_seek = fseek(file, length+4, SEEK_CUR);	
			
									
			if(i_seek != 0)
			{
				printf("7 PNG file not read\n");
				return 1;
			}
			flag+=(length+4);	
			cur_pos = ftell(file);
			//printf("flag = %d, cur pos = %d\n", flag, cur_pos);
			if(flag != cur_pos)
			{
				//printf("Error of read file\n");
			}
		}
	}

		

	fclose(file);

	// printf("count of critical chunk = %d\n", count_chunks);



	printf("==========================\n");
	
	printf("count_compr = %d\n", count_compr);

	if(count_compr > 0)
	{
		//printf("data_compr[0] = %d\n", (int)data_compr[0]);
		//printf("data_compr[1] = %d\n", (int)data_compr[1]);
		for(int i = 7; i < count_compr-4; i++)
		{
			//printf("data_compr[%d] = %d\n", i, (int)data_compr[i]);
		}
		int res_uncompr = uncompress((Bytef*)data_uncompr, (uLong*)&count_uncompr, (const Bytef*)buf, (uLong)length);
		printf("res_uncompr = %d\n", res_uncompr);

		delete data_compr;
	}
	

	printf("==========================\n");


	 


	//  int err;
	//  char example[] = "hello";
 //     uLong len = strlen(example)+1;

 //     uLong comprLen = 10*len;
 //     uLong uncomprLen;
 //     Byte *compr = new Byte[comprLen];
	//  Byte *uncompr;
  
 //     err = compress(compr, &comprLen, (const Bytef*)example, len);

 //     printf("err compr = %d, compr len = %d, initial len = %d\n", err, comprLen, len);
 //     //CHECK_ERR(err, "compress");
  
 //     //strcpy((char*)uncompr, "garbage");
  
 //     err = uncompress(uncompr, &uncomprLen, compr, comprLen);

 //     printf("err uncompr = %d, uncompr len = %d\n", err, uncomprLen);
 //     //CHECK_ERR(err, "uncompress");
  
    //  if (strcmp((char*)uncompr, hello))
    //  {
    //     fprintf(stderr, "bad uncompress\n");
    //     exit(1);
    // } else {
    //      printf("uncompress(): %s\n", (char *)uncompr);
    //  }

    

	return 0;
}