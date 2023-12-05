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
	std::fstream fs;
	string header;
	fs.open(inputGraphFileName, std::ios::in);

	if (fs.is_open())
	{
		getline(fs, header);

		string line;
		int sizeGraph = (int)header.size();
		// add nodes to graph
		for (int i = 0; i < sizeGraph; i++)
		{
			g.addNode(header[i]);
		}
		int tmp = 0;
		for (int i = 0; i < sizeGraph; i++) // iterate over rows of matrix
		{
			getline(fs, line);
			int lineIndex = 0;

			if (line.size() - (line.size() - 1) > sizeGraph) // more columns than expected
			{
				cout << "Too many columns in " << line << endl;
				return {};
			}
			for (int j = 0; j < sizeGraph; j++)
			{
				tmp = line[lineIndex] - '0'; // convert the char to int
				g.addLink(header[i], header[j], tmp); // add char to graph
				lineIndex += 2; // move forward by 2 to account for spaces between numbers
			}
		}
	}
	else
	{
		cout << "Error opening " << inputGraphFileName << endl;;
	}
	fs.close();
	return header;
}
int getSmallestWeightVect(vector<charNode>& vecNodes)
{
	int smallest;
	int i = 0;
	while (i < vecNodes.size())
	{
		if (vecNodes[i].weight > 0 && !vecNodes[i].visted)
			break; 
		i++;
	}
	if (i == vecNodes.size() - 1)
		return i;
	smallest = i;
	for (int j = i; j < vecNodes.size(); j++)
	{
		if (vecNodes[i].weight < smallest)
			smallest = i;
	}
	return smallest;
}

int main()
{
	GraphMatrix<char> g;
	string header = setupMatrix(g, "graphInput.txt"); // pass object and file to set up matrix with
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


	//// first distance is always 0
	//cout << startNode << ": " << g.getLink(startNode, startNode) << "(" << startNode << "), " << endl;
	////cout << "index of c " << g.getNodeIndex('c') << std::endl;
	//int* SPmatrix = new int[matrixSize * matrixSize]; // hold shortest path matrix
	//int nodeIndex;

	//for (int i = 0; i < matrixSize * matrixSize; i++) // initialize values to -2
	//{
	//	SPmatrix[i] = -2; // initialization value
	//}

	//for (int j = 0; j < matrixSize; j++) // place values in first row
	//{
	//	SPmatrix[j] = g.getLink(startNode, header[j]); // [0][j]
	//}
	//signalNodeVisited(SPmatrix, 0, g.getNodeIndex(startNode), matrixSize);

	//printSPmatrix(SPmatrix, matrixSize); // check results of matrix so far
	////int row = 0;
	////cout << "smallest value at row " << row << getSmallestWeightAtRow(SPmatrix, row, g.getNodeIndex(startNode), matrixSize) << endl;

	//int nodeToVisit = getSmallestWeightAtRow(SPmatrix, 0, g.getNodeIndex(startNode), matrixSize);
	//cout << nodeToVisit << endl;

	//for (int i = 1; i < header.size(); i++) // traverse all rows after first row of SPmatrix
	//{
	//	for (int j = 0; j < header.size(); j++)
	//	{
	//		if (SPmatrix[j + header.size() * i] == -1) // make sure its not in a visited col
	//			continue;
	//		if (SPmatrix[j + header.size() * i] == -2) // if that connection has not been seen yet
	//			SPmatrix[j + header.size() * i] = g.getLink(header[nodeToVisit], header[j]);
	//		if (g.getLink(header[nodeToVisit], header[j]) + < SPmatrix[j - 1 + header.size() * i]) // 

	//	}
	//	cout << "\n";
	//	//nodeIndex = g.getNodeIndex(startNode);


	//}

	//fill in first row of SPmatrix with values from adjacency matrix
	// fill in all rows in the col where node visits itself 







	return 0;
}