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

	return 0;
}