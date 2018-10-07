#pragma once
#include "BTreeNode.h"

class BTree
{
	BTreeNode *root = NULL; // Pointer to root node
	int t = 10;  // Minimum degree
public:
	BTree(int t);

	BTree();

	void traverse();

	void traverse2(vector<wstring> &res, wstring k);

	wstring search(wstring k);

	void insert(Item k);

	void remove(wstring k);

	void setRootNULL();

	void setT(int t);
};