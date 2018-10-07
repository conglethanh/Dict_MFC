#include "stdafx.h"
#include "HuffmanCompression.h"

void HuffmanCompression::convertTree()
{
	vector<Node*> tree;
	// Lập bảng tần số
	// Duyệt hết chuỗi để kiểm tra
	for (int i = 0; i < data.length(); i++)
	{
		bool existed = false;
		// Duyệt trong bảng, nếu ký tự ta xét đã có trong bảng thì ++freq
		for (int j = 0; j < tree.size(); j++)
		{
			if (tree[j]->c == data[i])
			{
				tree[j]->freq++;
				existed = true;
				break;
			}
		}
		// Ngược lại thì thêm ký tự đó vào bảng
		if (!existed)
		{
			Node* node = new Node();
			node->c = data[i];
			node->freq = 1;
			tree.push_back(node);
		}
	}

	// Tiếp theo là sắp xếp lại bảng tần số
	//(trọng số nhỏ bên trái, lớn bên phải, nếu bằng nhau thì ascii nhỏ bên trái, lớn bên phải)
	for (int i = 0; i < tree.size() - 1; i++)
	{
		for (int j = i + 1; j < tree.size(); j++)
		{
			if (tree[i]->freq > tree[j]->freq)
			{
				Node* temp = tree[i];
				tree[i] = tree[j];
				tree[j] = temp;
			}
		}
	}

	while (true)
	{
		// Tạo node z có con là 2 phần tử đầu trong bảng tần số
		Node* tmp = new Node();
		tmp->left = tree[0];
		tmp->right = tree[1];
		tmp->freq = tmp->left->freq + tmp->right->freq;

		// Xoá 2 phần tử đầu trong bảng tần số
		tree.erase(tree.begin(), tree.begin() + 2); // [0,2)
		tree.resize(tree.size() + 1);

		// Nếu chỉ còn đúng 1 phần tử
		if (tree.size() == 1)
		{
			tree[0] = tmp;
			break;
		}
		else
		{
			//vector insert
			// Chèn vị trí node z vào bảng tần số sao cho phù hợp
			// Ở đây mình sắp xếp giảm dần
			for (int j = 0; j < tree.size() - 1; j++)
			{
				bool isMax = true;
				if (tree[j]->freq > tmp->freq)
				{
					for (int k = tree.size() - 1; k > j; k--)
					{
						tree[k] = tree[k - 1];
					}
					tree[j] = tmp;
					isMax = false;
					break;
				}
				if (isMax) tree[tree.size() - 1] = tmp;
			}
		}
	}

	// Cuối cùng ta thu được cây Huffman là phần tử duy nhất còn lại trong bảng tần số
	root = tree[0];
}

void HuffmanCompression::generateTree(Node * curr)
{
	if (curr != NULL)
	{
		// Nếu node là lá
		if (curr->left == NULL && curr->right == NULL)
		{
			bitTree += '1';
			bitset<16> bitSq(curr->c);
			bitTree += bitSq.to_string();
		}
		// Ngược lại
		else
		{
			bitTree += '0';
			generateTree(curr->left);
			generateTree(curr->right);
		}
	}
}

void HuffmanCompression::visit(Node * curr, string bit)
{
	if (curr != NULL)
	{
		if (curr->left == NULL || curr->right == NULL)
		{
			Bit cBit;
			cBit.c = curr->c;
			cBit.bit = bit;
			this->bit.push_back(cBit);
		}
		else
		{
			visit(curr->left, bit + "0");
			visit(curr->right, bit + "1");
		}
	}
}

HuffmanCompression::HuffmanCompression()
{
	bitTree = "";
}

HuffmanCompression::HuffmanCompression(string filePath)
{
	// đọc file tiếng Việt (định dạng UTF-8 without BOM)
	std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	std::wifstream fin(filePath, ios::binary);
	if (!fin) {
		std::wcout << L"Không thể đọc file\n";
	}
	else {
		fin.imbue(loc);

		fin.seekg(0, fin.end);
		wchar_t* s = new wchar_t[fin.tellg()];
		fin.seekg(0, fin.beg);

		int i = 0;
		wchar_t c;

		while (fin >> noskipws >> c)
		{
			s[i] = c;
			i++;
		}
		s[i] = '\0';
		data = (wstring)s;
		delete s;

		bitTree = "";
		fin.close();
	}
}

wchar_t convertBitToChar(string input)
{
	wchar_t c = '\0';
	for (int i = 0; i < input.length(); i++)
	{
		c = (c << 1) | (input[i] - 48);
	}
	return c;
}

wstring convertBitStringToCharString(string input)
{
	wstring result;
	while (input.length() > 0)
	{
		string temp;
		if (input.length() > 16)
		{
			temp = input.substr(input.length() - 16, 16);
			input = input.erase(input.length() - 16, 16);
		}
		else
		{
			temp = input;
			if (temp.length() > 16) temp = "0" + temp;
			input = "";
		}
		result = convertBitToChar(temp) + result;
	}
	return result;
}

// Hàm dùng để chuyển chuỗi 8 bit thành ký tự tương ứng
char convertBitToChar1(string input)
{
	char c = 0;
	for (int i = 0; i < input.length(); i++)
	{
		c = (c << 1) | (input[i] - 48);
	}
	return c;
}

// Hàm dùng để chuyển chuỗi bit sang chuỗi ký tự tương ứng
string convertBitStringToCharString1(string input)
{
	string result = "";
	while (input.length() > 0)
	{
		string temp;
		if (input.length() > 8)
		{
			temp = input.substr(input.length() - 8, 8);
			input = input.erase(input.length() - 8, 8);
		}
		else
		{
			temp = input;
			if (temp.length() > 8) temp = "0" + temp;
			input = "";
		}
		result = convertBitToChar1(temp) + result;
	}
	return result;
}


void HuffmanCompression::compression(string outputPath)
{
	this->convertTree();
	this->visit(root, "");
	this->generateTree(root);

	// Thay thế các kí tự trong chuỗi data thành mã bit tương ứng trong bảng tần số
	string bitSq = "";
	for (int i = 0; i < data.length(); i++)
	{
		for (int j = 0; j < bit.size(); j++)
		{
			if (data[i] == bit[j].c)
			{
				bitSq += bit[j].bit;
				break;
			}
		}
	}

	// Mã hoá bit cây Huffman và bit data thành ký tự để lưu vào file
	wstring main = convertBitStringToCharString(bitTree + bitSq);
	int realBit = 0, sizeBit = (bitTree + bitSq).length();

	// Do độ dài bit của bitTree và bitSq không là bội của 8
	// Ta cần xác định có bao nhiêu bit '0' được thêm vào chuỗi bit bitTree và bitSq
	while (realBit < sizeBit) realBit += 16;
	wchar_t addBit = realBit - sizeBit;
	// Ta cần phải xác định số lượng ký tự có trong chuỗi 
	wchar_t bitNum = this->bit.size();

	wchar_t treeSize = bitTree.length();

	// lưu file tiếng Việt (UTF-8 without BOM)
	std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);  // UTF-8
	std::wofstream fout(outputPath);
	if (!fout)
	{
		std::wcout << L"Không thể tạo file\n";
	}
	else
	{
		fout.imbue(loc);
		fout << (wchar_t)bitNum << (wchar_t)treeSize << (wchar_t)addBit << main;
		fout.close();
	}
}