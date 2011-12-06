#include "PNGReader.h"
#include <iostream>

#include <stdlib.h>
#include <stdexcept>


int PNGReader::getIntInRightOrder(char *buf)
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

unsigned int PNGReader::readInt()
{
	char buf[] = "0000";
	
	file.read(buf, 4);
	if(file.gcount() != 4)
	{
		throw runtime_error("111 File not read");
	}

	return getIntInRightOrder(buf);
}


void PNGReader::getIHDRData()
{
	img_width        = readInt();
	img_height       = readInt();
	bit_depth        = readByte();
	colour_type      = readByte();
	compr_method     = readByte();
	filter_method    = readByte();
	interlace_method = readByte();
}


char* PNGReader::getIDATData(unsigned int &data_length)
{
	char *idat_tmp_data = new char[data_length];

	file.read(idat_tmp_data, data_length);
	int count = file.gcount();
	if(count != (int)data_length)
	{
		throw runtime_error("123 PNG file not read");
	}
	
	return idat_tmp_data;
}

void PNGReader::doInitData()
{
	img_width = 0;
	img_height = 0;

	unsigned int data_length = 0;
	char buf[] = "0000";	

	file.open(file_name.c_str(), fstream::binary);

	if(file.good() != true)
	{
		throw runtime_error("PNG file not open");
	}
		
	file.seekg(8, ios::beg);
		
	file.read(buf, 4);


	while(file.gcount() == 4)
	{
		data_length = getIntInRightOrder(buf);
				
		file.read(buf, 4);	
		if(file.gcount() != 4)
		{
	 		throw runtime_error("PNG file not read");
		}
				
		if(strcmp(buf, "IHDR")==0)
		{
			getIHDRData();
			file.seekg(4, ios::cur);	
		}
		else
		if(strcmp(buf, "IDAT")==0)
		{
			char *idat_tmp_buf = getIDATData(data_length);
				
			for(unsigned int i = 0; i < data_length; i++)
			{
				v_idat_total_data.push_back(idat_tmp_buf[i]);
			}
			file.seekg(4, ios::cur);
			
			delete[] idat_tmp_buf;
		}
		else
		{
			file.seekg(data_length+4, ios::cur);	
		}
		

		file.read(buf, 4);
	}		

	file.close();
}


PNGReader::PNGReader(string file_name)
{
	this->file_name = file_name;
	file.open(file_name.c_str(), fstream::binary);
	if(file.good() != true)
	{
		throw runtime_error("PNG file not open");
	}
	file.close();
}

char* PNGReader::getPNGData(unsigned int &data_length)
{
	doInitData();

	data_length = v_idat_total_data.size();
	char *idat_data = new char[data_length];
	for(unsigned int i = 0; i < data_length; i++)
	{
		idat_data[i] = v_idat_total_data[i];
	}
	return idat_data;
}


void PNGReader::getSizeSmallImage(int cur_small_img, int &col, int &row)
{
	switch(cur_small_img)
	{
		case 1:
			col = img_width/8; if(img_width%8 >= 1) col++;			
			row = img_height/8; if(img_height%8 >= 1) row++;
			break;

		case 2:
			col = img_width/8; if(img_width%8 >= 5) col++;			
			row = img_height/8; if(img_height%8 >= 1) row++;
			break;

		case 3:
			col = img_width/4; if(img_width%4 >= 1) col++;
			row = img_height/8; if(img_height%8 >= 5) row++;
			break;

		case 4:
			col = img_width/4; if(img_width%4 >= 3) col++;
			row = img_height/4; if(img_height%4 >= 1) row++;
			break;

		case 5:
			col = img_width/2; if(img_width%2 >= 1) col++;
			row = img_height/4; if(img_height%4 >= 3) row++;
			break;	

		case 6:
			col = img_width/2;
			row = img_height/2; if(img_height%2 >= 1) row++;
			break;

		case 7:
			col = img_width;
			row = img_height/2;
			break;		
	}
}


