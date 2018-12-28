#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>

#include"sudoku_generator.h"
#include"sudoku_solve.h"
#include"sudoku_check.h"

//��numת��Ϊint��������Ϸ�������-1
int transform_into_int(char* num)
{
	int n = 0;
	char n_t[10] = { 0 };
	strcpy_s(n_t, num);
	int len = strlen(n_t);
	for (int i = 0; i < len; i++)//��char����ת��Ϊ���֣����Ϊ���Ϸ����������˳�
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

int main(int argc, char *argv[])
{
	if (argc < 1)
		return 1;

	clock_t start, finish;
	double totaltime;

	//�жϽ�������Ҫִ�е�����
	char order[10] = { 0 };
	strcpy_s(order, argv[1]);
	switch (order[1])
	{
		case 'c':
		{
			int n = transform_into_int(argv[2]);
			if (n == -1)
			{
				printf("�����˴���Ĳ���\n");
				system("pause");

				return 1;
			}
			else
			{
				start = clock();

				int N = generate_sudoku(n, "endgame.txt");//����n�������վֲ���ӡ��endgame

				printf("������ %d �������վ�\n", N);
			}
		}; break;

		case 's':
		{
			start = clock();

			int n = solve_problem(argv[2], "sudoku.txt");//���argv[2]�ļ��е�������Ŀ�������sudoku.txt

			printf("����� %d ��������\n", n);
		}; break;

		case 'x':
		{
			int N = transform_into_int(argv[2]);
			if (N == -1)
			{
				printf("�����˴���Ĳ���\n");
				system("pause");

				return 1;
			}
			else
			{
				start = clock();

				int n = generate_problem(N, argv[3]);//����N�������Ⲣ��ӡ��argv[3]

				printf("������ %d ��������\n", n);
			}
		}; break;

		case 'y':
		{
			start = clock();

			int n = is_right(argv[2]);

			printf("����� %d �������վ�\n", n);		
		}; break;

		default:
		{
			start = clock();

			printf("�����˴���Ĳ���\n");
			system("pause");
			return 1;
		};break;
			
	}


	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;

	printf("�˳��������ʱ��Ϊ %.3lf ��\n", totaltime);

	system("pause");
}