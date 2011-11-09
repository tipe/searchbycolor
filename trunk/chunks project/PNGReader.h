#ifndef PNGREADER_H
#define PNGREADER_H

#include <stdio.h>
#include <vector>
#include <string.h>
#include <string>

using namespace std;


struct ChunkParams
{
	string chunk_type;
	unsigned int chunk_length;

	ChunkParams(string type, int lenght)
	{
		chunk_type = type;
		chunk_length = lenght;
	}
};

typedef std::vector<ChunkParams> V_ChunksParams;

typedef std::vector<char> V_IDATData;

class PNGReader
{
private:
	FILE *file;

	char *file_name;
	unsigned int file_size;
	int file_width;
	int file_heigth;

	unsigned int data_length;

	V_ChunksParams v_chunks_params;
	V_IDATData v_idat_total_data;	
	//char *idat_tmp_data;
	
	bool is_bigendian();
	int getIntInRightOrder(char *buf);

	int getIDChunk(char *buf);

	void getIHDRData();
	char* getIDATData();

public:
	PNGReader(char *file_name);
	
	unsigned int getFileSize();
	unsigned int getFileWidth();
	unsigned int getFileHeigth();
	
	V_ChunksParams* getPNGChunksParams();
	char* getPNGData(int &data_length);

	int getChunksCount();
};

#endif //PNGREADER_H