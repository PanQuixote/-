#pragma once

#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>

//将num转化为int，如果不合法，返回-1
int transform_into_int(char* num)
{
	int n = 0;
	char n_t[10] = { 0 };
	strcpy(n_t, num);
	for (int i = 0; i < strlen(n_t); i++)//将char数组转化为数字，如果为不合法的数字则退出
	{
		if (!(n_t[i] >= '0'&&n_t[i] <= '9'))
		{
			return -1;
		}
		else
			n = n * 10 + (n_t[i] - '0');
	}

	return n;
}

//清除文件内的内容
void clear_file(char* file_name)
{
	FILE *fp;
	fp = fopen(file_name, "w");

	fclose(fp);
}

//将矩阵sudoku输出到file_name
void s_print_sudoku(char* file_name, int sudo[][9])
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

//将矩阵sudoku输出到file_name
void g_print_sudoku(char* file_name, char sudo[][18])
{
	FILE *fp;
	fp = fopen(file_name, "a+");//以追加方式写入

	for (int i = 0; i < 9; i++)
	{
		fputs(sudo[i], fp);
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
		if (is_suit(copy, pos / 9, pos % 9, n) == 1)//如果当前位置置为n合适，则递归设置下一个为0的位置
		{
			place_num(copy, pos, n, result);
		}
	}
}

//从problem_filename中获取数独并求解，输入到result_filename
int solve_problem(char* problem_filename, char* result_filename)
{
	FILE *fp;
	fp = fopen(problem_filename, "r");
	if (fp == NULL)//打开记录数独问题的文件失败
		return -1;

	clear_file(result_filename);//清空记录结果的文件

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
					if (fscanf(fp, "%c", &tem) == -1)//读到文件尾
					{
						fclose(fp);
						return sudoku_sum;
					}

				} while (tem<'0' || tem>'9');

				sudo[i][j] = tem - '0';
			}
		}

		sudoku_sum++;

		int result[9][9] = { 0 };
		place_num(sudo, -1, 0, result);

		//print_sudoku(result_filename, result);
	}
}

//一个数独终局模板
char original_templet[9][9] = {
	{ 'i','g','h','c','a','b','f','d','e' },
	{ 'c','a','b','f','d','e','i','g','h' },
	{ 'f','d','e','i','g','h','c','a','b' },
	{ 'g','h','i','a','b','c','d','e','f' },
	{ 'a','b','c','d','e','f','g','h','i' },
	{ 'd','e','f','g','h','i','a','b','c' },
	{ 'h','i','g','b','c','a','e','f','d' },
	{ 'b','c','a','e','f','d','h','i','g' },
	{ 'e','f','d','h','i','g','b','c','a' }

};

//生成一个包含数字1-9且不含重复数字的随机数列se，用于替换模板中的字母。
//其中任意字母j对应的数字为se[j-'a'+1]。
//参数x为模板中的第一行第一列的字母。（此字母对应的数字为定值）
void generate_sequence(char x, int* se)
{
	srand((unsigned)time(NULL));//以系统时间作为随机数的种子

	int exist[10] = { 0 };//记录某个数字是否已存在于数列中。如果数字y已存在，则exist[y]=1，否则exist[y]=0

	se[x - 'a'] = (5 + 5) % 9 + 1;//设定字母x对应的数字。
	exist[se[x - 'a']] = 1;//标记此数字

	for (int i = 0; i < 9; i++)//随机生成一个符合要求的数列
	{
		if (i == x - 'a')
			continue;

		se[i] = rand() % 9 + 1;
		while (exist[se[i]] == 1)
			se[i] = rand() % 9 + 1;

		exist[se[i]] = 1;
	}
}

