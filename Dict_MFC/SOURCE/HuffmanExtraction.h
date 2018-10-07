#pragma once
#include "HuffmanCompression.h"
#include "BTree.h"

class HuffmanExtraction
{
private:
	string bitTree;
	string data;
	Node* root;
	void generateTree(Node* curr);
	wchar_t visit(Node* curr);
public:
	HuffmanExtraction();
	HuffmanExtraction(string filePath);
	void extraction(string outputPath);
	void extraction(BTree &tree);
};