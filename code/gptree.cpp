// Programmer: Josh Tuggle
// Program: KMeansGP

#include "gptree.h"

// Creates a tree with a number of nodes that are of none type
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

// Creates a tree based on the file data
void createTree(Tree & initialTree, std::string fileName)
{
  std::ifstream dataFile;
  int numNodes;

  dataFile.open(fileName.c_str());

  // First value in a file is the number tree nodes
  dataFile >> numNodes;
  initialTree.nodes.resize(numNodes);
  // The second value is the fitness of the tree
  dataFile >> initialTree.fitness;

  // Next all the node data is listed sequently
  for (int i = 0; i < numNodes; i++)
  {
    dataFile >> initialTree.nodes[i].nodeType;
    dataFile >> initialTree.nodes[i].operatorType;
    dataFile >> initialTree.nodes[i].featureIndex;
    dataFile >> initialTree.nodes[i].value;
  }
}

// Writes tree data to a specified file
void writeTreeToFile(Tree & tree, std::string fileName)
{
  std::ofstream dataFile;
  int numNodes;

  dataFile.open(fileName.c_str());

  // First value in a file is the number tree nodes
  dataFile << tree.nodes.size();
  dataFile << " ";
  // The second value is the fitness of the tree
  dataFile << tree.fitness;

  // Next all the node data is listed sequently
  for (int i = 0; i < tree.nodes.size(); i++)
  {
    dataFile << " ";
    dataFile << tree.nodes[i].nodeType;
    dataFile << " ";
    dataFile << tree.nodes[i].operatorType;
    dataFile << " ";
    dataFile << tree.nodes[i].featureIndex;
    dataFile << " ";
    dataFile << tree.nodes[i].value;
  }
}

// Reduce a set of features into one based on the parse tree
void reduceFeatures(Tree & parseTree, std::vector<float> & features)
{
  float featureVal;
  featureVal = calculateParseTree(0, parseTree, features);
  features.resize(1);
  features[0] = featureVal;
}

// Caclulates the value returned by the parse tree given a set of features
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

// Performs the function of each of the operators
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

// Calculates the depth of the tree
int calculateTreeDepth(Tree & parseTree)
{
  return (int)(std::ceil(log2(parseTree.nodes.size() + 1)) - 1);
}

// Calculates the depth a particular node in a tree
int calculateNodeDepth(int nodeNumber)
{
  return (int)(std::ceil(log2(nodeNumber + 1)) - 1);
}

// Cleans the tree of unnecessary none type nodes at the end of the tree array
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

// Randomly selects a ndoe from the tree
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

// Will copy a rhs tree into a lhs tree
void copyTree(Tree & lhsTree, Tree & rhsTree)
{
  lhsTree.nodes.resize(rhsTree.nodes.size());
  for (int i = 0; i < lhsTree.nodes.size(); i++)
    lhsTree.nodes[i] = rhsTree.nodes[i];
  lhsTree.fitness = rhsTree.fitness;
}

// Sets all the nodes in branch to none type
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

// Recursive utility function used for subtree crossover
void recursiveSubtreeCrossover(int childNodeIndex, Tree & childTree, int parentNodeIndex, Tree & parentTree)
{
  if (parentNodeIndex >= parentTree.nodes.size())
    return;
  if (childNodeIndex >= childTree.nodes.size())
  {
    Node tempNode;
    tempNode.nodeType = 0;
    tempNode.operatorType = 'N';
    tempNode.featureIndex = -1;
    tempNode.value = 0.0;

    int growDist = (childNodeIndex - childTree.nodes.size()) + 1;

    for (int i = 0; i < growDist; i++)
      childTree.nodes.push_back(tempNode);
  }

  childTree.nodes[childNodeIndex] = parentTree.nodes[parentNodeIndex];

  recursiveSubtreeCrossover((2 * childNodeIndex) + 1, childTree, (2 * parentNodeIndex) + 1, parentTree);
  recursiveSubtreeCrossover((2 * childNodeIndex) + 2, childTree, (2 * parentNodeIndex) + 2, parentTree);
}

// Performs subtree crossover and returns a single tree
Tree subtreeCrossover(Tree & tree1, Tree & tree2)
{
  int randTreeNode1 = chooseRandomNode(tree1);
  int randTreeNode2 = chooseRandomNode(tree2);

  Tree newTree;
  copyTree(newTree, tree1);

  cleanBranch(randTreeNode1, newTree);
  recursiveSubtreeCrossover(randTreeNode1, newTree, randTreeNode2, tree2);
  cleanTree(newTree);

  return newTree;
}