//由旧模板old_templet经过行变换或者列变换，生成一个新的模板new_templet，同时更新旧模板为新模板。变换次数为change_time。
void generate_templet(int change_time, char old_templet[][9], char new_templet[][9])
{
	for (int i = 0; i < 9; i++)//复制旧模板到新模板
		strcpy(new_templet[i], old_templet[i]);

	while (change_time--)//变换change_time次
	{
		srand((unsigned)time(NULL));//设定随机数的种子


		int line1, line2;//将要变换的行或列的编号
		line1 = rand() % 8 + 1;
		line2 = rand() % 8 + 1;
		while ((line1 == line2) || (line1 / 3 != line2 / 3))//不断生成line2直到符合条件
		{
			line2 = rand() % 8 + 1;
		}

		int change_way = rand() % 2;//变换的方式。change_way=0则进行行变换，change_way=1则进行列变换
		if (change_way == 0)//行变换
		{
			char tem;//临时存储将交换的字母
			for (int i = 0; i < 9; i++)//交换第line1行和line2行的字母
			{
				tem = new_templet[line1][i];
				new_templet[line1][i] = new_templet[line2][i];
				new_templet[line2][i] = tem;
			}
		}
		else//列变换
		{
			char tem;//临时存储将交换的字母
			for (int i = 0; i < 9; i++)//交换第line1列和line2列的字母
			{
				tem = new_templet[i][line1];
				new_templet[i][line1] = new_templet[i][line2];
				new_templet[i][line2] = tem;
			}
		}
	}

	for (int i = 0; i < 9; i++)//复制新模板到旧模板，用于下一次变换。
		strcpy(old_templet[i], new_templet[i]);
}


//由模板和数列转化为数独终局。se为数列，templet为模板。
void transform_into_sudo(int* se, char templet[][9], char sudo[][18])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			sudo[i][j] = se[templet[i][j] - 'a'] + '0';
		}

		for (int k = 1; k < 15; k = k + 2)
			sudo[i][k] = ' ';

		sudo[i][17] = '\n';
	}
}

//生成n个数独终局到文件file_name中。
void generate_sudoku(int n, char* file_name)
{
	//清空文件内容
	clear_file(file_name);

	//设置旧模板为原始模板
	char old_templet[9][9];
	for (int i = 0; i < 9; i++)
		strcpy(old_templet[i], original_templet[i]);


	srand((unsigned)time(NULL));
	int templet_sum = rand() % 25 + 25;//接下来将要使用的模板数。每个模板最多可生成的数组终局为8!，约4*10^4个，
									   //由于最多要求生成10^6个终局,所以最多需要25个模板。

	int se_sum = n / templet_sum + 1;//每个模板需要的数列数

	int sudo_sum = 0;//已生成的数独终局数

	for (int x = 1; x <= templet_sum; x++)
	{
		char new_templet[9][9];
		generate_templet(rand() % 10 + 3, old_templet, new_templet);//生成一个新模板。

		for (int y = 1; y <= se_sum; y++)
		{
			int se[9] = { 0 };
			generate_sequence(new_templet[0][0], se);//生成一个新数列

			char sudo[9][18];
			transform_into_sudo(se, new_templet, sudo);

			//g_print_sudoku(file_name, sudo);//打印到文件

			sudo_sum++;
			if (sudo_sum == n)//判断生成的终局数是否已达到要求
				return;
		}
	}
}


//从endgame_filename中读取终局，转化为题目写入problem_filename
int generate_problem(char* endgame_filename, char* problem_filename)
{
	FILE *fp1;
	if ((fp1 = fopen(endgame_filename, "r")) == NULL)
		return -1;

	clear_file(problem_filename);//清理记录数独题目的文件

	FILE *fp2;
	fp2 = fopen(problem_filename, "a+");

	int sudoku_sum = 0;
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

		srand((unsigned)time(NULL));
		int blank = rand() % 31 + 30;
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

		s_print_sudoku(problem_filename, sudo);

		sudoku_sum++;
	}

	fclose(fp1);
	fclose(fp2);
}

//检测文件filename中的数独是否正确，并打印结果
int is_right(char* filename)
{
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL)//打开文件失败
		return -1;

	int check_result = 1;//整个文件的检查结果，完全无误则为1
	int order_number = 0;//当前检查的数独的编号
	while (1)
	{
		int now_right = 1;//当前数独的检查结果，正确为1，错误为0
		int error_cow = 0, error_col = 0;//发现的错误元素的行号和列号
		order_number++;
		int sudo[9][9] = { 0 };
		//获取81个数，如果未足81个数就已读到文件尾，则退出，返回0
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

						if (check_result == 1)
						{
							printf("检查完毕，无误\n");
							return order_number;
						}
						else
							return order_number;
					}

				} while (tem<'0' || tem>'9');

				sudo[i][j] = tem - '0';

				if (now_right == 1 && (sudo[i][j] == 0 || is_suit(sudo, i, j, sudo[i][j]) != 1))
				{
					check_result = 0;
					now_right = 0;
					error_cow = i + 1;
					error_col = j + 1;
					printf("第 %d 号数独（%d, %d）有误\n", order_number, error_cow, error_col);
				}
			}
		}
	}

}
