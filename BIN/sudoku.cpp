#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>
#include"sudoku_function.h"


int main(int argc, char *argv[])
{
	//�жϽ�������Ҫִ�е�����
	char order[10] = { 0 };
	strcpy(order, argv[1]);
	switch (order[1])
	{
		case 'c':
		{
			int n = transform_into_int(argv[2]);
			if (n == -1)
			{
				printf("�����˴���Ĳ���\n");
				return 1;
			}
			else
			{
				generate_sudoku(n, "endgame.txt");//����n�������վֲ���ӡ��endgame
			}
		}; break;

		case 's':
		{
			solve_problem(argv[2], "sudoku.txt");//���argv[2]�ļ��е�������Ŀ�������sudoku.txt
		}; break;

		case 'x':
		{
			generate_problem("endgame.txt", argv[2]);//��endgame�е��վ�ת��Ϊ��Ŀ�������argv[2]
		}; break;

		case 'y':
		{
			is_right(argv[2]);
		}; break;

		default:
		{
			printf("�����˴���Ĳ���\n");
			return 1;
		};break;
			
	}

	system("pause");
}