#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

class Distribution
{
private:
	float ***colors_distribution;
	int size_distribution;	//TODO choose one name instead of two

	void getThreeDimensionalMatrix();
	void deleteThreeDimensionalMatrix();



public:
	Distribution(int divider=4);
	~Distribution();

	int getDivider();

	void setValue(int i, int j, int k, float value);
	float getValue(int i, int j, int k);

	void addValue(int i, int j, int k, float delta);

	int compare(Distribution *dist, int range_percent=15);
};

#endif // DISTRIBUTION_H
