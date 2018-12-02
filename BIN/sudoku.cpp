
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>

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

void generate_sequence(char x, int* se)
{
	srand((unsigned)time(NULL));
	
	int tem[10] = { 0 };
	se[x - 'a' + 1] = (5 + 5) % 9 + 1;
	tem[se[x - 'a' + 1]] = 1;

	for (int i = 1; i <= 9; i++)
	{
		if (i == x-'a'+1)
			continue;

		se[i] = rand() % 9 + 1;
		while(tem[se[i]] == 1)
			se[i] = rand() % 9 + 1;

		tem[se[i]] = 1;
	}
}

void generate_templet(int change_time, char old_templet[][10], char new_templet[][10])
{
	for (int i = 0; i <= 9; i++)
		strcpy(new_templet[i], old_templet[i]);

	while (change_time--)
	{
		srand((unsigned)time(NULL));


		int line1, line2;
		line1 = rand() % 8 + 2;
		line2 = rand() % 8 + 2;
		while ((line1==line2)||(((line2 - 1) / 3) != ((line1 - 1) / 3)))
		{
			line2 = rand() % 8 + 2;
		}

		int change_way = rand() % 2;
		if (change_way == 0)//行变换
		{
			char tem[10];
			for (int i = 1; i <= 9; i++)
			{
				tem[i] = new_templet[line1][i];
				new_templet[line1][i] = new_templet[line2][i];
				new_templet[line2][i] = tem[i];
			}
		}
		else//列变换
		{
			char tem[10];
			for (int i = 1; i <= 9; i++)
			{
				tem[i] = new_templet[i][line1];
				new_templet[i][line1] = new_templet[i][line2];
				new_templet[i][line2] = tem[i];
			}
		}
	}

	for (int i = 0; i <= 9; i++)
		strcpy(old_templet[i], new_templet[i]);
}

void print_sudoku(char* file_name, int* se ,char templet[][10])
{
	FILE *fp;
	fp = fopen(file_name, "a+");

	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 8; j++)
		{
			printf("%d ", se[templet[i][j] - 'a' + 1]);
			fprintf(fp, "%d ", se[templet[i][j] - 'a' + 1]);
		}
		printf("%d\n", se[templet[i][9] - 'a' + 1]);
		fprintf(fp, "%d\n", se[templet[i][9] - 'a' + 1]);
	}
	printf("\n");
	fprintf(fp, "\n");

	fclose(fp);
}

void generate_sudoku(int n, char* file_name)
{
	//清空文件内容
	FILE *fp;
	fp = fopen(file_name, "w");
	fclose(fp);

	char old_templet[10][10];
	for (int i = 0; i <= 9; i++)
		strcpy(old_templet[i], original_templet[i]);

	srand((unsigned)time(NULL));
	int templet_sum = rand() % 25 + 25;
	int se_sum = n / templet_sum + 1;
	int sudo_sum = 0;

	for (int x = 1; x <= templet_sum; x++)
	{
		char new_templet[10][10];
		generate_templet(rand() % 10 + 3, old_templet, new_templet);

		for (int y = 1; y <= se_sum; y++)
		{
			int se[10] = { 0 };
			generate_sequence(new_templet[1][1], se);

			print_sudoku(file_name, se, new_templet);
			sudo_sum++;

			if (sudo_sum == n)
				return;
		}
	}
}

int main()
{
	generate_sudoku(100, "sudoku.txt");


	system("pause");
}
