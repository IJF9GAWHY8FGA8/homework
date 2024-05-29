// MazeTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include<windows.h>
#include <conio.h>
#include <string.h>
#include<time.h>
#include<stdio.h>
#include<dos.h>
#include<conio.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>
#include <io.h>
#include <vector>
#include <string>
#include <io.h>  
#include <iostream>  
#include <vector>  
#include <fstream>
#include <iostream>
using namespace std;
struct TagMaze
{
public:
	TagMaze()
	{

	}
public:
	char* t_name;
	int t_row;
	int t_col;
	std::vector<std::vector<char> >t_map;
	char t_map1[100][100];
};

void getFiles(string path, vector<string>& files)
{
	//文件句柄  
	intptr_t   hFile = 0;
	//文件信息，声明一个存储文件信息的结构体  
	struct _finddata_t fileinfo;
	string p;//字符串，存放路径
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)//若查找成功，则进入
	{
		do
		{
			//如果是目录,迭代之（即文件夹内还有文件夹）  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				//文件名不等于"."&&文件名不等于".."
				//.表示当前目录
				//..表示当前目录的父目录
				//判断时，两者都要忽略，不然就无限递归跳不出去了！
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			//如果不是,加入列表  
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		//_findclose函数结束查找
		_findclose(hFile);
	}
}

int Replace(std::string& str, const char* pStrOld, const char* pStrNew)
{
	int nCount = 0;
	if (pStrOld && pStrNew)
	{
		int nSourceLen = (int)strlen(pStrOld);
		if (nSourceLen == 0)
		{
			return 0;
		}
		int nReplacementLen = (int)strlen(pStrNew);
		int nIndex = (int)str.find(pStrOld, 0);
		while (nIndex > -1)
		{
			str.replace(nIndex, nSourceLen, pStrNew);
			nCount++;
			nIndex = (int)str.find(pStrOld, nIndex + nReplacementLen);
		}
	}
	return nCount;
}
std::string Mid(std::string& str, int nFirst)
{
	if (nFirst >= str.size())
	{
		return "";
	}

	std::string strTmp;
	if (nFirst <= 0)
	{
		strTmp = str.substr(0);
	}
	else
	{
		strTmp = str.substr(nFirst);
	}
	return strTmp;
}
std::string Mid(std::string& str, int nFirst, int nCount)
{
	if (nCount <= 0)
	{
		return "";
	}
	if (nFirst >= str.size())
	{
		return "";
	}

	std::string strTmp;
	if (nFirst <= 0)
	{
		strTmp = str.substr(0, nCount);
	}
	else
	{
		strTmp = str.substr(nFirst, nCount);
	}
	return strTmp;
}

int Split(std::string& str, std::vector<std::string>& strDests, const std::string& strDelims = "\r\n", bool bKeepEmptyString = false)
{
	strDests.resize(0);
	const char* pdelims = strDelims.c_str();

	if (bKeepEmptyString)
	{ // 保留空字符串时, 其实就是先找分隔符
		int nLimIndex = 0;
		int nLimLen = strDelims.size();
		int nContentIndex = 0;
		bool bContinue = true;
		while (bContinue)
		{
			nLimIndex = (int)str.find(pdelims, nContentIndex);
			if (nLimIndex == -1)
			{
				bContinue = false;
			}
			else
			{
				strDests.push_back(Mid(str, nContentIndex, nLimIndex - nContentIndex));
				nContentIndex += (nLimIndex - nContentIndex) + nLimLen;
			}
		}
		// 最后一个分隔符 后的内容加入
		strDests.push_back(Mid(str, nContentIndex));
		return strDests.size();
	}
	else
	{
		int n = 0;	// i 需要的字符索引, j 分隔符所在的索引
		int i = 0;
		int j = 0;

		while (j != std::string::npos)
		{
			i = str.find_first_not_of(strDelims.c_str(), j);
			if (i == std::string::npos || str.size() < i)
			{
				break;
			}
			j = str.find_first_of(strDelims.c_str(), i);
			strDests.push_back(std::string(str.substr(i, j - i)));
			n++;
		}
		return n;
	}
}

//summarize the data count of a file
int data_count_sum(const char* file_name) {
	FILE* fp;
	int num = 1;
	char tmp;

	fopen_s(&fp, file_name, "r");
	if (fp == NULL) {
		printf("Fail to get a correct data sum number!");
		exit(1);
	}

	while (!feof(fp)) {
		tmp = fgetc(fp);
		if (tmp == '\n')
			num++;
	}
	//fgetc(fp) && num++;
	printf("%s: %d charaters in the file\n", file_name, num);

	fclose(fp);
	return num;
}

