// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

// #include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

struct Point
{
	std::string label;
	std::vector<float> row;
};

void getPointsFromFile(std::vector<Point> & fileData, std::string file)
{
	std::ifstream dataFile;
	std::string dummyString;
	int numFeatures = 0;
	int rowCounter = 0;
	fileData.resize(0);
	dataFile.open(file.c_str());
	getline(dataFile, dummyString);

	// Determine the number of number of features
	for (int i = 0; i < dummyString.size(); i++)
	{
		if (dummyString[i] == ',')
			numFeatures++;
	}

	Point tempPoint;
	std::string tempString;
	tempPoint.row.resize(numFeatures);


	while (!dataFile.eof())
	{
		getline(dataFile, tempPoint.label, ',');
		for (int i = 0; i < numFeatures - 1; i++)
		{
			getline(dataFile, tempString, ',');
			tempPoint.row[i] = stof(tempString);
		}
		getline(dataFile, tempString);
		tempPoint.row[numFeatures - 1] = stof(tempString);
		fileData.push_back(tempPoint);
	}
	fileData.pop_back();

}


int main()
{
	std::string testDataFile = "../data/iris.csv";
  std::vector<Point> fileData;
	getPointsFromFile(fileData, testDataFile);
  std::cout << (fileData.size());

	return 0;
}
