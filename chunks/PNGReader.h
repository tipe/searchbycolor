#ifndef PNGREADER_H
#define PNGREADER_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <string>

#include "MyDecompressor.h"
#include "ImageReader.h"




typedef std::vector<char> V_IDATData;

typedef std::vector<unsigned int> ScanLine;
typedef std::vector<ScanLine> V_ScanLines;


class PNGReader : public ImageReader
{
private:
	int bit_depth; 	
	int color_type; 	
	int compr_method; 	
	int filter_method; 
	int interlace_method;

	V_IDATData v_idat_total_data;	

	Pixel *color_table;

	void doInitData();
	
	void getIHDRData(std::ifstream &file);
	char* getIDATData(unsigned int &data_length, std::ifstream &file);

	char* getPNGData(unsigned int &data_length);

	void doDeFiltering(V_ScanLines &v_scanlines, Image *image);
	void doDeFilteringType0(int j_count, int j_delta, int cur_small_img, int cur_row, int i, ScanLine scanline, Image *image);
	void getScanLines(V_ScanLines &v_scanlines, char *decompr_data, int decompr_data_len);
	int getDecomprDataLen();

	void getSizeSmallImage(int cur_small_img, int &col, int &row);
	void getCoordPixel(int cur_small_image, int cur_row, int cur_col, int &i, int &j);
	int getCurRow(int cur_small_img, int general_index);

	unsigned int getPaethPredictor(unsigned int prev, unsigned int up, unsigned int diag);

	int getIntInRightOrder(char *buf);
	unsigned int readInt(std::ifstream &file);

public:
	PNGReader(std::string &file_name);	
	~PNGReader();
	Image* getImageStruct();
};

#endif //PNGREADER_H
