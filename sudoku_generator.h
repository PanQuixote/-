#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>
#include<algorithm>

using namespace std;

//��¼�ƶ��ķ�ʽ��ÿ�м�¼����һ������ʱ��Ҫ���еĲ�����
//���磬��һ�� 0,3,6,1,4,7,2,5,8 ��ʾ��������һ��������se�����ƶ�0λ�õ��������ڶ�����se�����ƶ�3λ�õ�����
//�����鹲��30�У�����ͨ��һ�����п��Ի��30��������
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

//������se�����ƶ�mλ���ƶ���Ľ������result
void move_se(char* se, char* result, int n)
{
	for (int i = 0; i < n; i++)
		result[i] = se[9 - n + i];

	for (int i = n; i < 9; i++)
		result[i] = se[i - n];
}

//������seת���ɷ���Ҫ��ĸ�ʽ���洢��se_string
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

//����N�������վֵ��ļ�file_name�С�
int generate_sudoku(int N, char* file_name)
{
	FILE *fp;
	errno_t open_error = fopen_s(&fp, file_name, "w");//�򿪳ɹ����ط��㣬ʧ�ܷ���0


	char se[9] = { '2','1','3','4','5','6','7','8','9' };//ѧ�ź���λΪ5��5��(5+5)%9+1=2������������λΪ2
	char copy[9] = { 0 };//���еĸ�������¼�����ƶ���Ľ����

	int sudoku_sum = 0;//�����ɵ������վ���

	while (1)
	{
		next_permutation(&se[1], &se[1] + 8);//��se�ĵڶ�λ���ھ�λ����ȫ���б任���õ�һ��������

		for (int i = 0; i < 30; i++)//��ÿ�����У�����30������
		{
			char sudoku_string[18 * 9 + 1] = { 0 };//�������ַ�����ʽ
			char se_string[18 + 1] = { 0 };//һ���������ַ�����ʽ

			for (int j = 0; j < 9; j++)
			{
				move_se(se, copy, move_way[i][j]);//�����ƶ������ƶ�����se���ƶ���Ľ������copy

				se_change_into_string(copy, se_string);//��copyת��Ϊ���ϸ�ʽ���ַ���

				strcat_s(sudoku_string, se_string);//��copyƴ�ӵ������վ��ַ�����
			}

			sudoku_sum++;

			if (sudoku_sum != N)
				sudoku_string[18 * 9] = '\n';
			else
			{
				sudoku_string[18 * 9 - 1] = '\0';
				sudoku_string[18 * 9] = '\0';
			}

			fputs(sudoku_string, fp);//��������վ��ַ������ļ�

			if (sudoku_sum == N)//�������㹻��Ŀ���վ�
			{
				fclose(fp);
				return N;
			}
		}

	}

}

//���վ�sudokuת��Ϊ��Ŀ,����problem
void change_into_problem(const char sudoku[][9], char problem[][9])
{

	for (int x = 0; x < 9; x++)
		for (int y = 0; y < 9; y++)
			problem[x][y] = sudoku[x][y];

	srand((unsigned int)(time(NULL)));
	int blank_sum = rand() % 3 + 4;//ÿ�����Ŀհ�������Χ[4,6]

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

//����N�������⵽�ļ�file_name�У��������ʧ�ܷ���0�����򷵻�N
int generate_problem(int N, char* file_name)
{
	FILE *fp;
	errno_t open_error = fopen_s(&fp, file_name, "w");//�򿪳ɹ����ط��㣬ʧ�ܷ���0
	if (open_error)
	{
		printf("���ļ� %s ʧ��\n", file_name);
		return 0;
	}

	char se[9] = { '2','1','3','4','5','6','7','8','9' };//ѧ�ź���λΪ5��5��(5+5)%9+1=2������������λΪ2
	char copy[9][9] = { 0 };//���еĸ�������¼�����ƶ���Ľ����

	int problem_sum = 0;//�����ɵ������վ���

	while (1)
	{
		next_permutation(&se[1], &se[1] + 8);//��se�ĵڶ�λ���ھ�λ����ȫ���б任���õ�һ��������

		for (int i = 0; i < 30; i++)//��ÿ�����У�����30������
		{
			char sudoku_string[18 * 9 + 1] = { 0 };//�������ַ�����ʽ
			char se_string[18 + 1] = { 0 };//һ���������ַ�����ʽ

			for (int j = 0; j < 9; j++)
			{
				move_se(se, copy[j], move_way[i][j]);//�����ƶ������ƶ�����se���ƶ���Ľ������copy
			}

			change_into_problem(copy, copy);//��copyת��Ϊ������

			for (int j = 0; j < 9; j++)
			{
				se_change_into_string(copy[j], se_string);//��copyת��Ϊ���ϸ�ʽ���ַ���

				strcat_s(sudoku_string, se_string);//��copyƴ�ӵ������վ��ַ�����
			}


			problem_sum++;

			if (problem_sum != N)
				sudoku_string[18 * 9] = '\n';
			else
			{
				sudoku_string[18 * 9 - 1] = '\0';
				sudoku_string[18 * 9] = '\0';
			}

			fputs(sudoku_string, fp);//��������վ��ַ������ļ�

			if (problem_sum == N)//�������㹻��Ŀ���վ�
			{
				fclose(fp);
				return N;
			}
		}

	}

}
