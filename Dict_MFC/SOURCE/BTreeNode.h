#pragma once
#include <iostream>
using namespace std;
#include <vector>
#include <fcntl.h> 
#include <io.h>    
#include <locale>
#include <codecvt> 

struct Item
{
	wstring eng;
	wstring viet;
	Item()
	{
		eng = L'\0';
		viet = L'\0';
	}
};

class BTreeNode
{
	Item *keys;  // An array of keys
	int t;      // Minimum degree (defines the range for number of keys)
	BTreeNode **C; // An array of child pointers
	int n;     // Current number of keys
	bool leaf; // Is true when node is leaf. Otherwise false
public:
	BTreeNode(int t, bool leaf);   // Constructor

	//duyet
	void traverse();

	void traverse2(vector<wstring> &res, wstring k);

	//tim kiem
	wstring search(wstring k);

	// them
	void splitChild(int i, BTreeNode *y);

	void insertNonFull(Item k);

	//xoa
	int findKey(wstring k);

	Item getPred(int idx);

	Item getSucc(int idx);

	void merge(int idx);

	void removeFromLeaf(int idx);

	void removeFromNonLeaf(int idx);

	void borrowFromPrev(int idx);

	void borrowFromNext(int idx);

	void fill(int idx);

	void remove(wstring k);

	friend class BTree;
};