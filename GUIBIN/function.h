#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>
#include<algorithm>
using namespace std;

int endgame_sudoku[9][9] = { 0 };
int problem_sudoku[9][9] = { 0 };
int now_sudoku[9][9] = { 0 };
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

//生成一个随机数列
void generator_se(int* se)
{
	srand((unsigned int)(time(NULL)));

	memset(se, 0, sizeof(se));

	int is_exit[10] = { 0 };
	is_exit[2] = 1;

	se[0] = 2;
	for (int i = 1; i < 9; i++)
	{
		int tem;
		do
			tem = rand() % 9 + 1;
		while (is_exit[tem] == 1);

		is_exit[tem] = 1;

		se[i] = tem;
	}
}

//将数列se向右移动m位，移动后的结果存入result
void move_se(int* se, int* result, int n)
{
	for (int i = 0; i < n; i++)
		result[i] = se[9 - n + i];

	for (int i = n; i < 9; i++)
		result[i] = se[i - n];
}

//生成一个数独终局
void generator_sudoku(int sudoku[][9])
{
	int se[9] = { 0 };
	int copy[9] = { 0 };

	srand((unsigned int)(time(NULL)));
	int way = rand() % 31;

	generator_se(se);
	for (int i = 0; i < 9; i++)
	{
		move_se(se, copy, move_way[way][i]);//按照移动表来移动数列se，移动后的结果存在copy

		for (int j = 0; j < 9; j++)
		{
			sudoku[i][j] = copy[j];
		}
	}
}

//把终局sudoku转化为题目,存在problem
void change_into_problem(const int sudoku[][9], int problem[][9])
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

				problem[row][col] = 0;
			}
		}
	}
}

//检查sudoku是否正确，正确返回-1，否则返回当前位置
int check_sudoku(const int sudoku[][9])
{
	for (int x = 0; x < 9; x++)
	{
		for (int y = 0; y < 9; y++)
		{
			if (sudoku[x][y] <= 0 || sudoku[x][y] > 9)
				return x * 9 + y;

			int num = sudoku[x][y];

			for (int k = 0; k < 9; k++)//判断当前的行或者列有没有相同的数字
			{
				if ((k != y && sudoku[x][k] == num) || (k != x && sudoku[k][y] == num))
					return x * 9 + y;
			}

			//判断当前的宫是否有相同的数字
			for (int i = 3 * (x / 3); i < 3 * (x / 3) + 3; i++)
			{
				for (int j = 3 * (y / 3); j < 3 * (y / 3) + 3; j++)
				{
					if (sudoku[i][j] == num && (!(i == x && j == y)))
						return x * 9 + y;
				}
			}
		}
	}


	return -1;
}

