// Programmer: Josh Tuggle

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

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
int calculateTreeDepth(Tree & parseTree);
int calculateNodeDepth(int nodeNumber);
void cleanTree(Tree & parseTree);
int chooseRandomNode(Tree & tree);
void copyTree(Tree & lhsTree, Tree & rhsTree);
void cleanBranch(int chosenNode, Tree & tree);
void recursiveSubtreeCrossover(int childNodeIndex, Tree & childTree, int parentNodeIndex, Tree & parentTree);
Tree subtreeCrossover(Tree & tree1, Tree & tree2);
Tree createFullTree(int treeDepth, int numFeatures);
Tree createGrowTree(int treeDepth, int numFeatures);
void markUnavailableNodes(int treeIndex, int numNodes, std::vector<bool> & availableNodes);
