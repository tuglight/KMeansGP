// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

// #include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include <cmath>
#include <limits>
#include "gptree.h"

const float MAX_FLOAT = std::numeric_limits<float>::max();

struct Point
{
	std::string label;
	std::vector<float> row;
};

void getPointsFromFile(std::vector<Point> & fileData, int & numFeatures, std::string file)
{
	std::ifstream dataFile;
	std::string dummyString;
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

std::vector<Point> createKRandomPoints(std::vector<Point> & dataPoints, int k)
{
	std::random_device rd;
	std::default_random_engine defRanEng{rd()};
	std::shuffle(dataPoints.begin(), dataPoints.end(), defRanEng);
	std::vector<Point> selectedPoints(k);

	for (int i = 0; i < k; i++)
	{
		selectedPoints[i] = dataPoints[i];
	}

	return selectedPoints;
}

float calculateEuclideanDistance(Point clusterCentroid, Point dataPoint)
{
	float euclidDist = 0.0;
	for (int i = 0; i < clusterCentroid.row.size(); i++)
	{
		euclidDist += std::pow((clusterCentroid.row[i] - dataPoint.row[i]), 2);
	}

	return std::sqrt(euclidDist);

}

int getBestPointAssignment(Point dataPoint, std::vector<Point> & clusters, int k)
{
	float leastDistance = MAX_FLOAT;
	float calculatedDistance;
	int bestClusterIndex;
	for (int i = 0; i < k; i++)
	{
		calculatedDistance = calculateEuclideanDistance(clusters[i], dataPoint);
		if (calculatedDistance < leastDistance)
		{
			leastDistance = calculatedDistance;
			bestClusterIndex = i;
		}
	}

	return bestClusterIndex;
}

std::vector<int> assignAllDataPoints(std::vector<Point> & dataPoints, std::vector<Point> & clusters, int k)
{
	std::vector<int> assignments(dataPoints.size());

	for (int i = 0; i < dataPoints.size(); i++)
	{
		assignments[i] = getBestPointAssignment(dataPoints[i], clusters, k);
	}
	return assignments;
}

std::vector<Point> recomputeCentroids(std::vector<int> currAssignments, std::vector<Point> & dataPoints, int numFeatures, int k)
{
	Point modelPoint;
	double numClusterPoints;
	modelPoint.row.resize(numFeatures, 0.0);
	std::vector<int> pointCount(k, 0);
	std::vector<Point> calcNewClusters(k, modelPoint);
	int currCluster;
	for (int i = 0; i < currAssignments.size(); i++)
	{
		currCluster = currAssignments[i];
		pointCount[currCluster]++;
		for (int j = 0; j < numFeatures; j++)
		{
			calcNewClusters[currCluster].row[j] += dataPoints[i].row[j];
		}
	}
	for (int i = 0; i < k; i++)
	{
		if (pointCount[i] == 0)
			numClusterPoints = 0.01;
		else
			numClusterPoints = pointCount[i];
		for (int j = 0; j < numFeatures; j++)
		{
			calcNewClusters[i].row[j] /= numClusterPoints;
		}
	}

	int mySize = calcNewClusters.size();

	return calcNewClusters;
}

bool equalCentroids(std::vector<Point> & oldClusters, std::vector<Point> & newClusters, int numFeatures)
{
	bool isAllEqualCentroids = true;
	if (oldClusters[0].row.size() == 0)
		isAllEqualCentroids = false;
	else
	{
		for (int i = 0; i < oldClusters.size(); i++)
		{
			for (int j = 0; j < numFeatures; j++)
			{
				if (oldClusters[i].row[j] != newClusters[i].row[j])
					isAllEqualCentroids = false;
					break;
			}
			if (isAllEqualCentroids == false)
				break;
		}
	}

	return isAllEqualCentroids;
}

void kmeans(std::vector<Point> & dataPoints, int k)
{
	std::vector<int> currAssignments(dataPoints.size());
	std::vector<Point> oldClusters(k);
	std::vector<Point> newClusters(k);
	newClusters = createKRandomPoints(dataPoints, k);
	int numFeatures = dataPoints[0].row.size();
	int counter = 0;
	while(1)
	{
		counter++;
		currAssignments = assignAllDataPoints(dataPoints, newClusters, k);
		newClusters = recomputeCentroids(currAssignments, dataPoints, numFeatures, k);
		if (equalCentroids(oldClusters, newClusters, numFeatures))
			break;
		oldClusters = newClusters;
	};
}

int main()
{
	std::string testDataFile = "../data/iris.csv";
	std::vector<Point> fileData;
	int k = 3;
	int numFeatures = 0;

	getPointsFromFile(fileData, numFeatures, testDataFile);

	Tree myTree;
	// createTree(myTree, 13);
	createTree(myTree, "../results/test.txt");
	for (int i = 0; i < fileData.size(); i++)
	{
		reduceFeatures(myTree, fileData[i].row);
	}
	kmeans(fileData, k);
	int depth = calculateTreeDepth(myTree);
	Tree newTree;
	copyTree(newTree, myTree);
	// cleanTree(myTree);
	Tree babyTree = subtreeCrossover(newTree, myTree);

	return 0;
}
