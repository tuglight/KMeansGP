// Programmer: Josh Tuggle
// Program: KMeansGP

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

struct Node
{
  int nodeType; // 0 - none, 1 - terminal, 2 - non-terminal
  char operatorType; // Character of an operator if there is one, otherwise 'N'
  int featureIndex; // Array index of a feature
  float value; // Can contain a value if terminal needs it
};

struct Tree
{
  float fitness;
  std::vector<Node> nodes;
};

void createTree(Tree & initialTree, int numNodes);
void createTree(Tree & initialTree, std::string fileName);
void writeTreeToFile(Tree & tree, std::string fileName);
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