int read_raw_hex_data(const char* path, int data_length, int* a) {
	FILE* fpRead = NULL;
	int ret = 0;
	int i = 0;
	fopen_s(&fpRead, path, "r");
	if (fpRead == NULL)
	{
		printf("Fail to read raw data file!");
		ret = -1;
		return ret;
	}

	for (i = 0; i < data_length; i++)
	{
		fscanf_s(fpRead, "%x", &a[i]);
	}
	fclose(fpRead);

	return ret;
}

TagMaze ReadMazeFromFile(std::string& file)
{
	TagMaze tagMaze;
	int dataCount = data_count_sum(file.c_str());
	string s;
	ifstream inf;     //用ifstream类定义输入对象
	inf.open(file);  //注意输出流要先打开文件！！！
	int row = 0;
	while (getline(inf, s)) //获取一行内容
	{
		std::vector<char> tlineMaps;
		int index = 0;
		for (; index < s.length(); index++)
		{
			tagMaze.t_map1[row][index] = s[index];
			tlineMaps.push_back(s[index]);
		}
		tagMaze.t_map1[row][++index] = '\0\n';
		tagMaze.t_map.push_back(tlineMaps);
		row++;
	}
	inf.close();
	tagMaze.t_col = tagMaze.t_map.at(0).size();
	tagMaze.t_row = tagMaze.t_map.size();
	return tagMaze;
}

void runMaze(TagMaze tagMaze)
{
	int i, x = 1, y = 1, p1 = 1, q1 = 5, p2 = 12, q2 = 29;
	for (int ii = 0; ii < tagMaze.t_map.size(); ii++)
	{
		vector<char> lineMap = tagMaze.t_map[ii];
		for (int cc = 0; cc < lineMap.size(); cc++)
		{
			if (lineMap[cc] == 'E')
			{
				p1 = ii;
				q1 = cc;
			}
			if (lineMap[cc] == 'S')
			{
				p2 = ii;
				q2 = cc;
			}
		}
	}
	if (p1 == p2 && q1 == q2)
	{
		printf("The maze of errors! ！！");
		return;
	}
	char ch;
	system("cls");
	for (i = 0; i < tagMaze.t_row; i++)
	{
		for (int c = 0; c < tagMaze.t_col; c++)
		{
			printf("%c", tagMaze.t_map1[i][c]);
		}
		cout << endl;
	}

	while (x != p1 || y != q1) {
		ch = getch();
		if (ch == 's')
		{
			if (tagMaze.t_map1[x + 1][y] != '#')
			{
				tagMaze.t_map1[x][y] = ' ';
				x++;
				tagMaze.t_map1[x][y] = 'O';
			}
		}
		if (ch == 'w')
		{
			if (tagMaze.t_map1[x - 1][y] != '#')
			{
				tagMaze.t_map1[x][y] = ' ';
				x--;
				tagMaze.t_map1[x][y] = 'O';
			}
		}
		if (ch == 'a')
		{
			if (tagMaze.t_map1[x][y - 1] != '#')
			{
				tagMaze.t_map1[x][y] = ' ';
				y--;
				tagMaze.t_map1[x][y] = 'O';
			}
		}
		if (ch == 'd')
		{
			if (tagMaze.t_map1[x][y + 1] != '#')
			{
				tagMaze.t_map1[x][y] = ' ';
				y++;
				tagMaze.t_map1[x][y] = 'O';
			}
		}
		system("cls");
		for (i = 0; i < tagMaze.t_row; i++)
		{
			for (int c = 0; c < tagMaze.t_col; c++)
			{
				printf("%c", tagMaze.t_map1[i][c]);
			}
			cout << endl;
		}
	}
}
int main()
{
	vector<string> files;
	getFiles("./testdata/mazes", files);
	int p = 0;
	printf("************************************************\n");
	printf("*          Welcome to maze walking games       *\n");
	printf("*          Other. Exit the game                *\n");
	printf("*          1. The first level                  *\n");
	printf("*          2. The second level                 *\n");
	printf("*          3. The third level                  *\n");
	printf("************************************************\n");
	printf("Please select：->");
	while (true)
	{
		p = 0;
		scanf("%d", &p);
		switch (p) {
		default:
			return 0;
		case 1:runMaze(ReadMazeFromFile(files[2]));
			break;
		case 2:runMaze(ReadMazeFromFile(files[1]));
			break;
		case 3:runMaze(ReadMazeFromFile(files[0]));
			break;
		}
		system("cls");
		printf("Congratulations, the game has been completed!!!");
		Sleep(3000);
		printf("************************************************\n");
		printf("*          Welcome to maze walking games       *\n");
		printf("*          Other. Exit the game                *\n");
		printf("*          1. The first level                  *\n");
		printf("*          2. The second level                 *\n");
		printf("*          3. The third level                  *\n");
		printf("************************************************\n");
		printf("Please select：->");
	}
	return 0; 
}
