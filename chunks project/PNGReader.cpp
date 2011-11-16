#include "PNGReader.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdexcept>



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

int PNGReader::getInt(char buf)
{
	if(buf < 0)
	{
		return (256 + buf);
	}
	return (int)buf;
}

unsigned int PNGReader::getIntSum(unsigned int i1, unsigned int i2)
{
	unsigned int res = i1 + i2;

	if(res > 255)
	{
		return res - 256;
	}
	return res;
}


void PNGReader::getIHDRData()
{
	int cur_file_pos = file.tellg();
	char buf[] = "0000";

	file.read(buf, 4);
	if(file.gcount() != 4)
	{
		throw runtime_error("PNG file not read");
	}	
	img_width = getIntInRightOrder(buf);

	file.read(buf, 4);
	if(file.gcount() != 4)
	{
		throw runtime_error("PNG file not read");
	}	
	img_heigth = getIntInRightOrder(buf);

	file.read(buf, 1);
	if(file.gcount() != 1)
	{
		throw runtime_error("PNG file not read");
	}	
	bit_depth = getInt(buf[0]);

	file.read(buf, 1);
	if(file.gcount() != 1)
	{
		throw runtime_error("PNG file not read");
	}	
	colour_type = getInt(buf[0]);

	file.read(buf, 1);
	if(file.gcount() != 1)
	{
		throw runtime_error("PNG file not read");
	}	
	compr_method = getInt(buf[0]);

	file.read(buf, 1);
	if(file.gcount() != 1)
	{
		throw runtime_error("PNG file not read");
	}	
	filter_method = getInt(buf[0]);
	
	file.read(buf, 1);
	if(file.gcount() != 1)
	{
		throw runtime_error("PNG file not read");
	}	
	interlace_method = getInt(buf[0]);
	
	file.seekg(cur_file_pos, ios::beg);
}


char* PNGReader::getIDATData()
{
	int cur_file_pos = file.tellg();
	char *idat_tmp_data = new char[data_length];

	file.read(idat_tmp_data, data_length);
	if(file.gcount() != data_length)
	{
		throw runtime_error("PNG file not read");
	}
	file.seekg(cur_file_pos, ios::beg);
	
	return idat_tmp_data;
}

