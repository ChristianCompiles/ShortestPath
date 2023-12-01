#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
const int BEGINNING_MATRIX_SIZE = 2;
/**
* A simple exception class to represent a request out of bounds
*/
class IndexOutOfBoundsException {
	string msg;
public:
	IndexOutOfBoundsException(string msg) { this->msg = msg; }
	string getMessage() { return msg; }
};
/**
* Helper function to initialize a matrix to all zeros
* @param matrix - The matrix to initialize
* @param size - the size of each dimension of the matrix
*/
void initializeMatrix(int* matrix, int size) {
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			matrix[i * size + j] = 0;
}
/**
* Helper function to print the contents of a matrix
* @param matrix - The matrix to print
* @param size - the size of each dimension of the matrix
*/
void printMatrix(int* matrix, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++)
			cout << (j == 0 ? "" : " ") << matrix[i * size + j];
		cout << endl;
	}
}
/**
* An adjacency matrix graph implementation
*/
template <typename TYPE>
class GraphMatrix {
private:
	int* adjacencyMatrix;
	vector<TYPE> nodes;
	int currentMatrixSize;
	/**
	* Helper function to resize the matrix when it runs out of space
	*/
	void resizeMatrix();
public:
	/**
	* Default constructor
	*/
	GraphMatrix();
	/**
	* Function to add a node to the grap. Resizes the matrix if necessary
	* @param item - The item to insert into the matrix
	*/
	void addNode(TYPE item);
	/**
	* Function to get the count of nodes in the graph
	* @return int the number of nodes in the graph
	*/
	int getNodeCount();
	/**
	* Function to retrieve the node at a given index
	* @param index - the index of the node
	* @return the item stored at the given index
	*/
	TYPE getNodeAtIndex(int index);
	/**
	* Function to add a link from a given index to a given index. The indecies
	* in question are the indicies of the nodes in the node vector
	* @param from - The index of the "from" node
	* @param to - The index of the "to" node
	* @param weight - An optional weight. Defaults to 1
	*/
	void addLink(int from, int to, int weight = 1);
	/**
	* Function to add a link from a node to a node. The nodes should have an
	* implementation of the == operator
	* @param from - The node that the link originates from
	* @param to - the node that the link terminates at
	* @param weight - An optional weight. Defaults to 1
	*/
	void addLink(TYPE from, TYPE to, int weight = 1);
	/**
	* Function to get the index of a particular node. The nodes should have an
	* implementation of the == operator.
	* @param item - The item to search for in the nodes
	*/
	int getNodeIndex(TYPE item);
	/**
	* Function to get the link status between two nodes by index.
	* @param from - The index of the node the link originates from (if link
	exists)
	* @param to - the index of the node the link terminates at (if link exists)
	* @return int - the weight of the potential link (0 means no link)
	*/
	int getLink(int from, int to);
	/**
	* Function to get the link status between two nodes.
	* @param from - The node the link originates from (if link exists)
	* @param to - the node the link terminates at (if link exists)
	* @return int - the weight of the potential link (0 means no link)
	*/
	int getLink(TYPE from, TYPE to);
	/**
	* Helper function to print the adjacency matrix
	*/
	void printAdjacencyMatrix() { printMatrix(adjacencyMatrix, nodes.size()); }
};
template <typename TYPE>
GraphMatrix<TYPE>::GraphMatrix()
{
	currentMatrixSize = BEGINNING_MATRIX_SIZE;
	adjacencyMatrix = new int[currentMatrixSize * currentMatrixSize];
	// Initialize the matrix to all zeros
	for (int i = 0; i < currentMatrixSize; i++)
		for (int j = 0; j < currentMatrixSize; j++)
			adjacencyMatrix[i * currentMatrixSize + j] = 0;
}
template <typename TYPE>
void GraphMatrix<TYPE>::addNode(TYPE item)
{
	if (nodes.size() >= currentMatrixSize) { // Need to increase the size of the matrix
			resizeMatrix();
	}
	nodes.push_back(item);
}
template <typename TYPE>
void GraphMatrix<TYPE>::addLink(int from, int to, int weight) {
	// Check that to and from indexes are in range
	if ((from < 0 || to < 0) || (from >= currentMatrixSize || to >= currentMatrixSize)) {
		// Throw an exception
		stringstream sstream;
		sstream << "Index " << from << " or " << to << "is out of range";
		throw IndexOutOfBoundsException(sstream.str());
	}
	adjacencyMatrix[from * nodes.size() + to] = weight;
}
template <typename TYPE>
void GraphMatrix<TYPE>::addLink(TYPE from, TYPE to, int weight) {
	int fromIndex = getNodeIndex(from);
	int toIndex = getNodeIndex(to);
	if (fromIndex < 0 || toIndex < 0)
	{
		stringstream sstream;
		sstream << "Node " << from << " or " << to << " was not found";
		throw IndexOutOfBoundsException(sstream.str());
	}
	addLink(fromIndex, toIndex, weight);
}
template <typename TYPE>
int GraphMatrix<TYPE>::getNodeCount() {
	return nodes.size();
}
template <typename TYPE>
TYPE GraphMatrix<TYPE>::getNodeAtIndex(int index) {
	return nodes[index];
}
template <typename TYPE>
int GraphMatrix<TYPE>::getNodeIndex(TYPE item) {
	for (int i = 0; i < nodes.size(); i++)
		if (nodes[i] == item)
			return i;
	return -1; // Item wasn't found
}
template <typename TYPE>
int GraphMatrix<TYPE>::getLink(int from, int to) {
	if (from < 0 || to < 0 || from >= nodes.size() || to >= nodes.size())
	{
		stringstream sstream;
		sstream << "Index " << from << " or " << to << "is out of range";
		throw IndexOutOfBoundsException(sstream.str());
	}
	return adjacencyMatrix[from * nodes.size() + to];
}
template <typename TYPE>
int GraphMatrix<TYPE>::getLink(TYPE from, TYPE to) {
	int fromIndex = getNodeIndex(from);
	int toIndex = getNodeIndex(to);
	if (fromIndex < 0 || toIndex < 0)
	{
		stringstream sstream;
		sstream << "Node " << from << " or " << to << " was not found";
		throw IndexOutOfBoundsException(sstream.str());
	}
	return getLink(fromIndex, toIndex);
}
// =========================================
// Private functions
template <typename TYPE>
void GraphMatrix<TYPE>::resizeMatrix()
{
	// Double the space for the matrix
	int doubleCurrentSize = currentMatrixSize * 2;
	int* temp = new int[doubleCurrentSize * doubleCurrentSize];
	initializeMatrix(temp, doubleCurrentSize);
	// Now copy the values from the original matrix into the new one
	for (int i = 0; i < currentMatrixSize; i++)
		for (int j = 0; j < currentMatrixSize; j++) 
		{
			temp[i * doubleCurrentSize + j] = adjacencyMatrix[i * currentMatrixSize+ j];
		}
	delete[] adjacencyMatrix;
	adjacencyMatrix = temp;
	currentMatrixSize = doubleCurrentSize;
}