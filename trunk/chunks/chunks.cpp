#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <string>
#include <stdexcept>
#include <dirent.h>
#include <vector>


#include "PNGReader.h"
#include "BMPReader.h"
#include "ImagesComparative.h"




void getImagesByColor(std::vector<Image*> &images, std::vector<int> &imagesByColor)
{ 
    for(unsigned int i = 0; i < images.size(); ++i)
    {
        if(images[i]->getPercentColor(250, 0, 0, 70) >= 5)
        {
           imagesByColor.push_back(i);
        }
    }
}


int main(int argc, char *argv[])
{	
	if(argc < 2)
	{
		throw runtime_error("Please enter name of PNG file");
	}

	DIR *dir = opendir(argv[1]);

	if(dir == NULL)
	{
		throw runtime_error("Can not open dir");
	}

	struct dirent *entry;

	
	vector<string> images_names;
	vector<Image*> images;
	

	while ((entry = readdir(dir)) != NULL)
    {
    	if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
    	{
    		string expansion = (strrchr(entry->d_name, '.'));

    		string file_name = argv[1];
    		file_name += "/";
    		file_name += entry->d_name;
    		//cout<<"file = "<<file_name<<endl;

    		Image *image;
    		ImageReader *reader = NULL;

    		if(expansion == ".png")
    		{
    			//cout<<"png"<<endl;
    			//cout<<"file = "<<file_name<<endl;
    			reader = new PNGReader(file_name);  
    		}
    		else
    		if(expansion == ".bmp")
    		{
    			//cout<<"bmp"<<endl;
    			//cout<<"file = "<<file_name<<endl;
    			reader = new BMPReader(file_name);
    		}


    		if(reader != NULL)
    		{
    			image = reader->getImageStruct();
    			images.push_back(image);
                images_names.push_back(file_name);

    			
    		}
    		
    	}        
    }

    ImagesComparative comparative;
    std::vector<std::vector<int> > similar_images;
    comparative.getSimilarImages(images, similar_images);

    for(unsigned int i = 0; i < similar_images.size(); ++i)
    {
        for(unsigned int j = 0; j < similar_images[i].size(); ++j)
        {
            std::cout<<images_names[similar_images[i][j]]<<" ";
        }
        std::cout<<std::endl;
    }

    for(unsigned int i = 0; i < images.size(); ++i)
    {
    	delete images[i];
    }
    
    closedir(dir);

	return 0;
}