#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>

//一个数独终局模板
char original_templet[10][10] = {
	{ '0','0','0','0','0','0','0','0','0','0' },
	{ '0','i','g','h','c','a','b','f','d','e' },
	{ '0','c','a','b','f','d','e','i','g','h' },
	{ '0','f','d','e','i','g','h','c','a','b' },
	{ '0','g','h','i','a','b','c','d','e','f' },
	{ '0','a','b','c','d','e','f','g','h','i' },
	{ '0','d','e','f','g','h','i','a','b','c' },
	{ '0','h','i','g','b','c','a','e','f','d' },
	{ '0','b','c','a','e','f','d','h','i','g' },
	{ '0','e','f','d','h','i','g','b','c','a' }

};

//生成一个包含数字1-9且不含重复数字的随机数列se，用于替换模板中的字母。
//其中任意字母j对应的数字为se[j-'a'+1]。
//参数x为模板中的第一行第一列的字母。（此字母对应的数字为定值）
void generate_sequence(char x, int* se)
{
	srand((unsigned)time(NULL));//以系统时间作为随机数的种子
	
	int exist[10] = { 0 };//记录某个数字是否已存在于数列中。如果数字y已存在，则exist[y]=1，否则exist[y]=0

	se[x - 'a' + 1] = (5 + 5) % 9 + 1;//设定字母x对应的数字。
	exist[se[x - 'a' + 1]] = 1;//标记此数字

	for (int i = 1; i <= 9; i++)//随机生成一个符合要求的数列
	{
		if (i == x-'a'+1)
			continue;

		se[i] = rand() % 9 + 1;
		while(exist[se[i]] == 1)
			se[i] = rand() % 9 + 1;

		exist[se[i]] = 1;
	}
}

//由旧模板old_templet经过行变换或者列变换，生成一个新的模板new_templet，同时更新旧模板为新模板。变换次数为change_time。
void generate_templet(int change_time, char old_templet[][10], char new_templet[][10])
{
	for (int i = 1; i <= 9; i++)//复制旧模板到新模板
		strcpy(new_templet[i], old_templet[i]);

	while (change_time--)//变换change_time次
	{
		srand((unsigned)time(NULL));//设定随机数的种子


		int line1, line2;//将要变换的行或列的编号
		line1 = rand() % 8 + 2;
		line2 = rand() % 8 + 2;
		while ((line1==line2)||(((line2 - 1) / 3) != ((line1 - 1) / 3)))//不断生成line2直到符合条件
		{
			line2 = rand() % 8 + 2;
		}

		int change_way = rand() % 2;//变换的方式。change_way=0则进行行变换，change_way=1则进行列变换
		if (change_way == 0)//行变换
		{
			char tem;//临时存储将交换的字母
			for (int i = 1; i <= 9; i++)//交换第line1行和line2行的字母
			{
				tem = new_templet[line1][i];
				new_templet[line1][i] = new_templet[line2][i];
				new_templet[line2][i] = tem;
			}
		}
		else//列变换
		{
			char tem;//临时存储将交换的字母
			for (int i = 1; i <= 9; i++)//交换第line1列和line2列的字母
			{
				tem = new_templet[i][line1];
				new_templet[i][line1] = new_templet[i][line2];
				new_templet[i][line2] = tem;
			}
		}
	}

	for (int i = 0; i <= 9; i++)//复制新模板到旧模板，用于下一次变换。
		strcpy(old_templet[i], new_templet[i]);
}

//打印数独终局到文件file_name中。se为数列，templet为模板。
void print_sudoku(char* file_name, int* se ,char templet[][10])
{
	FILE *fp;
	fp = fopen(file_name, "a+");//以追加方式写入

	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 8; j++)
		{
			//printf("%d ", se[templet[i][j] - 'a' + 1]);
			fprintf(fp, "%d ", se[templet[i][j] - 'a' + 1]);
		}
		//printf("%d\n", se[templet[i][9] - 'a' + 1]);
		fprintf(fp, "%d\n", se[templet[i][9] - 'a' + 1]);
	}
	//printf("\n");
	fprintf(fp, "\n");

	fclose(fp);
}

//生成n个数独终局到文件file_name中。
void generate_sudoku(int n, char* file_name)
{
	//清空文件内容
	FILE *fp;
	fp = fopen(file_name, "w");
	fclose(fp);

	//设置旧模板为原始模板
	char old_templet[10][10];
	for (int i = 0; i <= 9; i++)
		strcpy(old_templet[i], original_templet[i]);


	srand((unsigned)time(NULL));
	int templet_sum = rand() % 25 + 25;//接下来将要使用的模板数。每个模板最多可生成的数组终局为8!，约4*10^4个，
									   //由于最多要求生成10^6个终局,所以最多需要25个模板。

	int se_sum = n / templet_sum + 1;//每个模板需要的数列数

	int sudo_sum = 0;//已生成的数独终局数

	for (int x = 1; x <= templet_sum; x++)
	{
		char new_templet[10][10];
		generate_templet(rand() % 10 + 3, old_templet, new_templet);//生成一个新模板。

		for (int y = 1; y <= se_sum; y++)
		{
			int se[10] = { 0 };
			generate_sequence(new_templet[1][1], se);//生成一个新数列

			print_sudoku(file_name, se, new_templet);//打印到文件

			sudo_sum++;
			if (sudo_sum == n)//判断生成的终局数是否已达到要求
				return;
		}
	}
}

int main(int argc, char *argv[])
{
	//判断接下来将要执行的命令
	if (strcmp(argv[1], "-c") == 0)//如果是生成数独终局的命令
	{
		char n_t[10] = { 0 };
		strcpy(n_t, argv[2]);
		int n = 0;//生成的终局数
		for (int i = 0; i < strlen(n_t); i++)//将char数组转化为数字，如果为不合法的数字则退出
		{
			if (!(n_t[i] >= '0'&&n_t[i] <= '9'))
			{
				printf("输入了错误的参数");
				return 1;
			}
			n = n * 10 + (n_t[i]-'0');
		}

		generate_sudoku(n , "sudoku.txt");//生成n个数独终局并打印到sudoku.txt
	}

	


	system("pause");
}
