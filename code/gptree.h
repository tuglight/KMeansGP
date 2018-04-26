// Programmer: Josh Tuggle

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

struct Node
{
  int nodeType;
  char operatorType;
  int featureIndex;
  float value;
};

struct Tree
{
  float fitness;
  std::vector<Node> nodes;
};

void createTree(Tree & initialTree, int numNodes);
void createTree(Tree & initialTree, std::string fileName);
void reduceFeatures(Tree & parseTree, std::vector<float> & features);
float calculateParseTree(int nodeIndex, Tree & parseTree, std::vector<float> & features);
float binaryOperators(float lhs, float rhs, char operatorType);
