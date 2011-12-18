#ifndef IMAGESCOMPARATIVE_H
#define IMAGESCOMPARATIVE_H

#include <vector>

#include "Image.h"
#include "Graph.h"


class ImagesComparative
{
private:
	std::vector<Distribution*> images_dists;
	void buildGraph(std::vector<Image*> &images, Graph *graph);
	

public:
	ImagesComparative();
	~ImagesComparative();

	void getSimilarImages(std::vector<Image*> &images);
};


#endif // IMAGESCOMPARATIVE_H