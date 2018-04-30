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
#include <ctime>
#include <limits>
#include <chrono>
#include "gptree.h"

const float MAX_FLOAT = std::numeric_limits<float>::max();

struct Point
{
	std::string label;
	std::vector<float> row;
};

struct Settings
{
	int k;
	int numFeatures;
	std::string trainingDataset;
	std::string testingDataset;
	int populationSize;
	int generationSize;
	float parsimonyCoefficient;
	int numberEvaluations;
};

std::string getSettingsLine(std::ifstream & settingsIfStream);
Settings getSettings(std::string settingsFile);
void getPointsFromFile(std::vector<Point> & fileData, int & numFeatures, std::string file);
std::vector<Point> createKRandomPoints(std::vector<Point> & dataPoints, int k);
float calculateEuclideanDistance(Point clusterCentroid, Point dataPoint);
int getBestPointAssignment(Point dataPoint, std::vector<Point> & clusters, int k);
std::vector<int> assignAllDataPoints(std::vector<Point> & dataPoints, std::vector<Point> & clusters, int k);
std::vector<Point> recomputeCentroids(std::vector<int> currAssignments, std::vector<Point> & dataPoints, int numFeatures, int k);
bool equalCentroids(std::vector<Point> & oldClusters, std::vector<Point> & newClusters, int numFeatures);
std::vector<std::string> getLabels(std::vector<Point> & dataPoints);
float computePurity(std::vector<Point> & dataPoints, std::vector<std::string> & labels, std::vector<int> & assignments, int k);
void kmeans(std::vector<Point> & dataPoints, int k);
void runEA(Settings settingsInfo);
void initializePopulation(std::vector<Tree> & population, int populationSize, int numFeatures);

int main()
{
	srand(time(NULL));
	std::string testDataFile = "../data/iristraining.csv";
	std::string settingsFile = "../results/irissettings.txt";
	Settings eaSettings = getSettings(settingsFile);
	runEA(eaSettings);
	std::vector<Point> fileData;
	int k = 3;
	int numFeatures = 0;
	getPointsFromFile(fileData, numFeatures, testDataFile);

	Tree myTree;
	// createTree(myTree, 13);
	createTree(myTree, "../results/test.txt");
	int depth = calculateTreeDepth(myTree);
	Tree newTree;
	copyTree(newTree, myTree);
	Tree babyTree = subtreeCrossover(newTree, myTree);
	Tree fatBabyTree = createFullTree(3, numFeatures);
	Tree fatBabyTree2 = createFullTree(3, numFeatures);
	Tree fatBabyTree3 = subtreeCrossover(fatBabyTree, fatBabyTree2);
	Tree unbalancedBaby = createGrowTree(3, numFeatures);
	Tree unbalancedBaby2 = createGrowTree(3, numFeatures);
	Tree unbalancedBaby3 = subtreeCrossover(unbalancedBaby, unbalancedBaby2);

	// for (int i = 0; i < fileData.size(); i++)
	// {
	// 	reduceFeatures(fatBabyTree3, fileData[i].row);
	// }
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	kmeans(fileData, k);
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	std::cout << "It took me " << time_span.count() << " seconds.";



	return 0;
}

std::string getSettingsLine(std::ifstream & settingsIfStream)
{
	std::string dummyString;
	settingsIfStream >> dummyString;
	settingsIfStream >> dummyString;
	return dummyString;
}

Settings getSettings(std::string settingsFile)
{
	Settings settingsInfo;
	std::ifstream settingsIfStream;
	settingsIfStream.open(settingsFile.c_str());

	settingsInfo.k = std::stoi(getSettingsLine(settingsIfStream));
	settingsInfo.numFeatures = std::stoi(getSettingsLine(settingsIfStream));
	settingsInfo.trainingDataset = getSettingsLine(settingsIfStream);
	settingsInfo.testingDataset = getSettingsLine(settingsIfStream);
	settingsInfo.populationSize = std::stoi(getSettingsLine(settingsIfStream));
	settingsInfo.generationSize = std::stoi(getSettingsLine(settingsIfStream));
	settingsInfo.parsimonyCoefficient = std::stof(getSettingsLine(settingsIfStream));
	settingsInfo.numberEvaluations = std::stoi(getSettingsLine(settingsIfStream));

	return settingsInfo;
}

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
	std::cout << rd() << "\n";
	std::cout << rd() << "\n";
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

std::vector<std::string> getLabels(std::vector<Point> & dataPoints)
{
	std::vector<std::string> labels(0);
	bool foundLabel;

	for (int i = 0; i < dataPoints.size(); i++)
	{
		foundLabel = false;
		if (labels.size() > 0)
		{
			for (int j = 0; j < labels.size(); j++)
			{
				if (dataPoints[i].label == labels[j])
				{
					foundLabel = true;
					break;
				}
			}
			if (foundLabel == false)
				labels.push_back(dataPoints[i].label);
		}
		else
			labels.push_back(dataPoints[i].label);
	}

	return labels;
}

float computePurity(std::vector<Point> & dataPoints, std::vector<std::string> & labels, std::vector<int> & assignments, int k)
{
	// Two-dimensional vector that keeps track of how many times a label is in a cluster
	std::vector<std::vector<int> > assignmentCounts(k, std::vector<int>(labels.size(), 0));
	float purity = 0.0;
	int maxNumber;

	for (int i = 0; i < assignments.size(); i++)
	{
		for (int j = 0; j < labels.size(); j++)
		{
			if (dataPoints[i].label == labels[j])
			{
				assignmentCounts[assignments[i]][j]++;
				break;
			}
		}
	}

	for (int i = 0; i < assignmentCounts.size(); i++)
	{
		maxNumber = 0;
		for (int j = 0; j < assignmentCounts[i].size(); j++)
		{
			if (assignmentCounts[i][j] > maxNumber)
				maxNumber = assignmentCounts[i][j];
		}
		purity += maxNumber;
	}
	purity /= dataPoints.size();

	return purity;
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
	std::vector<std::string> labels = getLabels(dataPoints);
	float purity = computePurity(dataPoints, labels, currAssignments, k);
}

void runEA(Settings settingsInfo)
{
	int remainingEvals = settingsInfo.numberEvaluations;
	std::vector<Tree> population(0);
	initializePopulation(population, settingsInfo.populationSize, settingsInfo.numFeatures);
	// remainingEvals -= settingsInfo.populationSize;
}

void initializePopulation(std::vector<Tree> & population, int populationSize, int numFeatures)
{
	// Current implementation is ramped-half-and-ah
	int halfPopulation = populationSize / 2;
	int treeDepth = 3;
	for (int i = 0; i < populationSize; i++)
	{
		if (i < halfPopulation)
			population.push_back(createFullTree(3, numFeatures));
		else
			population.push_back(createGrowTree(3, numFeatures));
	}
}
