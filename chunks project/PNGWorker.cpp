#include "PNGWorker.h"
#include <stdexcept>

PNGWorker::PNGWorker()
{
	
}

int PNGWorker::getInt(char buf)
{
	if(buf < 0)
	{
		return (256 + buf);
	}
	return buf;
}

void PNGWorker::getSizeReducedImage(int w, int h, int numRedImg, int &col, int &row)
{
	switch(numRedImg)
	{
		case 1:
			col = w/8;
			if(w%8 > 0) col++;			
			row = h/8;
			if(h%8 > 0) row++;
			break;

		case 2:
			col = w/8;
			if(w%8 > 4) col++;			
			row = h/8;
			if(h%8 > 0) row++;
			break;

		case 3:
			col = w/4;
			if(w%4 > 0) col++;
			row = h/8;
			if(h%8 > 4) row++;
			break;

		case 4:
			col = 2*(w/8);
			if(w%8 > 2 && w%8 < 7) col++;
			else if(w%8 > 6) col += 2;
			row = h/4;
			if(h%4 > 0) row++;
			break;

		case 5:
			col = 5*(w/8);
			if(w%8 > 0 && w%8 < 3) col++;
			else if(w%8 > 2 && w%8 < 5) col += 2;
			else if(w%8 > 4 && w%8 < 7) col += 3;
			else if(w%8 > 6) col += 4;
			row = 2*(h/8);
			if(h%8 > 2 && h%8 < 7) row++;
			else if(h%8 > 6) row += 2;
			break;	

		case 7:
			col = w;
			row = h/2;
			break;		
	}
}

char* PNGWorker::getImage(char *filt_data, int data_size, int file_w, int file_h)
{
	char *image = new char[file_w*file_h];

	int i = 7;
	int cur_red_image = 1;
	while(1)
	{
		int filt_type = filt_data[i];

		int col = 0, row = 0;
		getSizeReducedImage(file_w, file_h, cur_red_image, col, row);
		

		switch(filt_type)
		{
			case 0:
				for(int r = 0; r < row; r++)
				{
					for(int c = 0; c < col; c++)
					{
						int orig = filt_data[i + 1 + r*col + c];
						//setPixel(char *image, );
					}
				}
				break;

			case 1:
				break;

			case 2:
				break;

			case 3:
				break;

			case 4:
				break;
		}
	}
}