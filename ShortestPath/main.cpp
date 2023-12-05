#include "graph_matrix.h"
#include <iostream>
#include <fstream>
#include <limits>

struct charNode
{
	char label = 0;
	int visted = 0;
	char prevNode = 0;
	int weight = -1;
};
string setupMatrix(GraphMatrix<char>& g, string inputGraphFileName)
{
	// setupMatrix sets up adjacency matrix
	std::fstream fs;
	stringstream ss;
	string headerString;
	fs.open(inputGraphFileName, std::ios::in);

	if (!fs.is_open()) // if file couldn't open
	{
		cout << "Error opening " << inputGraphFileName << endl;
		return "";
	}
	// file was opened
	getline(fs, headerString); // get header of graphInput.txt

	string line;
	int sizeGraph = (int)headerString.size();
	
	// add nodes to graph
	for (int i = 0; i < sizeGraph; i++)
	{
		g.addNode(headerString[i]);
	}

	int colCount = 0;
	int tmpNum = 0;
	for (int i = 0; i < sizeGraph; i++) // iterate over rows of matrix
	{
		getline(fs, line); // place next line of file into string "line"
		ss << line; // place string "line" into stringstream

		while (ss >> tmpNum)
		{
			if(colCount == headerString.size())
			{
				cout << "Too many columns in " << line << endl;
				return {};
			}
			//tmpNum -= '0'; // convert the char to int
			g.addLink(headerString[i], headerString[colCount], tmpNum); // add char to graph
			colCount++; // keep track of how wide this row is
		}
		colCount = 0;
		ss.str(""); // set stringstream to empty string)
		ss.clear(); // reset any flags in stringstream
	}
	fs.close();
	return headerString;
}
int getSmallestWeightVect(vector<charNode>& vecNodes)
{
	int smallestValue; // the smallest value we have come across so far
	int i = 0; // index through vector
	int colPosSmallestValue; // the col position of the smallest value we have come across so far

	while (i < vecNodes.size())
	{
		if (vecNodes[i].weight > 0 && !vecNodes[i].visted) // if weight above 0 (not INF), and not visited, 
			break;										   // then i is the col of the first smallest weight
		i++;
	}

	if (i == vecNodes.size() - 1) // if i is at the last col of the adjacency matrix, return that col position
		return i;

	smallestValue = vecNodes[i].weight; // initialize smallestWeight to first weighted, unvisted node's weight
	colPosSmallestValue = i;            // intialize col index to index of first weighted, unvisited node's weight

	while(i < vecNodes.size()) // loop over remaining length of nodes
	{
		if (vecNodes[i].weight < smallestValue)
		{
			smallestValue = vecNodes[i].weight;
			colPosSmallestValue = i;
		}
		i++;
	}

	return colPosSmallestValue;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "Must provide input file\n";
		return 0;
	}

	string fileToRead = argv[1];

	GraphMatrix<char> g;
	string header = setupMatrix(g, fileToRead); // pass object and file to set up matrix with
	int matrixSize = header.size();
	g.printAdjacencyMatrix();
	vector<charNode> vecNodes;
	vecNodes.resize(matrixSize);

	for (int i = 0; i < matrixSize; i++)
	{
		vecNodes[i].label = header[i];
	}
	char startNode;
	bool flag = 0;
	while (!flag) // get valid start node from user
	{
		cout << "Start node: ";
		std::cin >> startNode;

		for (int i = 0; i < matrixSize; i++) // make sure user input node is in the graph
		{
			if (startNode == header[i]) // case sensitive
			{
				flag = 1;
				break;
			}
		}
		if (!flag)
		{
			cout << "Start node: " << startNode << " not in graph\n";
		}
	}
	// first row initialization
	for (int i = 0; i < vecNodes.size(); i++)
	{
		if (vecNodes[i].label == startNode)
		{
			vecNodes[i].visted = 1;// set start node to visited
			vecNodes[i].weight = 0;
			continue;
		}	
		if (g.getLink(startNode, vecNodes[i].label) != 0) // if link exists between start node and other node
		{
			vecNodes[i].weight = g.getLink(startNode, vecNodes[i].label);
		}
	}

	int smallest = getSmallestWeightVect(vecNodes); // find the smallest non-startNode node
	vecNodes[smallest].visted = 1;

	for (int i = 0; i < vecNodes.size(); i++) // iterate over rows 1 through end
	{
		for (int j = 0; j < vecNodes.size(); j++) // iterate over all cols
		{
			if (vecNodes[j].visted) // if node visited, skip past it
				continue;
			if (g.getLink(header[smallest], vecNodes[j].label) != 0) // check for link between visted and node to visit
			{
				if (vecNodes[j].weight == -1) // if the node has no path to it yet
				{
					vecNodes[j].weight = g.getLink(header[smallest], vecNodes[j].label) + vecNodes[smallest].weight;
					vecNodes[j].prevNode = header[smallest];
				}
					
				else if (g.getLink(header[smallest], vecNodes[j].label) + vecNodes[smallest].weight < vecNodes[j].weight)
				{
					vecNodes[j].weight = g.getLink(header[smallest], vecNodes[j].label) + vecNodes[smallest].weight;
					vecNodes[j].prevNode = header[smallest];
				}
			}
		}
		smallest = getSmallestWeightVect(vecNodes);
	}

	for (int i = 0; i < vecNodes.size(); i++)
	{
		cout << vecNodes[i].label << ":" << vecNodes[i].weight << "(" << vecNodes[i].prevNode << "), ";
	}

	return 0;
}