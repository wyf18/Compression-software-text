#include "pch.h"
#include "HuffmanTree.h"
void HuffmanTree::SelectSmall(int &least, int &less, int i)
{
	int a[2];
	double b[2];
	int k = 0, j = 0;
	//先假定数组的前两位中的较小的是least，较大的是less
	for (j; j < i; j++)//找出前两位parent为-1的
	{
		if (hufftree[j].parent == -1)
		{
			b[k] = hufftree[j].weight;
			a[k] = j;
			k++;
		}
		if (k == 2)
			break;
	}
	if (b[0] > b[1])
	{
		least = a[1];
		less = a[0];
	}
	else
	{
		least = a[0];
		less = a[1];
	}
	//循环检查其余的元素
	//若新元素小于最小，则，最小变为次小，新元素成为最小
	//否则，再判断新元素是否小于次小，是，则新元素变为次小
	for (++j; j < i; j++)
	{
		if (hufftree[j].parent == -1)
		{
			if (hufftree[j].weight < hufftree[least].weight)
			{
				less = least;
				least = j;

			}
			else if (hufftree[j].weight < hufftree[less].weight)
			{
				less = j;
			}
		}
	}
}
HuffmanTree::HuffmanTree(vector< HuffmanNode> &leafs)		//构造函数
{
	n = leafs.size();
	hufftree.resize(2 * n - 1);
	for (int i = 0; i < n; i++)	 // 初始化n个二叉树
	{
		hufftree[i].data = leafs[i].data;
		hufftree[i].weight = leafs[i].weight;
		hufftree[i].parent = hufftree[i].lchild = hufftree[i].rchild = -1;
	}
	for (int i = n; i < 2 * n - 1; i++)
	{
		int least, less;				//最小、次小的根结点下标
		SelectSmall(least, less, i);	// 找到最小、次小的根结点下标,只能找根结点，叶子结点必须去掉
		//每次根结点都会变，传leafs是错的
		hufftree[least].parent = hufftree[less].parent = i;
		hufftree[i].parent = -1;
		hufftree[i].lchild = least;
		hufftree[i].rchild = less;
		hufftree[i].weight = hufftree[least].weight + hufftree[less].weight;
	}
}

vector<int> HuffmanTree::GetCode(int i)					//Huffman编码算法
{
	vector<int> code;		// 第i个符号的编码向量
	int p = i;				// p是当前结点的下标
	int parent = hufftree[i].parent;	// parent是当前结点的父结点的下标
	while (parent != -1)		// 只有根结点的parent域为-1
	{
		if (hufftree[parent].lchild == p)
			code.insert(code.begin(), 0);	// 在编码向量首部插入0
		else
			code.insert(code.begin(), 1);	// 在编码向量首部插入1
		p = parent;
		parent = hufftree[parent].parent;	// 沿父指针上溯
	}
	return code;
}

void HuffmanTree::Decode(vector<int>&source, vector<char>&result)			//Huffman译码算法
{
	//string target = "";		// 译码的目标：原信息符号串
	int root = hufftree.size() - 1;
	int p = root;
	for (unsigned int i = 0; i < source.size(); i++)
	{
		if (source[i] == 0)
			p = hufftree[p].lchild;	// 逢0向左孩子下行
		else
			p = hufftree[p].rchild;	// 逢1向右孩子下行
		if (hufftree[p].lchild == -1 && hufftree[p].rchild == -1)
		{
			//target = target + hufftree[p].data;
			result.push_back(hufftree[p].data);
			p = root;
		}
	}
}