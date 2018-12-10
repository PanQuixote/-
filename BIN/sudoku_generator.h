#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>
#include<algorithm>

using namespace std;

//修改了生成数独终局的算法。
//由于是初版，存在模块化和优化的问题。后续将继续修改。
//生成100万个终局需要2.6~2.9秒左右。


//清除文件内的内容
void g_clear_file(char* file_name)
{
	FILE *fp;
	fp = fopen(file_name, "w");

	fclose(fp);
}

int change_way[31][9] = {
	{ 0,3,6,1,4,7,2,5,8 },

	{ 0,3,6,1,7,4,2,5,8 },
	{ 0,3,6,4,1,7,2,5,8 },
	{ 0,3,6,4,7,1,2,5,8 },
	{ 0,3,6,7,4,1,2,5,8 },
	{ 0,3,6,7,1,4,2,5,8 },

	{ 0,3,6,1,4,7,2,8,5 },
	{ 0,3,6,1,4,7,5,2,8 },
	{ 0,3,6,1,4,7,5,2,2 },
	{ 0,3,6,1,4,7,8,2,5 },
	{ 0,3,6,1,4,7,8,5,2 },

	{ 0,3,6,1,7,4,2,8,5 },
	{ 0,3,6,4,1,7,5,2,8 },
	{ 0,3,6,4,7,1,5,8,2 },
	{ 0,3,6,7,4,1,8,2,5 },
	{ 0,3,6,7,1,4,8,5,2 },

	{ 0,6,3,1,7,4,2,5,8 },
	{ 0,6,3,4,1,7,2,5,8 },
	{ 0,6,3,4,7,1,2,5,8 },
	{ 0,6,3,7,4,1,2,5,8 },
	{ 0,6,3,7,1,4,2,5,8 },

	{ 0,6,3,1,4,7,2,8,5 },
	{ 0,6,3,1,4,7,5,2,8 },
	{ 0,6,3,1,4,7,5,2,2 },
	{ 0,6,3,1,4,7,8,2,5 },
	{ 0,6,3,1,4,7,8,5,2 },

	{ 0,6,3,1,7,4,2,8,5 },
	{ 0,6,3,4,1,7,5,2,8 },
	{ 0,6,3,4,7,1,5,8,2 },
	{ 0,6,3,7,4,1,8,2,5 },
	{ 0,6,3,7,1,4,8,5,2 },
};

void move_se(char* se, char* result, int n)//将数列se向右移动m位，移动后的结果存入result
{
	for (int i = 0; i < n; i++)
		result[i] = se[9 - n + i];

	for (int i = n; i < 9; i++)
		result[i] = se[i - n];
}

//生成n个数独终局到文件file_name中。
void generate_sudoku(int N, char* file_name)
{
	//清空文件内容
	g_clear_file(file_name);

	FILE *fp;
	fp = fopen(file_name, "a+");//以追加方式写入

	//学号后两位为5、5，(5+5)%9+1=2，首位为2
	char se[9] = {'2','1','3','4','5','6','7','8','9' };
	char copy[9] = { 0 };

	int sudoku_sum = 0;
	int change_sum = N / 40000 + 1;
	while (1)
	{
		for (int i = 1; i <= 40000; i++)
		{
			next_permutation(&se[1], &se[1] + 8);

			for (int j = 0; j < change_sum; j++)
			{
				char sudoku_string[18 * 9 + 1] = { 0 };
				char se_string[18 + 1] = { 0 };

				for (int k = 0; k < 9; k++)
				{
					move_se(se, copy, change_way[j][k]);
					
					for (int x = 0; x < 9; x++)
					{
						se_string[x * 2] = copy[x];

						if (x != 8)
							se_string[x * 2 + 1] = ' ';
						else
							se_string[x * 2 + 1] = '\n';
					}
					se_string[18] = '\0';

					strcat(sudoku_string, se_string);
				}

				sudoku_sum++;

				if (sudoku_sum != N)
					sudoku_string[18 * 9] = '\n';

				fputs(sudoku_string, fp);

				if (sudoku_sum == N)
				{
					fclose(fp);
					return;
				}
			}

		}

	}

}



//从endgame_filename中读取终局，转化为题目写入problem_filename
int generate_problem(char* endgame_filename, char* problem_filename)
{
	FILE *fp1;
	if ((fp1 = fopen(endgame_filename, "r")) == NULL)
		return -1;

	g_clear_file(problem_filename);//清理记录数独题目的文件
	FILE *fp2;
	fp2 = fopen(problem_filename, "a+");

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
					if (fscanf(fp1, "%c", &tem) == -1)//读到文件尾
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


		char c_sudo[10][18] = { 0 };
		for (int i = 0; i < 10; i++)
		{
			if (i != 9)
			{
				for (int j = 0; j < 9; j++)
				{
					c_sudo[i][j * 2] = sudo[i][j] + '0';

					sudo[i][j * 2 + 1] = ' ';

					if (j != 8)
						sudo[i][j * 2 + 1] = ' ';
					else
						sudo[i][j * 2 + 1] = '\n';
				}
			}
			else
			{
				sudo[9][0] = '\n';
			}

			fputs(c_sudo[i], fp2);
		}


		sudoku_sum++;
	}

	fclose(fp1);
	fclose(fp2);
}
