#ifndef PNGWORKER_H
#define PNGWORKER_H





class PNGWorker
{
private:
	Pixel **image;

	int getInt(char buf);
	void getSizeReducedImage(int img_h, int img_w, int numRedImg, int &col, int &row);
	char* getImage(char *filt_data, int data_size, int file_w, int file_h);
	
public:
	PNGWorker(unsigned int img_h, unsigned int img_w);
	PNGWorker();
};

#endif //PNGWORKER_H