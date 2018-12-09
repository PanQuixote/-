#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>

//�ж���sudo[x][y]�Ϸ�������num�Ƿ���Ϲ������򷵻�1�����򷵻�0
int c_is_suit(int sudo[][9], int x, int y, int num)
{
	for (int k = 0; k < 9; k++)//�жϵ�ǰ���л�������û����ͬ������
	{
		if ((k != y &&sudo[x][k] == num) || (k != x && sudo[k][y] == num))
			return 0;
	}

	//�жϵ�ǰ�Ĺ��Ƿ�����ͬ������
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

//����ļ�filename�е������Ƿ���ȷ������ӡ���
int is_right(char* filename)
{
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL)//���ļ�ʧ��
		return -1;

	int check_result = 0;//�����ļ��ļ��������ȫ������Ϊ0������Ϊ�����������
	int order_number = 0;//��ǰ���������ı��
	while (1)
	{
		int now_right = 1;//��ǰ�����ļ��������ȷΪ1������Ϊ0
		int error_cow = 0, error_col = 0;//���ֵĴ���Ԫ�ص��кź��к�

		int sudo[9][9] = { 0 };
		//��ȡ81���������δ��81�������Ѷ����ļ�β�����˳�������0
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				char tem;
				do
				{
					if (fscanf(fp, "%c", &tem) == -1)//�����ļ�β
					{
						fclose(fp);

						if (check_result == 0)
						{
							printf("�����ϣ�����\n");
						}
						else
						{
							printf("���� %d ����������\n", check_result);
						}

							return order_number;
					}

				} while (tem<'0' || tem>'9');

				sudo[i][j] = tem - '0';

			}
		}

		order_number++;

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (now_right == 1 && (sudo[i][j] == 0 || c_is_suit(sudo, i, j, sudo[i][j]) != 1))
				{
					check_result++;
					now_right = 0;
					error_cow = i + 1;
					error_col = j + 1;
					printf("�� %d ��������%d, %d������\n", order_number, error_cow, error_col);
					break;
				}
			}

			if (now_right == 0)
				break;
		}


	}

}