void PNGReader::getCoordPixel(int cur_small_img, int cur_row, int cur_col, int &i, int &j)
{
	switch(cur_small_img)
	{
		case 1:
			i = 8*cur_row;     j = 8*cur_col;
			break;

		case 2:
			i = 8*cur_row + 4; j = 8*cur_col;
			break;

		case 3:
			i = 4*cur_row;     j = 8*cur_col + 4;
			break;

		case 4:
			i = 4*cur_row;     j = 4*cur_col + 2;
			break;

		case 5:
			if(cur_row%2 == 0)
			{
				i = 4*cur_row + 2;
			}
			else
			{
				i = 4*(cur_row - 1) + 6;
			}			     

			j = 2*cur_col;
			break;
			
		case 6:
			i = 2*cur_row;     j = 2*cur_col + 1;
			break;		

		case 7:
			i = 2*cur_row + 1; j = cur_col;       
			break;		
	}
}

int PNGReader::getCurRow(int cur_small_img, int general_index)
{
	int row, col;

	for(int i = 1; i < cur_small_img; i++)
	{
		getSizeSmallImage(i, col, row);
		general_index -= row;
	}

	return general_index;
}


unsigned int PNGReader::getPaethPredictor(unsigned int prev, unsigned int up, unsigned int diag)
{
	unsigned int Pr;

	int p = prev + up - diag;
    unsigned int p_prev = abs(p - prev);
    unsigned int p_up = abs(p - up);
    unsigned int p_diag = abs(p - diag);
    if(p_prev <= p_up && p_prev <= p_diag)
    {
    	Pr = prev;
    } 
    else
    {
	    if(p_up <= p_diag)
	    {
	    	Pr = up;
	    } 
    	else
    	{
	    	Pr = diag;
	    }
	}
	
    return Pr;
}

void PNGReader::doDeFilteringType0(int j_count, int j_delta, int cur_small_img, int cur_row, int i, ScanLine scanline, Image *image)
{
	int cur_col = 0;
	int pix_i, pix_j;

	for(int j = 2; j <= 2 + j_count - j_delta + 1; j+=j_delta)
	{
		if(interlace_method == 0)
		{
			pix_i = i;
			pix_j = (j-2)/j_delta;
		}
		else
		if(interlace_method == 1)
		{
			getCoordPixel(cur_small_img, cur_row, cur_col, pix_i, pix_j);
		}				

		int alpha = 0;
		if(colour_type == 6)
		{
			alpha = getUnsignedInt(scanline[j+3]);
		}
		
		image->setPixel(pix_i, pix_j, getUnsignedInt(scanline[j]),
			                		  getUnsignedInt(scanline[j+1]),
			                		  getUnsignedInt(scanline[j+2]),
			                		  alpha);
			       
		cur_col++;	
	}
}

