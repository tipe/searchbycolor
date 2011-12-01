#ifndef MYDECOMPESSOR_H
#define MYDECOMPESSOR_H


class MyDecompressor
{
public:
 	MyDecompressor();

	char* getDecomprData(unsigned int &decompr_data_length, char *compr_data, unsigned int compr_data_length);
};

#endif //MYDECOMPESSOR_H
