#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>

//判断在sudo[x][y]上放置数字num是否符合规则，是则返回1，否则返回0
int c_is_suit(int sudo[][9], int x, int y, int num)
{
	if (sudo[x][y] <= 0 || sudo[x][y] > 9)
		return 0;

	for (int k = 0; k < 9; k++)//判断当前的行或者列有没有相同的数字
	{
		if ((k != y &&sudo[x][k] == num) || (k != x && sudo[k][y] == num))
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


//检测文件filename中的数独是否正确，并打印结果
int is_right(char* filename)
{
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL)//打开文件失败
		return -1;

	int check_result = 0;//整个文件的检查结果，完全无误则为0，否则为错误的数独数
	int order_number = 0;//当前检查的数独的编号
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

						if (check_result == 0)
						{
							printf("检查完毕，无误\n");
						}
						else
						{
							printf("共有 %d 个数独错误\n", check_result);
						}

							return order_number;
					}

				} while (tem<'0' || tem>'9');

				sudo[i][j] = tem - '0';

			}
		}

		order_number++;
		int now_right = 1;//当前数独的检查结果，正确为1，错误为0
		int error_cow = 0, error_col = 0;//发现的错误元素的行号和列号
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (c_is_suit(sudo, i, j, sudo[i][j]) != 1)//发现错误
				{
					check_result++;
					now_right = 0;
					error_cow = i + 1;
					error_col = j + 1;
					printf("第 %d 号数独（%d, %d）有误\n", order_number, error_cow, error_col);
					break;
				}
			}

			if (now_right == 0)
				break;
		}


	}

}
