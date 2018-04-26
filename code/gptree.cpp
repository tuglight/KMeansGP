#include "gptree.h"

void createTree(Tree & initialTree, int numNodes)
{
  initialTree.fitness = 0.0;
  Node tempNode;
  tempNode.nodeType = 0;
  tempNode.operatorType = 'N';
  tempNode.featureIndex = -1;
  tempNode.value = 0.0;
  initialTree.nodes.resize(numNodes, tempNode);
}

void createTree(Tree & initialTree, std::string fileName)
{
  std::ifstream dataFile;
  int numNodes;

  dataFile.open(fileName.c_str());

  dataFile >> numNodes;

  initialTree.nodes.resize(numNodes);

  for (int i = 0; i < numNodes; i++)
  {
    dataFile >> initialTree.nodes[i].nodeType;
    dataFile >> initialTree.nodes[i].operatorType;
    dataFile >> initialTree.nodes[i].featureIndex;
    dataFile >> initialTree.nodes[i].value;
  }
}

void reduceFeatures(Tree & parseTree, std::vector<float> & features)
{
  float featureVal;
  featureVal = calculateParseTree(0, parseTree, features);
  features.resize(1);
  features[0] = featureVal;
}

float calculateParseTree(int nodeIndex, Tree & parseTree, std::vector<float> & features)
{
  float calculatedValue;
  if (parseTree.nodes[nodeIndex].nodeType == 2)
  {
    float lhs = calculateParseTree((2 * nodeIndex) + 1, parseTree, features);
    float rhs = calculateParseTree((2 * nodeIndex) + 2, parseTree, features);
    calculatedValue = binaryOperators(lhs, rhs, parseTree.nodes[nodeIndex].operatorType);
  }
  else if (parseTree.nodes[nodeIndex].nodeType == 1)
  {
    if (parseTree.nodes[nodeIndex].featureIndex == -1)
      calculatedValue = parseTree.nodes[nodeIndex].value;
    else
      calculatedValue = features[parseTree.nodes[nodeIndex].featureIndex];
  }
  return calculatedValue;
}

float binaryOperators(float lhs, float rhs, char operatorType)
{
  float evalVal;
  switch (operatorType) {
    case '+':
      evalVal = lhs + rhs;
      break;
    case '-':
      evalVal = lhs - rhs;
      break;
    case '*':
      evalVal = lhs * rhs;
      break;
    case '/':
      if (rhs == 0.0)
        rhs = 0.01;
      evalVal = lhs / rhs;
      break;
  }

  return evalVal;
}