void PNGReader::doDeFiltering(V_ScanLines &v_scanlines, Image *image)
{
	if(filter_method != 0)
	{
		throw runtime_error("PNG file has incorrect data");
	}
	
	int scanlines_count = v_scanlines.size();
	

	for(int i = 0; i < scanlines_count; i++)
	{
		int j_count, j_delta, alpha = 0;
		if(colour_type == 2)
		{					
			j_delta = 3;
		}
		else
		if(colour_type == 6)
		{
			j_delta = 4;
		}

		j_count = v_scanlines[i].size() - 2;

		int filt_type = getUnsignedInt(v_scanlines[i][1]);
		
		int cur_small_img = v_scanlines[i][0];
		int cur_col = 0, cur_row = 0;


		if(interlace_method == 1)
		{
			cur_row = getCurRow(cur_small_img, i);
		}
		
		int j, pix_i, pix_j, pix_prev_i, pix_prev_j, pix_up_i, pix_up_j, pix_diag_i, pix_diag_j;
		unsigned int prevR, prevG, prevB, prevA, diagR, diagG, diagB, diagA;
		unsigned int upR, upG, upB, upA;
		int condition;

			
		switch(filt_type)
		{
			case 0:
				doDeFilteringType0(j_count, j_delta, cur_small_img, cur_row, i, v_scanlines[i], image);
				break;

			case 1:
				if(interlace_method == 0)
				{
					pix_i = i;
					pix_j = 0;
				}
				else
				if(interlace_method == 1)
				{
					getCoordPixel(cur_small_img, cur_row, 0, pix_i, pix_j);
				}

				

				if(colour_type == 6)
				{
					alpha = getUnsignedInt(v_scanlines[i][5]);
				}
				
				image->setPixel(pix_i, pix_j, getUnsignedInt(v_scanlines[i][2]),
					                  		  getUnsignedInt(v_scanlines[i][3]),
					                 		  getUnsignedInt(v_scanlines[i][4]),
					                  		  alpha);

				cur_col = 1;
				for(j = 1 + j_delta; j <= 2 + j_count - j_delta + 1; j+=j_delta)
				{
					if(interlace_method == 0)
					{
						pix_i = i;
						pix_j = (j-1)/j_delta;

						pix_prev_i = i;
						pix_prev_j = (j-1)/j_delta - 1;
					}
					else
					if(interlace_method == 1)
					{
						getCoordPixel(cur_small_img, cur_row, cur_col, pix_i, pix_j);
						getCoordPixel(cur_small_img, cur_row, cur_col-1, pix_prev_i, pix_prev_j);
					}

					if(colour_type == 6)
					{
						alpha = getIntSum(getUnsignedInt(v_scanlines[i][j+4]), image->getPixel(pix_prev_i,pix_prev_j).getAlpha());
					}

					image->setPixel(pix_i, pix_j,
					                   getIntSum(getUnsignedInt(v_scanlines[i][j+1]), image->getPixel(pix_prev_i,pix_prev_j).getRed()),
					                   getIntSum(getUnsignedInt(v_scanlines[i][j+2]), image->getPixel(pix_prev_i,pix_prev_j).getGreen()),
					                   getIntSum(getUnsignedInt(v_scanlines[i][j+3]), image->getPixel(pix_prev_i,pix_prev_j).getBlue()),
					                   alpha);
					
					cur_col++;
				}
					
				break;						

			case 2:		
				if(interlace_method == 0)
				{
					pix_i = i;
					pix_j = 0;
				}
				else
				if(interlace_method == 1)
				{
					getCoordPixel(cur_small_img, cur_row, 0, pix_i, pix_j);
				}

				if(colour_type == 6)
				{
					alpha = getUnsignedInt(v_scanlines[i][5]);
				}

				image->setPixel(pix_i, pix_j, getUnsignedInt(v_scanlines[i][2]),
					                  		  getUnsignedInt(v_scanlines[i][3]),
					                  		  getUnsignedInt(v_scanlines[i][4]),
					                  		  alpha);

				cur_col = 1;
				for(j = 1 + j_delta; j <= 2 + j_count - j_delta + 1; j+=j_delta)
				{
					if(interlace_method == 0)
					{
						condition = i;
					}
					else
					{
						condition = cur_row;
					}
					
					if(condition/*i*/ == 0)
					{
						if(interlace_method == 0)
						{
							pix_i = 0;
							pix_j = (j-1)/j_delta;
						}
						else
						if(interlace_method == 1)
						{
							getCoordPixel(cur_small_img, cur_row, cur_col, pix_i, pix_j);
						}

						if(colour_type == 6)
						{
							alpha = getUnsignedInt(v_scanlines[i][j+4]);
						}

						image->setPixel(pix_i, pix_j, getUnsignedInt(v_scanlines[i][j+1]),
							                          getUnsignedInt(v_scanlines[i][j+2]),
							                          getUnsignedInt(v_scanlines[i][j+3]),
							                          alpha);
						
					}
					else
					{
						if(interlace_method == 0)
						{
							pix_i = i;
							pix_j = (j-1)/j_delta;

							pix_up_i = i-1;
							pix_up_j = (j-1)/j_delta;
						}
						else
						if(interlace_method == 1)
						{
							getCoordPixel(cur_small_img, cur_row, cur_col, pix_i, pix_j);
							getCoordPixel(cur_small_img, cur_row-1, cur_col, pix_up_i, pix_up_j);
						}

						if(colour_type == 6)
						{
							alpha = getIntSum(getUnsignedInt(v_scanlines[i][j+4]), image->getPixel(pix_up_i,pix_up_j).getBlue());
						}

						image->setPixel(pix_i, pix_j,
						                   getIntSum(getUnsignedInt(v_scanlines[i][j+1]), image->getPixel(pix_up_i,pix_up_j).getRed()),
						                   getIntSum(getUnsignedInt(v_scanlines[i][j+2]), image->getPixel(pix_up_i,pix_up_j).getGreen()),
						                   getIntSum(getUnsignedInt(v_scanlines[i][j+3]), image->getPixel(pix_up_i,pix_up_j).getBlue()),
						                   alpha);
					}	
					cur_col++;				  							
				}
				break;

			case 3:				
				if(interlace_method == 0)
				{
					condition = i;
				}
				else
				{
					condition = cur_row;
				}

				if(condition/*i*/ == 0)
				{
					upR = upG = upB = upA = 0;
				}
				else
				{
					if(interlace_method == 0)
					{
						pix_i = i;
						pix_j = 0;

						pix_up_i = i-1;
						pix_up_j = (j-1)/j_delta;
					}
					else
					if(interlace_method == 1)
					{
						getCoordPixel(cur_small_img, cur_row-1, 0, pix_up_i, pix_up_j);
					}
				
					upR = image->getPixel(pix_up_i,pix_up_j).getRed();
					upG = image->getPixel(pix_up_i,pix_up_j).getGreen();
					upB = image->getPixel(pix_up_i,pix_up_j).getBlue();
					upA = image->getPixel(pix_up_i,pix_up_j).getAlpha();
				}

				if(colour_type == 6)
				{
					alpha = getUnsignedInt(v_scanlines[i][5]);
				}

				image->setPixel(pix_i, pix_j, getIntSum(getUnsignedInt(v_scanlines[i][2]), upR/2),
					                  		  getIntSum(getUnsignedInt(v_scanlines[i][3]), upG/2),
					                  		  getIntSum(getUnsignedInt(v_scanlines[i][4]), upB/2),
					                  		  getIntSum(alpha, upA/2));

				cur_col = 1;
				for(j = 1 + j_delta; j <= 2 + j_count - j_delta + 1; j+=j_delta)
				{
					if(interlace_method == 0)
					{
						condition = i;
					}
					else
					{
						condition = cur_row;
					}

					if(condition/*i*/ == 0)
					{
						upR = upG = upB = upA = 0;
					}
					else
					{
						if(interlace_method == 0)
						{
							pix_up_i = i-1;
							pix_up_j = (j-1)/j_delta;
						}
						else
						if(interlace_method == 1)
						{
							getCoordPixel(cur_small_img, cur_row-1, cur_col, pix_up_i, pix_up_j);
						}

						upR = image->getPixel(pix_up_i,pix_up_j).getRed();
						upG = image->getPixel(pix_up_i,pix_up_j).getGreen();
						upB = image->getPixel(pix_up_i,pix_up_j).getBlue();
						upA = image->getPixel(pix_up_i,pix_up_j).getAlpha();
					}

					if(interlace_method == 0)
					{
						pix_i = i;
						pix_j = (j-1)/j_delta;

						pix_prev_i = i;
						pix_prev_j = (j-1)/j_delta - 1;
					}
					else
					if(interlace_method == 1)
					{
						getCoordPixel(cur_small_img, cur_row, cur_col, pix_i, pix_j);
						getCoordPixel(cur_small_img, cur_row, cur_col-1, pix_prev_i, pix_prev_j);
					}

					if(colour_type == 6)
					{
						alpha = getIntSum(getUnsignedInt(v_scanlines[i][j+4]), (upA + image->getPixel(pix_prev_i, pix_prev_j).getAlpha()));
					}

					image->setPixel(pix_i, pix_j, getIntSum(getUnsignedInt(v_scanlines[i][j+1]), (upR + image->getPixel(pix_prev_i, pix_prev_j).getRed())/2),
					                 	  getIntSum(getUnsignedInt(v_scanlines[i][j+2]), (upG + image->getPixel(pix_prev_i, pix_prev_j).getGreen())/2),
					                  	  getIntSum(getUnsignedInt(v_scanlines[i][j+3]), (upB + image->getPixel(pix_prev_i, pix_prev_j).getBlue())/2),
					                  	  alpha);
					cur_col++;
				}
				
				break;

			case 4:	
				if(interlace_method == 0)
				{
					condition = i;
				}			
				else
				{
					condition = cur_row;
				}

				if(condition/*i*/ == 0)
				{
					upR = upG = upB = upA = 0;					
				}
				else
				{
					if(interlace_method == 0)
					{
						pix_up_i = i-1;
						pix_up_j = 0;
					}
					else
					if(interlace_method == 1)
					{
						getCoordPixel(cur_small_img, cur_row-1, 0, pix_up_i, pix_up_j);
					}

					upR = image->getPixel(pix_up_i, pix_up_j).getRed();
					upG = image->getPixel(pix_up_i, pix_up_j).getGreen();
					upB = image->getPixel(pix_up_i, pix_up_j).getBlue();
					upA = image->getPixel(pix_up_i, pix_up_j).getAlpha();
				}

				
				prevR = prevG = prevB = prevA = 0; // for j = 0
				diagR = diagG = diagB = diagA = 0;

				if(interlace_method == 0)
				{
					pix_i = i;
					pix_j = 0;
				}
				else
				if(interlace_method == 1)
				{
					getCoordPixel(cur_small_img, cur_row, cur_col, pix_i, pix_j);
				}

				if(colour_type == 6)
				{
					alpha = getIntSum(getUnsignedInt(v_scanlines[i][5]), getPaethPredictor(prevA, upA, diagA));
				}

				image->setPixel(pix_i, pix_j, getIntSum(getUnsignedInt(v_scanlines[i][2]), getPaethPredictor(prevR, upR, diagR)),
					                  getIntSum(getUnsignedInt(v_scanlines[i][3]), getPaethPredictor(prevG, upG, diagG)),
					                  getIntSum(getUnsignedInt(v_scanlines[i][4]), getPaethPredictor(prevB, upB, diagB)),
					                  alpha);

				cur_col = 1;
				for(j = 1 + j_delta; j <= 2 + j_count - j_delta + 1; j+=j_delta)
				{
					if(interlace_method == 0)
					{
						pix_i = i;
						pix_j = (j-1)/j_delta;

						pix_prev_i = i;
						pix_prev_j = (j-1)/j_delta - 1;

						if(interlace_method == 0)
						{
							condition = i;
						}
						else
						{
							condition = cur_row;
						}

						if(condition/*i*/ > 0)
						{
							pix_up_i = i-1;
							pix_up_j = (j-1)/j_delta;

							pix_diag_i = i-1;
							pix_diag_j = (j-1)/j_delta - 1;
						}
					}
					else
					if(interlace_method == 1)
					{
						getCoordPixel(cur_small_img, cur_row, cur_col, pix_i, pix_j);
						getCoordPixel(cur_small_img, cur_row, cur_col-1, pix_prev_i, pix_prev_j);

						if(interlace_method == 0)
						{
							condition = i;
						}
						else
						{
							condition = cur_row;
						}

						if(condition/*i*/ > 0)
						{
							getCoordPixel(cur_small_img, cur_row-1, cur_col, pix_up_i, pix_up_j);
							getCoordPixel(cur_small_img, cur_row-1, cur_col-1, pix_diag_i, pix_diag_j);
						}
					}

					prevR = image->getPixel(pix_prev_i, pix_prev_j).getRed();
					prevG = image->getPixel(pix_prev_i, pix_prev_j).getGreen();
					prevB = image->getPixel(pix_prev_i, pix_prev_j).getBlue();

					if(colour_type == 6)
					{
						prevA = image->getPixel(pix_prev_i, pix_prev_j).getAlpha();
					}

					upR = upG = upB = upA = diagR = diagG = diagB = diagA = 0;

					if(interlace_method == 0)
					{
						condition = i;
					}
					else
					{
						condition = cur_row;
					}
					
					if(condition/*i*/ > 0)
					{
						upR = image->getPixel(pix_up_i, pix_up_j).getRed();
						upG = image->getPixel(pix_up_i, pix_up_j).getGreen();
						upB = image->getPixel(pix_up_i, pix_up_j).getBlue();

						diagR = image->getPixel(pix_diag_i, pix_diag_j).getRed();
						diagG = image->getPixel(pix_diag_i, pix_diag_j).getGreen();
						diagB = image->getPixel(pix_diag_i, pix_diag_j).getBlue();

						if(colour_type == 6)
						{
							upA = image->getPixel(pix_up_i, pix_up_j).getAlpha();
							diagA = image->getPixel(pix_diag_i, pix_diag_j).getAlpha();
						}
					}		

					if(colour_type == 6)
					{
						alpha = getIntSum(getUnsignedInt(v_scanlines[i][j+4]), getPaethPredictor(prevA, upA, diagA));
					}

					image->setPixel(pix_i, pix_j, getIntSum(getUnsignedInt(v_scanlines[i][j+1]),
									     		  		    getPaethPredictor(prevR, upR, diagR)
												           ),
					                 	          getIntSum(getUnsignedInt(v_scanlines[i][j+2]),
					                 	                    getPaethPredictor(prevG, upG, diagG)
					                 	                   ),
					                  	  		  getIntSum(getUnsignedInt(v_scanlines[i][j+3]),
					                  	  		            getPaethPredictor(prevB, upB, diagB)
					                  	  		           ),
					                  	          alpha);
					cur_col++;
				}
					
				break;
		}
	}
}



