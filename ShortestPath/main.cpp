#include "graph_matrix.h"
#include <iostream>
#include <fstream>

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

			if (line.size() - (line.size()-1) > sizeGraph) // more columns than expected
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

void signalNodeVisited(int* ptr, int lastRow, int lastCol, int matrixSize) // set rows to -1 to signal that node been visited
{
	for (int i = lastRow + 1; i < matrixSize; i++) // iterate over rows of col
	{
		ptr[lastCol + matrixSize * i] = -1; // [i][const col]
	}
}

void changeAllValues(int* ptr, int matrixSize)
{
	for (int i = 0; i < matrixSize * matrixSize; i++)
	{
		ptr[i] = -4;
	}
}
void printSPmatrix(int* ptr, int matrixSize)
{
	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++)
		{
			cout << ptr[j + matrixSize * i] << " "; // [i][j]
		}
		cout << endl;
	}
}
int main()
{
	GraphMatrix<char> g;
	string header = setupMatrix(g, "graphInput.txt"); // pass object and file to set up matrix with
	int matrixSize = header.size();
	g.printAdjacencyMatrix();
	
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
	
	// first distance is always 0
	cout << startNode << ": " << g.getLink(startNode, startNode) << "(" << startNode << "), ";
	cout << "index of c " << g.getNodeIndex('c') << std::endl;
	int* SPmatrix = new int[matrixSize * matrixSize]; // hold shortest path matrix
	int nodeIndex;

	for (int i = 0; i < matrixSize * matrixSize; i++) // initialize values to -2
	{
		SPmatrix[i] = -2; // initialization value
	}

	for (int j = 0; j < matrixSize; j++) // place values in first row
	{
		SPmatrix[j] = g.getLink(startNode, header[j]); // [0][j]
	}
	signalNodeVisited(SPmatrix, g.getNodeIndex(startNode), g.getNodeIndex(startNode), matrixSize);

	printSPmatrix(SPmatrix, matrixSize); // check results of matrix so far
	
	changeAllValues(SPmatrix, matrixSize);

	printSPmatrix(SPmatrix, matrixSize);

	//for (int i = 1; i < header.size(); i++) // traverse all rows besides first row of SPmatrix
	//{
	//	for (int j = 0; j < header.size(); j++)
	//	{
	//		if (i == 0)
	//		{
	//			nodeIndex = 
	//			SPmatrix[j+header.size() * i] = g.// [i][j]
	//		}
	//		cout << SPmatrix[j + header.size() * i] << " ";
	//	}
	//	cout << "\n";
	//	//nodeIndex = g.getNodeIndex(startNode);


	//}

	 //fill in first row of SPmatrix with values from adjacency matrix
	 // fill in all rows in the col where node visits itself 
	 






	return 0;
}