#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>

//���sudo�Ƿ����Ҫ�󣬷��Ϸ���-1�����򷵻ش����λ��
int c_is_suit(int sudo[][9])
{
	for (int x = 0; x < 9; x++)
	{
		for (int y = 0; y < 9; y++)
		{
			if (sudo[x][y] <= 0 || sudo[x][y] > 9)
				return x*9+y;

			int num = sudo[x][y];

			for (int k = 0; k < 9; k++)//�жϵ�ǰ���л�������û����ͬ������
			{
				if ((k != y && sudo[x][k] == num) || (k != x && sudo[k][y] == num))
					return x*9+y;
			}

			//�жϵ�ǰ�Ĺ��Ƿ�����ͬ������
			for (int i = 3 * (x / 3); i < 3 * (x / 3) + 3; i++)
			{
				for (int j = 3 * (y / 3); j < 3 * (y / 3) + 3; j++)
				{
					if (sudo[i][j] == num && (!(i == x && j == y)))
						return x*9+y;
				}
			}
		}
	}


	return 1;
}


//����ļ�filename�е������Ƿ���ȷ������ӡ���
int is_right(char* filename)
{
	FILE *fp;
	errno_t open_error = fopen_s(&fp, filename, "r");//�򿪳ɹ����ط��㣬ʧ�ܷ���0
	if (open_error)
	{
		printf("���ļ� %s ʧ��\n", filename);
		return -1;
	}

	int check_result = 0;//�����ļ��ļ��������ȫ������Ϊ0������Ϊ�����������
	int order_number = 0;//��ǰ���������ı��
	while (1)
	{


		int sudo[9][9] = { 0 };
		//��ȡ81���������δ��81�������Ѷ����ļ�β�����˳�������0
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				char tem;
				do
				{
					if (fscanf_s(fp, "%c", &tem, 1) == -1)//�����ļ�β
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
		int now_right = c_is_suit(sudo);//��ǰ�����ļ��������ȷΪ-1������Ϊ����
		
		if (now_right != -1)
		{
			//���ֵĴ���Ԫ�ص��кź��к�
			int error_cow = now_right / 9;
			int error_col = now_right % 9;

			printf("�� %d �������� %d �е� %d ������\n");

		}


	}

}
