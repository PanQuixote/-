#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>

#include"sudoku_generator.h"
#include"sudoku_solve.h"
#include"sudoku_check.h"


int main(int argc, char *argv[])
{
	//判断接下来将要执行的命令
	char order[10] = { 0 };
	strcpy(order, argv[1]);
	switch (order[1])
	{
		case 'c':
		{
			int n = transform_into_int(argv[2]);
			if (n == -1)
			{
				printf("输入了错误的参数\n");
				return 1;
			}
			else
			{
				generate_sudoku(n, "endgame.txt");//生成n个数独终局并打印到endgame
			}
		}; break;

		case 's':
		{
			solve_problem(argv[2], "sudoku.txt");//解决argv[2]文件中的数独题目并输出到sudoku.txt
		}; break;

		case 'x':
		{
			generate_problem("endgame.txt", argv[2]);//将endgame中的终局转化为题目，输出到argv[2]
		}; break;

		case 'y':
		{
			is_right(argv[2]);
		}; break;

		default:
		{
			printf("输入了错误的参数\n");
			return 1;
		};break;
			
	}

	system("pause");
}
