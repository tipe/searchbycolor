#ifndef PNGWORKER_H
#define PNGWORKER_H

class PNGWorker
{
private:
	int getInt(char buf);
	void getSizeReducedImage(int w, int h, int numRedImg, int &col, int &row);
	char* getImage(char *filt_data, int data_size, int file_w, int file_h);
	
public:
	PNGWorker();

};

#endif //PNGWORKER_H