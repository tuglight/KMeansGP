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
  dataFile >> initialTree.fitness;

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

int calculateTreeDepth(Tree & parseTree)
{
  return (int)(std::ceil(log2(parseTree.nodes.size() + 1)) - 1);
}

void cleanTree(Tree & parseTree)
{
  int counter = parseTree.nodes.size();
  while(counter > 0)
  {
    counter--;
    if (parseTree.nodes[counter].nodeType == 1 || parseTree.nodes[counter].nodeType == 2)
    {
      parseTree.nodes.resize(counter + 1);
      break;
    }
  }

}

int chooseRandomNode(Tree & tree)
{
  int numTreeNodes = tree.nodes.size();
  int numCurrRolls = 0;
  int randVal;
  do {
    randVal = rand() % numTreeNodes;
    numCurrRolls++;
    if (tree.nodes[randVal].nodeType != 0)
      break;
  } while(numCurrRolls <= numTreeNodes);
  return randVal;
}

void copyTree(Tree & lhsTree, Tree & rhsTree)
{
  lhsTree.nodes.resize(rhsTree.nodes.size());
  for (int i = 0; i < lhsTree.nodes.size(); i++)
    lhsTree.nodes[i] = rhsTree.nodes[i];
  lhsTree.fitness = rhsTree.fitness;
}

void cleanBranch(int chosenNode, Tree & tree)
{
  if (chosenNode < tree.nodes.size())
  {
    tree.nodes[chosenNode].nodeType = 0;
    tree.nodes[chosenNode].operatorType = 'N';
    tree.nodes[chosenNode].featureIndex = -1;
    tree.nodes[chosenNode].value = 0.0;
    cleanBranch((2 * chosenNode) + 1, tree);
    cleanBranch((2 * chosenNode) + 2, tree);
  }
}

void recursiveSubtreeCrossover(int childNodeIndex, Tree & childTree, int parentNodeIndex, Tree & parentTree)
{
  if (parentNodeIndex >= parentTree.nodes.size() - 1)
    return;
  if (childNodeIndex >= childTree.nodes.size())
  {
    Node tempNode;
    tempNode.nodeType = 0;
    tempNode.operatorType = 'N';
    tempNode.featureIndex = -1;
    tempNode.value = 0.0;

    int growDist = (childTree.nodes.size() - childNodeIndex) + 1;

    for (int i = 0; i < growDist; i++)
      childTree.nodes.push_back(tempNode);
  }

  childTree.nodes[childNodeIndex] = parentTree.nodes[parentNodeIndex];

  recursiveSubtreeCrossover((2 * childNodeIndex) + 1, childTree, (2 * parentNodeIndex) + 1, parentTree);
  recursiveSubtreeCrossover((2 * childNodeIndex) + 2, childTree, (2 * parentNodeIndex) + 2, parentTree);
}

Tree subtreeCrossover(Tree & tree1, Tree & tree2)
{
  int randTreeNode1 = chooseRandomNode(tree1);
  int randTreeNode2 = chooseRandomNode(tree2);

  Tree newTree;
  copyTree(newTree, tree1);

  cleanBranch(randTreeNode1, newTree);
  recursiveSubtreeCrossover(randTreeNode1, newTree, randTreeNode2, tree2);

  return newTree;
}