// Creates a tree of a specified depth that has all nodes being a terminal or non-terminal
Tree createFullTree(int treeDepth, int numFeatures)
{
  int numNodes = std::pow(2, treeDepth + 1) - 1;
  Tree newTree;
  newTree.fitness = 0.0;
  Node tempNode;
  tempNode.nodeType = 0;
  tempNode.operatorType = 'N';
  tempNode.featureIndex = -1;
  tempNode.value = 0.0;
  newTree.nodes.resize(numNodes, tempNode);


  Node terminalNode;
  terminalNode.nodeType = 1;
  terminalNode.operatorType = 'N';
  terminalNode.featureIndex = -1;
  terminalNode.value = 0.0;

  Node functionNode;
  functionNode.nodeType = 2;
  functionNode.operatorType = 'N';
  functionNode.featureIndex = -1;
  functionNode.value = 0.0;

  Node randomNode;
  randomNode.nodeType = 1;
  randomNode.operatorType = 'N';
  randomNode.featureIndex = -1;
  randomNode.value = 0.0;


  std::vector<char> functions = {'+', '-', '*', '/'};
  int randVal;
  float floatRandVal;

  for (int i = 0; i < numNodes; i++)
  {

    if (calculateNodeDepth(i + 1) < treeDepth)
    {
      randVal = rand() % functions.size();
      functionNode.operatorType = functions[randVal];
      newTree.nodes[i] = functionNode;
    }
    else
    {
      randVal = rand() % (numFeatures + 1);
      // The non-random terminals case
      if (randVal < numFeatures)
      {
        terminalNode.featureIndex = randVal;
        newTree.nodes[i] = terminalNode;
      }
      else
      {
        floatRandVal = (float) rand() / RAND_MAX;
        randomNode.value = floatRandVal;
        newTree.nodes[i] = randomNode;
      }
    }
  }

  return newTree;
}

// Creates a tree that has random valid parse tree nodes and is with the specified depth
Tree createGrowTree(int treeDepth, int numFeatures)
{
  int numNodes = std::pow(2, treeDepth + 1) - 1;
  Tree newTree;
  newTree.fitness = 0.0;
  Node tempNode;
  tempNode.nodeType = 0;
  tempNode.operatorType = 'N';
  tempNode.featureIndex = -1;
  tempNode.value = 0.0;
  newTree.nodes.resize(numNodes, tempNode);
  std::vector<bool> availableNodes(numNodes, true);

  Node terminalNode;
  terminalNode.nodeType = 1;
  terminalNode.operatorType = 'N';
  terminalNode.featureIndex = -1;
  terminalNode.value = 0.0;

  Node functionNode;
  functionNode.nodeType = 2;
  functionNode.operatorType = 'N';
  functionNode.featureIndex = -1;
  functionNode.value = 0.0;

  Node randomNode;
  randomNode.nodeType = 1;
  randomNode.operatorType = 'N';
  randomNode.featureIndex = -1;
  randomNode.value = 0.0;


  std::vector<char> functions = {'+', '-', '*', '/'};
  int randVal;
  float floatRandVal;

  for (int i = 0; i < numNodes; i++)
  {
    if (availableNodes[i] == true)
    {
      if (calculateNodeDepth(i + 1) < treeDepth)
      {
        randVal = rand() % 2;
        if (randVal == 0)
        {
          randVal = rand() % functions.size();
          functionNode.operatorType = functions[randVal];
          newTree.nodes[i] = functionNode;
        }
        else
        {
          randVal = rand() % (numFeatures + 1);
          // The non-random terminals case
          if (randVal < numFeatures)
          {
            terminalNode.featureIndex = randVal;
            newTree.nodes[i] = terminalNode;
          }
          else
          {
            floatRandVal = (float) rand() / RAND_MAX;
            randomNode.value = floatRandVal;
            newTree.nodes[i] = randomNode;
          }
          markUnavailableNodes(i, numNodes, availableNodes);
        }
      }
      else
      {
        randVal = rand() % (numFeatures + 1);
        // The non-random terminals case
        if (randVal < numFeatures)
        {
          terminalNode.featureIndex = randVal;
          newTree.nodes[i] = terminalNode;
        }
        else
        {
          floatRandVal = (float) rand() / RAND_MAX;
          randomNode.value = floatRandVal;
          newTree.nodes[i] = randomNode;
        }
        markUnavailableNodes(i, numNodes, availableNodes);
      }
    }
  }
  cleanTree(newTree);

  return newTree;
}

// Utility function used grow method that markes nodes in branch as being essentially none types
void markUnavailableNodes(int treeIndex, int numNodes, std::vector<bool> & availableNodes)
{
  if (treeIndex >= numNodes)
    return;

  availableNodes[treeIndex] = false;

  markUnavailableNodes((2 * treeIndex) + 1, numNodes, availableNodes);
  markUnavailableNodes((2 * treeIndex) + 2, numNodes, availableNodes);
}
