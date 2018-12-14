#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>
#include<algorithm>

using namespace std;


//清除文件内的内容
void g_clear_file(char* file_name)
{
	FILE *fp;
	errno_t open_error = fopen_s(&fp, file_name, "w");//打开成功返回非零，失败返回0
	if (open_error)
	{
		printf("打开文件 %s 失败\n", file_name);
		return;
	}

	fclose(fp);
}

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
void generate_sudoku(int N, char* file_name)
{
	//清空文件内容
	g_clear_file(file_name);

	FILE *fp;
	errno_t open_error = fopen_s(&fp, file_name, "a+");//打开成功返回非零，失败返回0
	if (open_error)
	{
		printf("打开文件 %s 失败\n", file_name);
		return;
	}
	//fp = fopen(file_name, "a+");//以追加方式写入

	
	char se[9] = {'2','1','3','4','5','6','7','8','9' };//学号后两位为5、5，(5+5)%9+1=2，所以数列首位为2
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

			fputs(sudoku_string, fp);//输出数独终局字符串到文件

			if (sudoku_sum == N)//已生成足够数目的终局
			{
				fclose(fp);
				return;
			}
		}

	}

}

//从endgame_filename中读取终局，转化为题目写入problem_filename
int generate_problem(char* endgame_filename, char* problem_filename)
{
	FILE *fp1;
	errno_t open_error = fopen_s(&fp1, endgame_filename, "r");//打开成功返回非零，失败返回0
	if (open_error)
	{
		printf("打开文件 %s 失败\n", endgame_filename);
		return -1;
	}

	g_clear_file(problem_filename);//清理记录数独题目的文件
	FILE *fp2;
	open_error = fopen_s(&fp2, problem_filename, "a+");//打开成功返回非零，失败返回0
	if (open_error)
	{
		printf("打开文件 %s 失败\n", problem_filename);
		return -1;
	}

	int sudoku_sum = 0;

	while (1)
	{
		int sudo[9][9] = { 0 };
		//获取81个数，如果未足81个数就已读到文件尾，则退出
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				char tem;
				do
				{
					if (fscanf_s(fp1, "%c", &tem, 1) == -1)//读到文件尾
					{
						fclose(fp1);
						fclose(fp2);
						return sudoku_sum;
					}

				} while (tem<'0' || tem>'9');

				sudo[i][j] = tem - '0';
			}
		}

		//随机挖空
		srand((unsigned)time(NULL));
		int blank = rand() % 31 + 30;//将要挖空的数量
		while (blank--)
		{
			int place = rand() % 81;
			int x = place / 9;
			int y = place % 9;
			while (sudo[x][y] == 0)
			{
				place = rand() % 81;
				x = place / 9;
				y = place % 9;
			}
			sudo[x][y] = 0;
		}

		char sudoku_string[18 * 9 + 1] = { 0 };//数独的字符串形式
		char se_string[18 + 1] = { 0 };//一行数独的字符串形式

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				se_string[j * 2] = sudo[i][j] + '0';

				se_string[j * 2 + 1] = ' ';

				if (j != 8)
					se_string[j * 2 + 1] = ' ';
				else
					se_string[j * 2 + 1] = '\n';
			}
			se_string[18] = '\0';
			
			strcat_s(sudoku_string, se_string);//将copy拼接到数独终局中
		}
		sudoku_string[18 * 9] = '\n';
		fputs(sudoku_string, fp2);


		sudoku_sum++;
	}

	fclose(fp1);
	fclose(fp2);
}
