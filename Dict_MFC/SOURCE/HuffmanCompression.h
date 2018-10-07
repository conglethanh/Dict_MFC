#pragma once
#include <iostream>
using namespace std;
#include <string>
#include <vector>
#include <bitset>
#include <fstream>

#include <fcntl.h> 
#include <io.h>    
#include <locale>
#include <codecvt> 

struct Node
{
	wchar_t c;
	int freq;
	Node* left;
	Node* right;
	Node()
	{
		c = '\0';
		freq = -1;
		left = NULL;
		right = NULL;
	}
};

// Struct này có công dụng lưu trữ bit đã mã hoá theo mã tiền tố
struct Bit
{
	wchar_t c;
	string bit;
};

class HuffmanCompression
{
private:
	wstring data; // Lưu trữ chuỗi đọc vào từ file
	Node* root;
	vector<Bit> bit; // Lưu trữ mã bit tương ứng với ký tự ta xét
	string bitTree; // Cây Huffman được mã hoá để lưu vào file
	void convertTree();
	void generateTree(Node* curr);
	void visit(Node* curr, string bit);
public:
	HuffmanCompression();
	HuffmanCompression(string filePath);
	void compression(string outputPath);
};
char convertBitToChar1(string input); //8 bit
wchar_t convertBitToChar(string input); //16 bit
wstring convertBitStringToCharString(string input); //16 bit
string convertBitStringToCharString1(string input);