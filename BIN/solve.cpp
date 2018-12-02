#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>

//清除文件内的内容
void clear_file(char* file_name)
{
	FILE *fp;
	fp = fopen("result.txt", "w");
	fclose(fp);
}

//将数独sudo的终局输出到file_name
void print_result(char* file_name, int sudo[][9])
{
	FILE *fp;
	fp = fopen(file_name, "a+");//以追加方式写入

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 8; j++)
		{

			fprintf(fp, "%d ", sudo[i][j]);
		}

		fprintf(fp, "%d\n", sudo[i][8]);
	}

	fprintf(fp, "\n");

	fclose(fp);
}

//判断在sudo[x][y]上放置数字num是否符合规则，是则返回1，否则返回0
int is_suit(int sudo[][9], int x, int y, int num)
{
	for (int k = 0; k < 9; k++)//判断当前的行或者列有没有相同的数字
	{
		if (sudo[x][k] == num || sudo[k][y] == num)
			return 0;
	}

	//判断当前的宫是否有相同的数字
	for (int i = 3 * (x / 3); i < 3 * (x / 3) + 3; i++)
	{
		for (int j = 3 * (y / 3); j < 3 * (y / 3) + 3; j++)
		{
			if (sudo[i][j] == num)
				return 0;
		}
	}

	return 1;
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

//递归，将num放置到空位置pos（0~80）上,直到所有的空位置被填满，输出结果到file_name
void place_num(int sudo[][9], int pos, int num, char* file_name)
{
	int copy[9][9] = { 0 };
	copy_sudo(sudo, copy);
		
	int x = pos / 9;
	int y = pos % 9;

	if (pos >= 0)//当前位置合法，将此位置置为num
		copy[x][y] = num;

	//找到下一个对应的数字为0的位置
	do
	{
		pos++;
		if (pos > 80)//当前数独已是终局
		{
			print_result(file_name, sudo);
			return;
		}
	} 
	while (copy[pos / 9][pos % 9] != 0);

	//尝试将此位置置为n，n的范围是1~9
	for (int n = 1; n <= 9; n++)
	{
		if (is_suit(copy, x, y, n) == 1)//如果当前位置置为n合适，则递归设置下一个为0的位置
		{
			copy[x][y] = n;
 			place_num(copy, pos, n, file_name);
		}
	}
}

//从file_name中获取数独,并求解
int get_sudoku(char* file_name)
{
	FILE *fp;
	fp = fopen(file_name, "r");
	if (fp == NULL)
		return 0;

	int sudo[9][9] = { 0 };

	while (1)
	{
		//获取81个数，如果未足81个数就已读到文件尾，则退出
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				char tem;
				do
				{
					if (fscanf(fp, "%c", &tem) == -1)//读到文件尾
					{
						fclose(fp);
						return 0;
					}

				} while (tem<'0' || tem>'9');

				sudo[i][j] = tem - '0';
			}
		}

		place_num(sudo, -1, 0, "result.txt");
	}



	return 1;
}

int main(int argc, char *argv[])
{

	if (strcmp(argv[1], "-s") == 0)//如果是求解数独终局的命令
	{
		clear_file("sudoku.txt");
		get_sudoku(argv[2]);
	}
	
	system("pause");

}
