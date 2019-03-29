#pragma once
#include<vector>
#include<string>
using namespace std;
struct HuffmanNode	// 哈夫曼树结点的定义
{
	char data; // 待编码的符号		
	double weight;		// 符号出现的频率 
	int parent, lchild, rchild;	// 父结点、左、右孩子结点的位置
};

class HuffmanTree
{
private:
	vector<HuffmanNode> hufftree;	// 树中所有结点的存储空间
	int n;						// 叶子结点数
	void SelectSmall(int &least, int &less, int i);//选择一组数据中最小的和次小的数
public:
	HuffmanTree(vector<HuffmanNode> &leafs);//构造函数
	vector<int>GetCode(int i);//huffman编码算法
	void Decode(vector<int> &source, vector<char>&result);//huffman译码算法
};

