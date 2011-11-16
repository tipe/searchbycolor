#include "PNGWorker.h"
#include <stdexcept>
#include <cstdlib>

PNGWorker::PNGWorker(unsigned int img_h, unsigned int img_w)
{
	*image = new Pixel[img_h];
	for(int i = 0; i < img_h; i++)
	{
		image[i] = new Pixel[img_w];
	}
}

PNGWorker::~PNGWorker()
{
	if(image != NULL)
	{
		delete image;
	} 
}

int PNGWorker::getInt(char buf)
{
	if(buf < 0)
	{
		return (256 + buf);
	}
	return buf;
}

void PNGWorker::getSizeReducedImage(int h, int w, int numRedImg, int &col, int &row)
{
	switch(numRedImg)
	{
		case 1:
			col = (w+1)/8; if((w+1)%8 >= 1) col++;			
			row = (h+1)/8; if((h+1)%8 >= 1) row++;
			break;

		case 2:
			col = (w+1)/8; if((w+1)%8 >= 5) col++;			
			row = (h+1)/8; if((h+1)%8 >= 1) row++;
			break;

		case 3:
			col = (w+1)/4; if((w+1)%4 >= 1) col++;
			row = (h+1)/8; if((h+1)%8 >= 5) row++;
			break;

		case 4:
			col = (w+1)/4; if((w+1)%4 >= 3) col++;
			row = (h+1)/4; if((h+1)%4 >= 1) row++;
			break;

		case 5:
			col = (w+1)/2; if((w+1)%2 >= 1) col++;
			row = (h+1)/4; if((h+1)%4 >= 3) row++;
			break;	

		case 6:
			col = (w+1)/2;
			row = (h+1)/2; if((h+1)%2 >= 1) row++;
			break;

		case 7:
			col = w;
			row = h/2;
			break;		
	}
}

void getCoordPixel(int cur_red_image, int cur_row, int cur_col, int &i, int &j)
{
	switch(cur_red_image)
	{
		case 1:
			i = 8*cur_row;     j =8*cur_col;
			break;

		case 2:
			i = 8*cur_row + 4; j = 8*cur_col;
			break;

		case 3:
			i = 4*cur_row;     j = 8*cur_col + 4;;
			break;

		case 4:
			i = 4*cur_row;     j = 4*cur_col + 2;;
			break;

		case 5:
			i = 8*cur_row;     j = 8*cur_col;
			break;
			
		case 6:
			i = 2*cur_row;     j = 2*cur_col + 1;
			break;		

		case 7:
			i = cur_row;       j = 2*cur_col + 1;
			break;		
	}
}


char* PNGWorker::getImage(char *filt_data, int data_size, int file_w, int file_h)
{
	char *image = new char[file_w*file_h];

	int i = 7;
	int cur_red_image = 1;

	while(cur_red_image <= 7)
	{
		int filt_type = filt_data[i];

		int col = 0, row = 0;
		getSizeReducedImage(file_w, file_h, cur_red_image, col, row);

		int cur_col = 0, cur_row = 0;
		
		for(int r = 0; r < row; r++)
		{
			for(int c = 0; c < col; c++)
			{
				int orig_r, orig_g, orig_b, orig_a;

				switch(filt_type)
				{
					case 0:
						orig_r = filt_data[i + cur_red_image + r*col + c];						
						orig_g = filt_data[i + cur_red_image + r*col + c + 1];
						orig_b = filt_data[i + cur_red_image + r*col + c + 2];
						orig_a = filt_data[i + cur_red_image + r*col + c + 3];
						break;

					case 1:
						int prev_r = 0, prev_g = 0, prev_b = 0, prev_a = 0;

						for(int color = 0; color < 4; color++)
						{
							if(cur_col > 0)
							{
								int i = 0, j = 0;
								getCoordPixel(cur_red_image, cur_row, cur_col-1, i, j);
								prev = image[i][j];
							}
							orig = filt_data[i + cur_red_image + r*col + c] + prev;
							}
														
						break;

					case 2:
						int prev_r = 0, prev_g = 0, prev_b = 0, prev_a = 0;
						if(cur_row > 0)
						{
							int i = 0, j = 0;
							getCoordPixel(cur_red_image, cur_row-1, cur_col, i, j);
							prev = image[i][j];
						}
						orig = filt_data[i + cur_red_image + r*col + c] + prev;	
						break;

					case 3:
						int prev_left = 0;
						int prev_up = 0;
						if(cur_col > 0)
						{
							int i = 0, j = 0;
							getCoordPixel(cur_red_image, cur_row, cur_col-1, i, j);
							prev_left = image[i][j];
						}
						if(cur_row > 0)
						{
							int i = 0, j = 0;
							getCoordPixel(cur_red_image, cur_row-1, cur_col, i, j);
							prev_up = image[i][j];
						}
						orig = filt_data[i + cur_red_image + r*col + c] + (prev_left + prev_up)/2;
						break;

					case 4:
						int prev_left = 0;
						int prev_up_left = 0;
						int prev_up = 0;
						if(cur_col > 0)
						{
							int i = 0, j = 0;
							getCoordPixel(cur_red_image, cur_row, cur_col-1, i, j);
							prev_left = image[i][j];
						}
						if(cur_col > 0 && cur_row > 0)
						{
							int i = 0, j = 0;
							getCoordPixel(cur_red_image, cur_row-1, cur_col-1, i, j);
							prev_up_left = image[i][j];
						}
						if(cur_row > 0)
						{
							int i = 0, j = 0;
							getCoordPixel(cur_red_image, cur_row-1, cur_col, i, j);
							prev_up = image[i][j];
						}

						int p = prev_left + prev_up - prev_up_left;
    					int pa = abs(p - prev_left);
						int pb = abs(p - prev_up);
						int pc = abs(p - prev_up_left);
						int pr;
						if(pa <= pb && pa <= pc)
						{
						    Pr = prev_left;
						}
						else if(pb <= pc)
						{
						  	Pr = prev_up;
						}
						else
						{
						    Pr = prev_up_left;
						}

						orig = filt_data[i + cur_red_image + r*col + c] + Pr;
						break;
				}

				int i = 0;
				int j = 0;
				getCoordPixel(cur_red_image, cur_row, cur_col, i, j)
				image[i][j] = orig;
			}
		}

		cur_red_image++;
	}
}