void PNGReader::doInitData()
{
	file.seekg(0, ios::end);
	img_size = file.tellg();
	file.seekg(0, ios::beg);

	img_width = 0;
	img_heigth = 0;

	data_length = 0;
	char buf[] = "0000";
	
		
	file.seekg(8, ios::beg);
		
	file.read(buf, 4);
	while(file.gcount() == 4)
	{
	 	data_length = getIntInRightOrder(buf);
				
		file.read(buf, 4);	
		if(file.gcount() != 1)
		{
	 		throw runtime_error("PNG file not read");
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
			
		file.seekg(data_length+4, ios::cur);	
	}		

	file.close();

	isInitData = 1;
}


PNGReader::PNGReader(char *file_name)
{
	file.open(file_name, fstream::binary);
	if(file.good() != true)
	{
		throw runtime_error("PNG file not open");
	}

	isInitData = 0;
}

char* PNGReader::getPNGData(int &data_length)
{
	doInitData();

	data_length = v_idat_total_data.size();
	char *idat_data = new char[data_length];
	for(int i = 0; i < data_length; i++)
	{
		idat_data[i] = v_idat_total_data[i];
	}
	return idat_data;
}

int PNGReader::getIndexFilteringType(int cur_small_img)
{
	// if(interlace_method == 1)
	// {
	// 	if(colour_type == 2)
	// 	{
	// 		return 0;
	// 	}
	// 	throw runtime_error("PNG file has unknown data");
	// }
	// else
	// {
		return 0;
	// }
}

void PNGReader::getSizeSmallImage(int cur_small_img, int &col, int &row)
{
	switch(cur_small_img)
	{
		case 1:
			col = (img_width+1)/8; if((img_width+1)%8 >= 1) col++;			
			row = (img_heigth+1)/8; if((img_heigth+1)%8 >= 1) row++;
			break;

		case 2:
			col = (img_width+1)/8; if((img_width+1)%8 >= 5) col++;			
			row = (img_heigth+1)/8; if((img_heigth+1)%8 >= 1) row++;
			break;

		case 3:
			col = (img_width+1)/4; if((img_width+1)%4 >= 1) col++;
			row = (img_heigth+1)/8; if((img_heigth+1)%8 >= 5) row++;
			break;

		case 4:
			col = (img_width+1)/4; if((img_width+1)%4 >= 3) col++;
			row = (img_heigth+1)/4; if((img_heigth+1)%4 >= 1) row++;
			break;

		case 5:
			col = (img_width+1)/2; if((img_width+1)%2 >= 1) col++;
			row = (img_heigth+1)/4; if((img_heigth+1)%4 >= 3) row++;
			break;	

		case 6:
			col = (img_width+1)/2;
			row = (img_heigth+1)/2; if((img_heigth+1)%2 >= 1) row++;
			break;

		case 7:
			col = img_width;
			row = img_heigth/2;
			break;		
	}
}

void doDeFilteringScanline(char *decompr_data, int filt_type, int begin, int end)
{
	// switch(filt_type)
	// {
	// 	case 0:
	// 		if(colour_type == 2)
	// 		{
	// 			if(interlace_method == 0)
	// 			{
	// 				for(int i = begin; i <= end; i++)
	// 				{
	// 					image.setPixel(i-1,);
	// 				}
	// 			}
	// 			else
	// 			{
	// 				throw runtime_error("PNG file has unknown data");
	// 			}
	// 		}
	// 		else
	// 		{
	// 			throw runtime_error("PNG file has unknown data");
	// 		}
			
			
	// 	case 1:

	// 	case 2:

	// 	case 3:

	// 	case 4:
	// }
}

void PNGReader::doDeFiltering(char *decompr_data, int decompr_data_len, Image *image)
{
	if(filter_method != 0)
	{
		throw runtime_error("PNG file has incorrect data");
	}

	if(interlace_method == 0)
	{
		for(int i = 0; i < img_heigth; i++)
		{
			int index, j_count, j_delta, alfa = 0;
			if(colour_type == 2)
			{					
				j_delta = 3;
			}
			if(colour_type == 6)
			{
				j_delta = 4;
			}

			j_count = img_width*j_delta;
			index = i*(j_count + 1);
			
				
			int filt_type = getInt(decompr_data[index]);
			int j;
			
			switch(filt_type)
			{
				case 0:
					for(j = 1; j <= j_count - j_delta + 1; j+=j_delta)
					{
						if(colour_type == 6)
						{
							alfa = getInt(decompr_data[index + j+3]);
						}	

						image->setPixel(i, j/j_delta,
							                getInt(decompr_data[index + j]),
							                getInt(decompr_data[index + j+1]),
							                getInt(decompr_data[index + j+2]),
							                alfa);
												
					}
					break;

				case 1:
					if(colour_type == 6)
					{
						alfa = getInt(decompr_data[index + 4]);
					}

					image->setPixel(i, 0, getInt(decompr_data[index + 1]),
						                  getInt(decompr_data[index + 2]),
						                  getInt(decompr_data[index + 3]),
						                  alfa);

						

					for(j = j_delta; j <= j_count - j_delta + 1; j+=j_delta)
					{
						if(colour_type == 6)
						{
							alfa = getIntSum(getInt(decompr_data[index + j+4]), image->getPixel(i, j/j_delta - 1).getB());
						}

						image->setPixel(i, j/j_delta,
						                   getIntSum(getInt(decompr_data[j+1]), image->getPixel(i, j/j_delta -1 ).getR()),
						                   getIntSum(getInt(decompr_data[j+2]), image->getPixel(i, j/j_delta - 1).getG()),
						                   getIntSum(getInt(decompr_data[j+3]), image->getPixel(i, j/j_delta - 1).getB()),
						                   alfa);
					}
						
					break;						

				case 2:		
					if(colour_type == 6)
					{
						alfa = getInt(decompr_data[index + 4]);
					}

					image->setPixel(i, 0, getInt(decompr_data[index + 1]),
						                  getInt(decompr_data[index + 2]),
						                  getInt(decompr_data[index + 3]),
						                  alfa);

					for(j = 1; j <= j_count - j_delta + 1; j+=j_delta)
					{
						if(i == 0)
						{
							if(colour_type == 6)
							{
								alfa = getInt(decompr_data[index + 4]);
							}

							image->setPixel(0, j/j_delta, getInt(decompr_data[index + 1]),
								                          getInt(decompr_data[index + 2]),
								                          getInt(decompr_data[index + 3]),
								                          alfa);
						}
						else
						{
							if(colour_type == 6)
							{
								alfa = getIntSum(getInt(decompr_data[index + j+3]), image->getPixel(i, j/j_delta - 1).getB());
							}
							image->setPixel(i, j/j_delta,
							                   getIntSum(getInt(decompr_data[j]), image->getPixel(i-1, j/j_delta).getR()),
							                   getIntSum(getInt(decompr_data[j+1]), image->getPixel(i-1, j/j_delta).getG()),
							                   getIntSum(getInt(decompr_data[j+2]), image->getPixel(i-1, j/j_delta).getB()),
							                   alfa);

						}
							
					}
					break;

				case 3:
					// for(j = 1; j <= j_count - j_delta + 1; j+=j_delta)
					// {
					// 	if(colour_type == 6)
					// 	{
					// 		alfa = getInt(decompr_data[index + 4]);
					// 	}

					// 	image->setPixel(0, j/j_delta, getInt(decompr_data[index + 1]),
					// 			                          getInt(decompr_data[index + 2]),
					// 			                          getInt(decompr_data[index + 3]),
					// 			                          alfa);
					// }

					
					
					break;

				case 4:
						
					break;
			}
		}

		return;
	}

	throw runtime_error("PNG file has unknown data");		
	

	if(interlace_method == 1)
	{
		for(int cur_small_img = 0; cur_small_img < 7; cur_small_img++)
		{
			int filt_type = getInt(decompr_data[getIndexFilteringType(cur_small_img)]);
		}
	}
}

Image* PNGReader::getImageStruct()
{
	int compr_data_len = 0;
	char *compr_data = getPNGData(compr_data_len);
	
	MyDecompressor decopressor = MyDecompressor(compr_data, compr_data_len);
	int decompr_data_len = 0;
	char *decompr_data = decopressor.getDecomprData(decompr_data_len);

	Image *image = new Image(img_width, img_heigth, colour_type);

	doDeFiltering(decompr_data, decompr_data_len, image);

	return image;
}