void PNGReader::getScanLines(V_ScanLines &v_scanlines, char *decompr_data, int decompr_data_len)
{
	if(interlace_method == 0)
	{
		int scan_size;
		if(colour_type == 2)
		{
			scan_size = img_width*3 + 1;
		}
		else
		if(colour_type == 6)
		{
			scan_size = img_width*4 + 1;
		}

		for(int i = 0; i < img_height; i++)
		{
			ScanLine scanline;
			scanline.push_back(0);
			scanline.push_back(getUnsignedInt(decompr_data[i*scan_size])); // put filt type

			for(int j = 1; j < scan_size; j++)
			{
				scanline.push_back(getUnsignedInt(decompr_data[i*scan_size + j]));	
			}

			v_scanlines.push_back(scanline);
		}
	}
	else
	if(interlace_method == 1)
	{
		ScanLine scanline;

		int n = 0, col, row;
		while(n < decompr_data_len)
		{
			for(int s = 1; s <= 7; s++)
			{	
				getSizeSmallImage(s, col, row);
				
				for(int i = 0; i < row; i++)
				{
					scanline.clear();
					scanline.push_back(s);

					scanline.push_back(getUnsignedInt(decompr_data[n])); // put filt type

					for(int j = 1; j <= col; j++)
					{
						if(colour_type == 2)
						{
							scanline.push_back(getUnsignedInt(decompr_data[n+1]));
							scanline.push_back(getUnsignedInt(decompr_data[n+2]));
							scanline.push_back(getUnsignedInt(decompr_data[n+3]));

							n += 3;
						}
						else
						if(colour_type == 6)
						{
							scanline.push_back(getUnsignedInt(decompr_data[n+1]));
							scanline.push_back(getUnsignedInt(decompr_data[n+2]));
							scanline.push_back(getUnsignedInt(decompr_data[n+3]));
							scanline.push_back(getUnsignedInt(decompr_data[n+4]));

							n += 4;							
						}
					}
					n++;

					v_scanlines.push_back(scanline);					
				}
			}			
		}
	}
}


