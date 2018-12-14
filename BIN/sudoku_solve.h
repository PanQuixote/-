#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>



//清除文件内的内容
void s_clear_file(char* file_name)
{
	FILE *fp;
	fp = fopen(file_name, "w");

	fclose(fp);
}

//将old_sudo复制到new_sudo
void copy_sudo(int old_sudo[][9], int new_sudo[][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			new_sudo[i][j] = old_sudo[i][j];
		}
	}
}

//判断在sudo[x][y]上放置数字num是否符合规则，是则返回1，否则返回0
int s_is_suit(int sudo[][9], int pos, int num)
{
	int x = pos / 9;
	int y = pos % 9;

	for (int k = 0; k < 9; k++)//判断当前的行或者列有没有相同的数字
	{
		if ((k != y && sudo[x][k] == num) || (k != x && sudo[k][y] == num))
			return 0;
	}

	//判断当前的宫是否有相同的数字
	for (int i = 3 * (x / 3); i < 3 * (x / 3) + 3; i++)
	{
		for (int j = 3 * (y / 3); j < 3 * (y / 3) + 3; j++)
		{
			if (sudo[i][j] == num && (!(i == x && j == y)))
				return 0;
		}
	}

	return 1;
}

//递归，将num放置到空位置pos（0~80）上,直到所有的空位置被填满，填好的数独存在result中
void place_num(int sudo[][9], int pos, int num, int result[][9])
{
	int copy[9][9] = { 0 };
	copy_sudo(sudo, copy);

	if (pos >= 0)//当前位置合法，将此位置置为num
		copy[pos / 9][pos % 9] = num;

	//找到下一个对应的数字为0的位置
	do
	{
		pos++;
		if (pos > 80)//当前数独已是终局
		{
			copy_sudo(copy, result);
			return;
		}
	} while (copy[pos / 9][pos % 9] != 0);

	//尝试将此位置的下一位置置为n，n的范围是1~9
	for (int n = 1; n <= 9; n++)
	{
		if (s_is_suit(copy, pos, n) == 1)//如果当前位置置为n合适，则递归设置下一个为0的位置
		{
			place_num(copy, pos, n, result);
		}
	}

}

//从problem_filename中获取数独并求解，输入到result_filename
int solve_problem(char* problem_filename, char* result_filename)
{
	FILE *fp1;
	fp1 = fopen(problem_filename, "r");
	if (fp1 == NULL)//打开记录数独问题的文件失败
		return -1;

	s_clear_file(result_filename);//清空记录结果的文件
	FILE *fp2;
	fp2 = fopen(result_filename, "a+");

	int sudoku_sum = 0;
	while (1)
	{
		int sudo[9][9] = { 0 };
		//获取81个数，如果未足81个数就已读到文件尾，则退出，返回0
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

		int result[9][9] = { 0 };
		place_num(sudo, -1, 0, result);

		char sudoku_string[18 * 9 + 1] = { 0 };//数独的字符串形式
		char se_string[18 + 1] = { 0 };//一行数独的字符串形式

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				se_string[j * 2] = result[i][j] + '0';

				se_string[j * 2 + 1] = ' ';

				if (j != 8)
					se_string[j * 2 + 1] = ' ';
				else
					se_string[j * 2 + 1] = '\n';
			}
			se_string[18] = '\0';

			strcat(sudoku_string, se_string);//将copy拼接到数独终局中
		}
		sudoku_string[18 * 9] = '\n';
		fputs(sudoku_string, fp2);

		sudoku_sum++;

	}
}
