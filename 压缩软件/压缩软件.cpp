// 压缩软件.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<fstream>
#include<time.h>
#include"huffmantree.h"
using namespace std;
const int MAX = 255;
void Count(int *count, unsigned char *buf,int filelen);//计算频率
HuffmanTree BuildHuffTree(int *count,int filelen, unsigned char *buf);//建立huffman树
vector<vector<int>> GetandOutputcode(HuffmanTree tree1, int filelen,unsigned char*buf);//获得并输出编码
void Binarycode(int filelen,unsigned char *buf, vector<vector<int>>&code);//压缩编码
char Transinttochar(int *buffer);//将8位01序列转换为ASCII码
void Transchartoint(int x, int *a);//将数字转换为二进制
int Translate(HuffmanTree tree1);//对B1进行译码并写入文件C
//void Write(HuffmanTree tree1, vector<int> &source);//直接对编码解码并写入D
int main()
{
	//读取文件名
	cout << "请输入要压缩的文件名：";
	char name[MAX];
	cin.getline(name,MAX);

	//计算程序执行时间
	clock_t start, finish;
	double totaltime;
	start = clock();

	//读取文件中的字符
	FILE *file1 = NULL;
	file1 = fopen(name, "rb");//只读方式打开文件
	if (file1 == NULL)
	{
		cout << "文件打开失败！";
		exit(1);
	}
	cout << "文件打开成功" << endl;

	//获得文件长度
	fseek(file1, 0, SEEK_END); //文件指针移到末尾
	int filelen = ftell(file1); //获得文件当前指针位置，即为文件长度
	rewind(file1); //将文件指针移到开头，准备读取
	unsigned char *buf = (unsigned char *)malloc(filelen); //新建缓冲区，存储读出的数据
	for (int i = 0; i < filelen; i++)//将缓冲区的数据设置为0
		buf[i] = 0;
	fread(buf, 1, filelen, file1);//读取文件
	fclose(file1);//关闭文件
	cout << "文件读取完毕" << endl;
	int count[MAX];//记录每个编码出现次数
	Count(count,buf, filelen);//计算频率
	
	HuffmanTree tree1 = BuildHuffTree(count, filelen, buf);//建立huffman树
	cout << "huffman树已建立" << endl;
	
	vector<vector<int>>code(MAX);//获取编码
	for (int i = 0; i < MAX; i++)
		code[i] = tree1.GetCode(i);
	//code= GetandOutputcode(tree1, filelen, buf);//获得并输出编码
	//cout << "编码以01序列的形式储存在B1……" << endl;

	Binarycode(filelen, buf, code);//输出压缩编码
	cout << "压缩编码已存入B" << endl;

	int filelenB=Translate(tree1);//对B1进行译码并写入文件C
	cout << "译码已存入文件C" << endl;
	double rate=(double)filelenB/filelen;
	cout << "压缩比为："<<rate << endl;
	//Write(tree1, source);//直接对编码解码并写入D
	free(buf);//释放缓冲区

	//输出程序运行时间
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "此程序的运行时间为" << totaltime << "秒！" << endl;
	return 0;
}
//计算频率
void Count(int *count,unsigned char *buf,int filelen)
{
	for (int i = 0; i < MAX; i++)
		count[i] = 0;
	//记录每个8位二进制串出现次数
	for (int i = 0; i < filelen; i++)
		count[buf[i]]++;
}
//建立huffman树
HuffmanTree BuildHuffTree(int *count,int filelen,unsigned char *buf)
{
	for (int i = 0; i < MAX; i++)
		count[i] = 0;
	//记录每个8位二进制串出现次数
	for (int i = 0; i < filelen; i++)
		count[buf[i]]++;
	//建立huffman树
	vector< HuffmanNode> leafs(MAX);//对每个ASCII码进行huffman编码
	for (int i = 0; i < 255; i++)//每个结点的符号
	{
		leafs[i].data = i;
	}
	for (int i = 0; i < MAX; i++)//每个结点的权值，为其出现的频率
		leafs[i].weight = count[i];
	HuffmanTree tree1(leafs);//建立huffman树
	return tree1;
}
//获得编码
vector<vector<int>> GetandOutputcode(HuffmanTree tree1,int filelen,unsigned char*buf)
{
	//对文件中字符进行编码
	vector<vector<int>> code(MAX);
	for (int i = 0; i < MAX; i++)
	{
		code[i] = tree1.GetCode(i);
	}
	//输出编码到文件B
	/*ofstream file2("B1.txt");
	if (!file2.is_open())
	{
		cout << "文件B1打开失败！";
		exit(1);
	}
	//展示编码
	for (int i = 0; i < filelen; i++)
	{
		int x;//code下标
		x = buf[i];
		for (unsigned int j = 0; j < code[x].size(); j++)
		{
			file2 << code[x][j];
			//source.push_back(code[x][j]);
		}
	}
	file2.close();*/
	return code;
}
//压缩编码
void Binarycode(int filelen,unsigned char *buf,vector<vector<int>>&code)
{
	ofstream file4("B.txt");
	if (!file4.is_open())
	{
		cout << "文件B打开失败！";
		exit(1);
	}
	int buffer[8];//用作缓冲区
	for(int i=0;i<8;i++)
		buffer[i] = 0;
	int bufcount = 0;//缓冲字节数，初始为0
	//输出编码到文件B
	for (int i = 0; i < filelen; i++)
	{
		int x;//code下标
		x = buf[i];
		for (unsigned int j = 0; j < code[x].size(); j++)
		{
			if (bufcount == 8)
			{
				file4 << Transinttochar(buffer);
				bufcount = 0;
			}
			buffer[bufcount] = code[x][j];
			bufcount++;
		}
	}
	//最后一位
	if (bufcount != 0)
	{
		file4 << Transinttochar(buffer);
	}
	char lackcount = bufcount;//缺位数，以char的形式放最后
	file4 << lackcount;
	file4.close();
}
//对B进行译码并写入文件C
int Translate(HuffmanTree tree1)
{
	//读出文件B的内容
	FILE *file1 = NULL;
	file1 = fopen("B.txt", "rb");//只读方式打开文件
	if (file1 == NULL)
	{
		cout << "文件B打开失败！";
		exit(1);
	}

	//获得文件长度
	fseek(file1, 0, SEEK_END); //文件指针移到末尾
	int filelenB = ftell(file1); //获得文件当前指针位置，即为文件长度
	//读取文件
	rewind(file1); //将文件指针移到开头，准备读取
	unsigned char *buf1 = (unsigned char *)malloc(filelenB); //新建缓冲区，存储读出的数据
	for (int i = 0; i < filelenB; i++)//将缓冲区的数据设置为0
		buf1[i] = 0;
	fread(buf1, 1, filelenB, file1);//读取文件
	fclose(file1);//关闭文件

	//对编码进行译码
	vector<int>source1;//读出的编码
	int a[8];//储存数字
	for (int i = 0; i < 8; i++)
		a[i] = 0;
	for (int i = 0; i < filelenB-2; i++)
	{
		if (buf1[i] == 13 && buf1[i + 1] == 10)//如果遇到回车换行符就略过回车符
			continue;
		Transchartoint(buf1[i],a);
		for (int j = 0; j < 8; j++)
			source1.push_back(a[j]);
	}
	//倒数第二位是一个字符，最后一位是缺位数，字符的二进制只读到缺位数
	int lackcount = buf1[filelenB - 1];
	Transchartoint(buf1[filelenB-2], a);
	for (int j = 0; j < lackcount; j++)
		source1.push_back(a[j]);

	//输入文件C
	ofstream file3("C.txt", ios::binary);
	if (!file3.is_open())
	{
		cout << "文件C打开失败！";
		exit(1);
	}
	vector<char> result;
	tree1.Decode(source1, result);
	for (unsigned int i = 0; i < result.size(); i++)
	{
		file3 << result[i];
	}
	file3.close();

	return filelenB;
}
//将8位01序列转换为ASCII码
char Transinttochar(int *buffer)
{
	char x;	
	x = buffer[0]*128+buffer[1] * 64 + buffer[2] * 32 + buffer[3] * 16 + buffer[4] * 8 + buffer[5] * 4 + buffer[6] * 2 + buffer[7] * 1;
	return x;
}
//将数字转换为二进制
void Transchartoint(int x,int *a)
{
	for (int i = 7; i >= 0; i--)
	{
		a[i] = x % 2;
		x = x / 2;
	}
}
//直接对B编码解码并写入D
/*void Write(HuffmanTree tree1, vector<int> &source)
{
	//对编码进行译码并输入文件C
	ofstream file3("D.txt", ios::binary);
	if (!file3.is_open())
	{
		cout << "文件D打开失败！";
		exit(1);
	}
	vector<char> result;
	tree1.Decode(source, result);
	for (unsigned int i = 0; i < result.size(); i++)
	{
		file3 << result[i];
	}
	file3.close();
}*/
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
