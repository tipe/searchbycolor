#ifndef MYDECOMPESSOR_H
#define MYDECOMPESSOR_H


class MyDecompressor
{
private:
	char *compr_data;
	int compr_data_length;

	char *decompr_data;
	int decompr_data_length;
	
public:
 	MyDecompressor(char *_compr_data, int _compr_data_length);

	char* getDecomprData(int &_decompr_data_length);
};

#endif //MYDECOMPESSOR_H