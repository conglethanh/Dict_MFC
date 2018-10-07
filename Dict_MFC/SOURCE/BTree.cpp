#include "stdafx.h"
#include "BTree.h"

BTree::BTree(int t)
{
	this->root = NULL;
	this->t = t;
}

BTree::BTree()
{
	this->root = NULL;
	this->t = 10;
}

void BTree::traverse()
{
	if (root != NULL) root->traverse();
}

void BTree::traverse2(vector<wstring> &res, wstring k)
{
	if (root != NULL) root->traverse2(res, k);
}

wstring BTree::search(wstring k)
{
	return (root == NULL) ? L"Không tìm thấy!" : root->search(k);
}

void BTree::insert(Item k)
{
	if (root == NULL)
	{
		root = new BTreeNode(t, true);
		root->keys[0] = k;
		root->n = 1;
	}
	else
	{
		if (root->n == 2 * t - 1)
		{
			BTreeNode *s = new BTreeNode(t, false);

			s->C[0] = root;

			s->splitChild(0, root);

			int i = 0;
			if (s->keys[0].eng < k.eng)
				i++;
			s->C[i]->insertNonFull(k);

			root = s;
		}
		else
			root->insertNonFull(k);
	}
}

void BTree::remove(wstring k)
{
	if (!root)
	{
		cout << "The tree is empty\n";
		return;
	}

	root->remove(k);

	if (root->n == 0)
	{
		BTreeNode *tmp = root;
		if (root->leaf)
			root = NULL;
		else
			root = root->C[0];

		delete tmp;
	}
	return;
}

void BTree::setRootNULL()
{
	this->root = NULL;
}

void BTree::setT(int t)
{
	this->t = t;
}