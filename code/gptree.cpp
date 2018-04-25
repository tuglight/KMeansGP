#include "gptree.h"

void createTree(Tree & initialTree, int numNodes)
{
  initialTree.fitness = 0.0;
  Node tempNode;
  tempNode.nodeType = 0;
  tempNode.operatorType = 0;
  tempNode.value = 0.0;
  initialTree.nodes.resize(numNodes, tempNode);
}
