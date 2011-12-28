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
#include "ImagesComparison.h"




void getImagesByColor(std::vector<Image*> &images, std::vector<int> &images_by_color, int red, int green, int blue, int range, int percent)
{ 
    for(unsigned int i = 0; i < images.size(); ++i)
    {
        if(images[i]->getPercentColor(red, green, blue, range) >= percent)
        {
           images_by_color.push_back(i);
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
    		string extension = (strrchr(entry->d_name, '.'));

    		string file_name = argv[1];
    		file_name += "/";
    		file_name += entry->d_name;

    		Image *image;
    		ImageReader *reader = NULL;

    		if(extension == ".png")
    		{
    			reader = new PNGReader(file_name);  
    		}
    		else
    		if(extension == ".bmp")
    		{
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


    // search by color

    // std::vector<int> images_by_color;

    // getImagesByColor(images, images_by_color, 80, 170, 190, 100, 5);

    // for(int i = 0; i < images_by_color.size(); ++i)
    // {
    //     std::cout<<images_names[images_by_color[i]]<<std::endl;
    // }


    // search similar

    ImagesComparison comparative;
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