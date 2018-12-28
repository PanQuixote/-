#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>
#include<algorithm>

using namespace std;

//记录移动的方式。每行记录生成一个数独时需要进行的操作。
//例如，第一行 0,3,6,1,4,7,2,5,8 表示，数独第一行由数列se向右移动0位得到；数独第二行由se向右移动3位得到……
//此数组共有30行，代表通过一个数列可以获得30个数独。
int move_way[30][9] = {
	{ 0,3,6,1,4,7,2,5,8 },
	{ 0,3,6,1,7,4,2,5,8 },
	{ 0,3,6,4,1,7,2,5,8 },
	{ 0,3,6,4,7,1,2,5,8 },
	{ 0,3,6,7,1,4,2,5,8 },


	{ 0,3,6,1,4,7,2,8,5 },
	{ 0,3,6,1,4,7,5,2,8 },
	{ 0,3,6,1,4,7,5,8,2 },
	{ 0,3,6,1,4,7,8,2,5 },
	{ 0,3,6,1,4,7,8,5,2 },

	{ 0,3,6,1,7,4,2,8,5 },
	{ 0,3,6,4,1,7,5,2,8 },
	{ 0,3,6,4,7,1,5,8,2 },
	{ 0,3,6,7,4,1,8,2,5 },
	{ 0,3,6,7,1,4,8,5,2 },

	{ 0,6,3,1,4,7,2,5,8 },
	{ 0,6,3,1,7,4,2,5,8 },
	{ 0,6,3,4,1,7,2,5,8 },
	{ 0,6,3,4,7,1,2,5,8 },
	{ 0,6,3,7,1,4,2,5,8 },


	{ 0,6,3,1,4,7,2,8,5 },
	{ 0,6,3,1,4,7,5,2,8 },
	{ 0,6,3,1,4,7,5,8,2 },
	{ 0,6,3,1,4,7,8,2,5 },
	{ 0,6,3,1,4,7,8,5,2 },

	{ 0,6,3,1,7,4,2,8,5 },
	{ 0,6,3,4,1,7,5,2,8 },
	{ 0,6,3,4,7,1,5,8,2 },
	{ 0,6,3,7,4,1,8,2,5 },
	{ 0,6,3,7,1,4,8,5,2 },
};

//将数列se向右移动m位，移动后的结果存入result
void move_se(char* se, char* result, int n)
{
	for (int i = 0; i < n; i++)
		result[i] = se[9 - n + i];

	for (int i = n; i < 9; i++)
		result[i] = se[i - n];
}

//将数列se转化成符合要求的格式，存储在se_string
void se_change_into_string(char* se, char* se_string)
{
	for (int x = 0; x < 9; x++)
	{
		se_string[x * 2] = se[x];

		if (x != 8)
			se_string[x * 2 + 1] = ' ';
		else
			se_string[x * 2 + 1] = '\n';
	}
	se_string[18] = '\0';
}

//生成N个数独终局到文件file_name中。
int generate_sudoku(int N, char* file_name)
{
	FILE *fp;
	errno_t open_error = fopen_s(&fp, file_name, "w");//打开成功返回非零，失败返回0


	char se[9] = { '2','1','3','4','5','6','7','8','9' };//学号后两位为5、5，(5+5)%9+1=2，所以数列首位为2
	char copy[9] = { 0 };//数列的副本。记录数列移动后的结果。

	int sudoku_sum = 0;//已生成的数独终局数

	while (1)
	{
		next_permutation(&se[1], &se[1] + 8);//对se的第二位到第九位进行全排列变换，得到一个新数列

		for (int i = 0; i < 30; i++)//对每个数列，生成30个数独
		{
			char sudoku_string[18 * 9 + 1] = { 0 };//数独的字符串形式
			char se_string[18 + 1] = { 0 };//一行数独的字符串形式

			for (int j = 0; j < 9; j++)
			{
				move_se(se, copy, move_way[i][j]);//按照移动表来移动数列se，移动后的结果存在copy

				se_change_into_string(copy, se_string);//将copy转化为符合格式的字符串

				strcat_s(sudoku_string, se_string);//将copy拼接到数独终局字符串中
			}

			sudoku_sum++;

			if (sudoku_sum != N)
				sudoku_string[18 * 9] = '\n';
			else
			{
				sudoku_string[18 * 9 - 1] = '\0';
				sudoku_string[18 * 9] = '\0';
			}

			fputs(sudoku_string, fp);//输出数独终局字符串到文件

			if (sudoku_sum == N)//已生成足够数目的终局
			{
				fclose(fp);
				return N;
			}
		}

	}

}

//把终局sudoku转化为题目,存在problem
void change_into_problem(const char sudoku[][9], char problem[][9])
{

	for (int x = 0; x < 9; x++)
		for (int y = 0; y < 9; y++)
			problem[x][y] = sudoku[x][y];

	srand((unsigned int)(time(NULL)));
	int blank_sum = rand() % 3 + 4;//每个宫的空白数，范围[4,6]

	for (int x = 0; x <= 2; x++)
	{
		for (int y = 0; y <= 2; y++)
		{
			int is_exit[9] = { 0 };
			int pos;
			for (int t = 1; t <= blank_sum; t++)
			{
				do
				{
					pos = rand() % 9;
				} while (is_exit[pos] == 1);

				is_exit[pos] = 1;

				int row = x * 3 + pos / 3;
				int col = y * 3 + pos % 3;

				problem[row][col] = '0';
			}
		}
	}
}

//生成N个数独题到文件file_name中，如果生成失败返回0，否则返回N
int generate_problem(int N, char* file_name)
{
	FILE *fp;
	errno_t open_error = fopen_s(&fp, file_name, "w");//打开成功返回非零，失败返回0
	if (open_error)
	{
		printf("打开文件 %s 失败\n", file_name);
		return 0;
	}

	char se[9] = { '2','1','3','4','5','6','7','8','9' };//学号后两位为5、5，(5+5)%9+1=2，所以数列首位为2
	char copy[9][9] = { 0 };//数列的副本。记录数列移动后的结果。

	int problem_sum = 0;//已生成的数独终局数

	while (1)
	{
		next_permutation(&se[1], &se[1] + 8);//对se的第二位到第九位进行全排列变换，得到一个新数列

		for (int i = 0; i < 30; i++)//对每个数列，生成30个数独
		{
			char sudoku_string[18 * 9 + 1] = { 0 };//数独的字符串形式
			char se_string[18 + 1] = { 0 };//一行数独的字符串形式

			for (int j = 0; j < 9; j++)
			{
				move_se(se, copy[j], move_way[i][j]);//按照移动表来移动数列se，移动后的结果存在copy
			}

			change_into_problem(copy, copy);//将copy转化为数独题

			for (int j = 0; j < 9; j++)
			{
				se_change_into_string(copy[j], se_string);//将copy转化为符合格式的字符串

				strcat_s(sudoku_string, se_string);//将copy拼接到数独终局字符串中
			}


			problem_sum++;

			if (problem_sum != N)
				sudoku_string[18 * 9] = '\n';
			else
			{
				sudoku_string[18 * 9 - 1] = '\0';
				sudoku_string[18 * 9] = '\0';
			}

			fputs(sudoku_string, fp);//输出数独终局字符串到文件

			if (problem_sum == N)//已生成足够数目的终局
			{
				fclose(fp);
				return N;
			}
		}

	}

}
