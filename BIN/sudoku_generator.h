#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>

//清除文件内的内容
void g_clear_file(char* file_name)
{
	FILE *fp;
	fp = fopen(file_name, "w");

	fclose(fp);
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

//生成n个数独终局到文件file_name中。
void generate_sudoku(int n, char* file_name)
{
	//清空文件内容
	g_clear_file(file_name);

	FILE *fp;
	fp = fopen(file_name, "a+");//以追加方式写入

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


			//利用模板和数列，转化为终局并输出
			char sudo[10][18] = { 0 };
			for (int i = 0; i < 10; i++)
			{
				if (i != 9)
				{
					for (int j = 0; j < 9; j++)
					{
						sudo[i][j*2] = se[new_templet[i][j] - 'a'] + '0';

						sudo[i][j * 2 + 1] = ' ';

						if (j != 8)
							sudo[i][j * 2 + 1] = ' ';
						else
							sudo[i][j * 2 + 1] = '\n';
					}
				}
				else
				{
					if (sudo_sum + 1 == n)
						break;
					else
						sudo[9][0] = '\n';
				}

				fputs(sudo[i], fp);
			}




			sudo_sum++;
			if (sudo_sum == n)//判断生成的终局数是否已达到要求
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