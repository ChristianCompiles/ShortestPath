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

int main()
{
	GraphMatrix<char> g;
	string header = setupMatrix(g, "graphInput.txt"); // pass object and file to set up matrix with
	g.printAdjacencyMatrix();
	
	char startNode;
	cout << "Start node: ";
	std::cin >> startNode;
	bool flag = 0;
	for (int i = 0; i < header.size(); i++) // make sure user input node is in the graph
	{
		if (startNode == header[i]) // case sensitive
		{
			flag = 1;
			break;
		}
	}

	if (flag) // user input is good
	{
		// first distance is always 0
		cout << startNode << ": " << g.getLink(startNode, startNode) << "(" << startNode << "), ";

		for (int i = 0; i < header.size(); i++)
		{
			
		}
	}
	else
	{
		cout << "Start node: " << startNode << " not in graph\n";
	}
	







	return 0;
}