#include "stdafx.h"
#include "BTreeNode.h"

BTreeNode::BTreeNode(int t, bool leaf)   // Constructor
{
	this->t = t;
	this->leaf = leaf;

	keys = new Item[2 * t - 1];
	C = new BTreeNode *[2 * t];

	n = 0;
}

//duyet
void BTreeNode::traverse()
{
	int i;
	for (i = 0; i < n; i++)
	{
		if (leaf == false)
			C[i]->traverse();
		wcout << L"    " << keys[i].eng << L"-" << keys[i].viet;
	}

	if (leaf == false)
		C[i]->traverse();
}

void BTreeNode::traverse2(vector<wstring>& res, wstring k)
{
	int i;
	for (i = 0; i < n; i++)
	{
		if (leaf == false)
			C[i]->traverse2(res, k);

		if (keys[i].eng.substr(0, k.length()) == k)
			res.push_back(keys[i].eng);
	}

	if (leaf == false)
		C[i]->traverse2(res, k);
}

//tim kiem
wstring BTreeNode::search(wstring k)
{
	int i = 0;
	while (i < n && k > keys[i].eng)
		i++;

	if (keys[i].eng == k)
		return keys[i].viet;

	if (leaf == true)
		return L"Không tìm thấy!";

	return C[i]->search(k);
}

// them
void BTreeNode::splitChild(int i, BTreeNode *y)
{
	BTreeNode *z = new BTreeNode(y->t, y->leaf);
	z->n = t - 1;

	for (int j = 0; j < t - 1; j++)
		z->keys[j] = y->keys[j + t];

	if (y->leaf == false)
	{
		for (int j = 0; j < t; j++)
			z->C[j] = y->C[j + t];
	}

	y->n = t - 1;

	for (int j = n; j >= i + 1; j--)
		C[j + 1] = C[j];

	C[i + 1] = z;

	for (int j = n - 1; j >= i; j--)
		keys[j + 1] = keys[j];

	keys[i] = y->keys[t - 1];

	n = n + 1;
}

void BTreeNode::insertNonFull(Item k)
{
	int i = n - 1;

	if (leaf == true)
	{
		while (i >= 0 && keys[i].eng > k.eng)
		{
			keys[i + 1] = keys[i];
			i--;
		}

		keys[i + 1] = k;
		n = n + 1;
	}
	else
	{
		while (i >= 0 && keys[i].eng > k.eng)
			i--;

		if (C[i + 1]->n == 2 * t - 1)
		{
			splitChild(i + 1, C[i + 1]);

			if (keys[i + 1].eng < k.eng)
				i++;
		}
		C[i + 1]->insertNonFull(k);
	}
}


int BTreeNode::findKey(wstring k)
{
	int idx = 0;
	while (idx < n && keys[idx].eng < k)
		++idx;
	return idx;
}

Item BTreeNode::getPred(int idx)
{
	BTreeNode *cur = C[idx];
	while (!cur->leaf)
		cur = cur->C[cur->n];

	return cur->keys[cur->n - 1];
}

Item BTreeNode::getSucc(int idx)
{
	BTreeNode *cur = C[idx + 1];
	while (!cur->leaf)
		cur = cur->C[0];

	return cur->keys[0];
}

void BTreeNode::merge(int idx)
{
	BTreeNode *child = C[idx];
	BTreeNode *sibling = C[idx + 1];

	child->keys[t - 1] = keys[idx];

	for (int i = 0; i<sibling->n; ++i)
		child->keys[i + t] = sibling->keys[i];

	if (!child->leaf)
	{
		for (int i = 0; i <= sibling->n; ++i)
			child->C[i + t] = sibling->C[i];
	}

	for (int i = idx + 1; i<n; ++i)
		keys[i - 1] = keys[i];

	for (int i = idx + 2; i <= n; ++i)
		C[i - 1] = C[i];

	child->n += sibling->n + 1;
	n--;

	delete(sibling);
	return;
}

void BTreeNode::removeFromLeaf(int idx)
{
	for (int i = idx + 1; i<n; ++i)
		keys[i - 1] = keys[i];

	n--;
	return;
}

void BTreeNode::removeFromNonLeaf(int idx)
{
	Item k = keys[idx];

	if (C[idx]->n >= t)
	{
		Item pred = getPred(idx);
		keys[idx] = pred;
		C[idx]->remove(pred.eng);
	}

	else if (C[idx + 1]->n >= t)
	{
		Item succ = getSucc(idx);
		keys[idx] = succ;
		C[idx + 1]->remove(succ.eng);
	}

	else
	{
		merge(idx);
		C[idx]->remove(k.eng);
	}
	return;
}

void BTreeNode::borrowFromPrev(int idx)
{
	BTreeNode *child = C[idx];
	BTreeNode *sibling = C[idx - 1];

	for (int i = child->n - 1; i >= 0; --i)
		child->keys[i + 1] = child->keys[i];

	if (!child->leaf)
	{
		for (int i = child->n; i >= 0; --i)
			child->C[i + 1] = child->C[i];
	}

	child->keys[0] = keys[idx - 1];

	if (!leaf)
		child->C[0] = sibling->C[sibling->n];

	keys[idx - 1] = sibling->keys[sibling->n - 1];

	child->n += 1;
	sibling->n -= 1;

	return;
}

void BTreeNode::borrowFromNext(int idx)
{
	BTreeNode *child = C[idx];
	BTreeNode *sibling = C[idx + 1];

	child->keys[(child->n)] = keys[idx];

	if (!(child->leaf))
		child->C[(child->n) + 1] = sibling->C[0];

	keys[idx] = sibling->keys[0];

	for (int i = 1; i<sibling->n; ++i)
		sibling->keys[i - 1] = sibling->keys[i];

	if (!sibling->leaf)
	{
		for (int i = 1; i <= sibling->n; ++i)
			sibling->C[i - 1] = sibling->C[i];
	}

	child->n += 1;
	sibling->n -= 1;

	return;
}

void BTreeNode::fill(int idx)
{
	if (idx != 0 && C[idx - 1]->n >= t)
		borrowFromPrev(idx);

	else if (idx != n && C[idx + 1]->n >= t)
		borrowFromNext(idx);

	else
	{
		if (idx != n)
			merge(idx);
		else
			merge(idx - 1);
	}
	return;
}

void BTreeNode::remove(wstring k)
{
	int idx = findKey(k);

	if (idx < n && keys[idx].eng == k)
	{
		if (leaf)
			removeFromLeaf(idx);
		else
			removeFromNonLeaf(idx);
	}
	else
	{

		if (leaf)
		{
			wcout << "The key " << k << " is does not exist in the tree\n";
			return;
		}

		bool flag = ((idx == n) ? true : false);

		if (C[idx]->n < t)
			fill(idx);

		if (flag && idx > n)
			C[idx - 1]->remove(k);
		else
			C[idx]->remove(k);
	}
	return;
}