int PNGReader::getDecomprDataLen()
{
	int res = 0;
		
	int count;
	if(colour_type == 2)
	{
		count = 3;
	}
	else
	if(colour_type ==6)
	{
		count = 4;
	}

	if(interlace_method == 0)
	{
		res = (count*img_width + 1)*img_height;	
	}
	else
	if(interlace_method == 1)
	{
		int row, col;

		for(int s = 1; s <=7; s++)
		{
			getSizeSmallImage(s, col, row);
			res += row + count*col*row;
		}
	}

	return res;
}


Image* PNGReader::getImageStruct()
{
	unsigned int compr_data_len = 0;
	char *compr_data = getPNGData(compr_data_len);

	MyDecompressor decopressor = MyDecompressor();
	unsigned int decompr_data_len = getDecomprDataLen();
	
	char *decompr_data = decopressor.getDecomprData(decompr_data_len, compr_data, compr_data_len);

	//for(int i = (103*3+1)*0; i < (103*3+1)*1; i++)
	{
		//cout<<"d["<<350*4+2<<"] = "<<getUnsignedInt(decompr_data[350*4+2])<<"  ";
	}

	
	V_ScanLines v_scanlines;
	getScanLines(v_scanlines, decompr_data, decompr_data_len);

	delete decompr_data;


	Image *image = new Image(img_width, img_height);
	doDeFiltering(v_scanlines, image);


	return image;
}
