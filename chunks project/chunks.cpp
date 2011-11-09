#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <string>
#include <stdexcept>

#include "PNGReader.h"
#include "MyDecompressor.h"
#include "PNGWorker.h"


using namespace std;



int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		throw runtime_error("Please enter name of PNG file");
	}

	PNGReader reader = PNGReader(argv[1]);
	
	V_ChunksParams *chunks_params = reader.getPNGChunksParams();
	V_ChunksParams::iterator it = chunks_params->begin();

	while(it != chunks_params->end())
	{
		cout<<it->chunk_type<<" ("<<it->chunk_length<<")"<<endl;
		it++;
	}

	int compr_data_len = 0;
	char *compr_data = reader.getPNGData(compr_data_len);
	
	MyDecompressor decopressor = MyDecompressor(compr_data, compr_data_len);
	int decompr_data_len = 0;
	char *decompr_data = decopressor.getDecomprData(decompr_data_len);
	//if(decompr_data != NULL) cout<<"len = "<<decompr_data_len<<endl;


	PNGWorker worker = PNGWorker();

	return 0;
}