// Programmer: Josh Tuggle

#include <vector>

struct Node
{
  int nodeType;
  int operatorType;
  float value;
};

struct Tree
{
  float fitness;
  std::vector<Node> nodes;
};

void createTree(Tree & initialTree, int numNodes);
