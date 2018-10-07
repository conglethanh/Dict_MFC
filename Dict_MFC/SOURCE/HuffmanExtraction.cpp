#include "stdafx.h"
#include "HuffmanExtraction.h"

void HuffmanExtraction::generateTree(Node * curr)
{
	while (bitTree.length() > 0)
	{
		// Nếu là node lá
		if (curr->left != NULL && curr->right != NULL) return;

		Node* node = new Node();
		if (bitTree[0] == '0')
		{
			bitTree.erase(0, 1);
			// Ta cần phải xác định xem mình nên gọi đệ quy đến node trái hay phải
			if (curr->left == NULL)
			{
				curr->left = node;
				generateTree(curr->left);
			}
			else
			{
				curr->right = node;
				generateTree(curr->right);
			}
		}

		// Nếu gặp bit '1', ta get 16 bit kế tiếp
		else
		{
			//16 bit
			string temp = bitTree.substr(1, 16);
			bitTree.erase(0, 17);

			// Hàm char convertBitToChar(string temp) dùng để xuất ra ký tự tương ứng với chuỗi bit
			node->c = convertBitToChar(temp);
			if (curr->left == NULL) curr->left = node;
			else curr->right = node;
		}
	}
}

wchar_t HuffmanExtraction::visit(Node * curr)
{
	if (curr->left == NULL && curr->right == NULL)
	{
		return curr->c;
	}
	if (data[0] == '0')
	{
		data.erase(0, 1);
		visit(curr->left);
	}
	else
	{
		data.erase(0, 1);
		visit(curr->right);
	}
}

HuffmanExtraction::HuffmanExtraction()
{
	root = new Node();
}

HuffmanExtraction::HuffmanExtraction(string filePath)
{
	root = new Node();
	string bitSequence = "";

	std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);

	wifstream fin(filePath, ios::binary);

	if (!fin) {
		std::wcout << L"Không thể đọc file\n";
		return;
	}

	fin.imbue(loc);

	wchar_t c;
	// Ta không thể đọc 1 lần toàn bộ kí tự trong file
	// Vì có thể đâu đó trong file có chứa kí tự '\0'
	// Nếu ta cố tình đọc thì chuỗi bitSequence sẽ không thể lưu hết được
	// Vì vậy ta cần phải đọc từng ký tự trong file
	while (fin >> noskipws >> c)
	{
		bitset<16> bit(c);
		bitSequence += bit.to_string();
	}

	// Lấy ra số ký tự có trong chuỗi mã hoá
	wchar_t numChar = convertBitToChar(bitSequence.substr(0, 16));
	bitSequence.erase(0, 16);

	wchar_t treeSize = convertBitToChar(bitSequence.substr(0, 16));
	bitSequence.erase(0, 16);

	// Lấy ra số bit '0' được thêm vào như phía trên mình đề cập
	wchar_t addBit = convertBitToChar(bitSequence.substr(0, 16));
	bitSequence.erase(0, 16);

	// Bỏ đi các bit '0' thừa
	bitSequence.erase(0, addBit);

	// Số bit cần lấy tuân theo công thức dưới đây
	int sizeBit = treeSize;
	//int sizeBit = 20 * numChar - 2;
	bitTree = bitSequence.substr(0, sizeBit);
	bitSequence.erase(0, sizeBit);
	data = bitSequence;

	root = new Node();
	fin.close();
}

void HuffmanExtraction::extraction(string outputPath)
{
	this->generateTree(root);
	root = root->left;

	std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	std::wofstream fout(outputPath);
	if (!fout)
	{
		std::wcout << L"Không thể tạo file\n";
	}
	else
	{
		fout.imbue(loc);
		while (data.length() > 0)
		{
			fout << this->visit(root);
		}
		fout.close();
	}
}

void HuffmanExtraction::extraction(BTree &tree)
{
	this->generateTree(root);
	root = root->left;

	wstring result;
	Item item;
	wchar_t t;
	int demHaiCham = 0;
	while (data.length() > 0)
	{
		t = this->visit(root);
		if (t == L':'&&demHaiCham == 0)
		{
			item.eng = result;
			result.clear();
			demHaiCham++;
		}
		else if (t == L'\n')
		{
			item.viet = result;
			result.clear();
			tree.insert(item);
			demHaiCham = 0;
		}
		else result += t;
	}
}