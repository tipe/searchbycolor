#ifndef PNGREADER_H
#define PNGREADER_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <string>

#include "MyDecompressor.h"
#include "Image.h"

using namespace std;



typedef std::vector<char> V_IDATData;

typedef vector<unsigned int> ScanLine;
typedef vector<ScanLine> V_ScanLines;


class PNGReader
{
private:
	ifstream file;
	string file_name;

	int img_width;
	int img_height;

	int bit_depth; 	
	int colour_type; 	
	int compr_method; 	
	int filter_method; 
	int interlace_method;

	V_IDATData v_idat_total_data;	

	void doInitData();
	
	void getIHDRData();
	char* getIDATData(unsigned int &data_length);

	char* getPNGData(unsigned int &data_length);

	void doDeFiltering(V_ScanLines &v_scanlines, Image *image);
	void doDeFilteringType0(int j_count, int j_delta, int cur_small_img, int cur_row, int i, ScanLine scanline, Image *image);
	void getScanLines(V_ScanLines &v_scanlines, char *decompr_data, int decompr_data_len);
	int getDecomprDataLen();

	void getSizeSmallImage(int cur_small_img, int &col, int &row);
	void getCoordPixel(int cur_small_image, int cur_row, int cur_col, int &i, int &j);
	int getCurRow(int cur_small_img, int general_index);

	unsigned int getPaethPredictor(unsigned int prev, unsigned int up, unsigned int diag);

public:
	PNGReader(char *file_name);
	
	Image* getImageStruct();
};

#endif //PNGREADER_H
