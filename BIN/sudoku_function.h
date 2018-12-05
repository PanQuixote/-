#pragma once

#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>

//��numת��Ϊint��������Ϸ�������-1
int transform_into_int(char* num)
{
	int n = 0;
	char n_t[10] = { 0 };
	strcpy(n_t, num);
	for (int i = 0; i < strlen(n_t); i++)//��char����ת��Ϊ���֣����Ϊ���Ϸ����������˳�
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

//����ļ��ڵ�����
void clear_file(char* file_name)
{
	FILE *fp;
	fp = fopen(file_name, "w");

	fclose(fp);
}

//������sudoku�����file_name
void print_sudoku(char* file_name, int sudo[][9])
{
	FILE *fp;
	fp = fopen(file_name, "a+");//��׷�ӷ�ʽд��

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 8; j++)
		{

			fprintf(fp, "%d ", sudo[i][j]);
		}

		fprintf(fp, "%d\n", sudo[i][8]);
	}

	fprintf(fp, "\n");

	fclose(fp);
}

//�ж���sudo[x][y]�Ϸ�������num�Ƿ���Ϲ������򷵻�1�����򷵻�0
int is_suit(int sudo[][9], int x, int y, int num)
{
	for (int k = 0; k < 9; k++)//�жϵ�ǰ���л�������û����ͬ������
	{
		if (sudo[x][k] == num || sudo[k][y] == num)
			return 0;
	}

	//�жϵ�ǰ�Ĺ��Ƿ�����ͬ������
	for (int i = 3 * (x / 3); i < 3 * (x / 3) + 3; i++)
	{
		for (int j = 3 * (y / 3); j < 3 * (y / 3) + 3; j++)
		{
			if (sudo[i][j] == num)
				return 0;
		}
	}

	return 1;
}

//��old_sudo���Ƶ�new_sudo
void copy_sudo(int old_sudo[][9], int new_sudo[][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			new_sudo[i][j] = old_sudo[i][j];
		}
	}
}

//�ݹ飬��num���õ���λ��pos��0~80����,ֱ�����еĿ�λ�ñ���������õ���������result��
void place_num(int sudo[][9], int pos, int num, int result[][9])
{
	int copy[9][9] = { 0 };
	copy_sudo(sudo, copy);

	if (pos >= 0)//��ǰλ�úϷ�������λ����Ϊnum
		copy[pos/9][pos%9] = num;

	//�ҵ���һ����Ӧ������Ϊ0��λ��
	do
	{
		pos++;
		if (pos > 80)//��ǰ���������վ�
		{
			copy_sudo(copy, result);
			return;
		}
	} while (copy[pos / 9][pos % 9] != 0);

	//���Խ���λ����Ϊn��n�ķ�Χ��1~9
	for (int n = 1; n <= 9; n++)
	{
		if (is_suit(copy, pos/9, pos%9, n) == 1)//�����ǰλ����Ϊn���ʣ���ݹ�������һ��Ϊ0��λ��
		{
			place_num(copy, pos, n, result);
		}
	}
}

//��problem_filename�л�ȡ��������⣬���뵽result_filename
int solve_problem(char* problem_filename, char* result_filename)
{
	FILE *fp;
	fp = fopen(problem_filename, "r");
	if (fp == NULL)//�򿪼�¼����������ļ�ʧ��
		return -1;

	clear_file(result_filename);//��ռ�¼������ļ�

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
					if (fscanf(fp, "%c", &tem) == -1)//�����ļ�β
					{
						fclose(fp);
						return 0;
					}

				} while (tem<'0' || tem>'9');

				sudo[i][j] = tem - '0';
			}
		}

		int result[9][9] = { 0 };
		place_num(sudo, -1, 0, result);

		print_sudoku(result_filename, result);
	}
}

//һ�������վ�ģ��
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

//����һ����������1-9�Ҳ����ظ����ֵ��������se�������滻ģ���е���ĸ��
//����������ĸj��Ӧ������Ϊse[j-'a'+1]��
//����xΪģ���еĵ�һ�е�һ�е���ĸ��������ĸ��Ӧ������Ϊ��ֵ��
void generate_sequence(char x, int* se)
{
	srand((unsigned)time(NULL));//��ϵͳʱ����Ϊ�����������

	int exist[10] = { 0 };//��¼ĳ�������Ƿ��Ѵ����������С��������y�Ѵ��ڣ���exist[y]=1������exist[y]=0

	se[x - 'a'] = (5 + 5) % 9 + 1;//�趨��ĸx��Ӧ�����֡�
	exist[se[x - 'a']] = 1;//��Ǵ�����

	for (int i = 0; i < 9; i++)//�������һ������Ҫ�������
	{
		if (i == x - 'a')
			continue;

		se[i] = rand() % 9 + 1;
		while (exist[se[i]] == 1)
			se[i] = rand() % 9 + 1;

		exist[se[i]] = 1;
	}
}

//�ɾ�ģ��old_templet�����б任�����б任������һ���µ�ģ��new_templet��ͬʱ���¾�ģ��Ϊ��ģ�塣�任����Ϊchange_time��
void generate_templet(int change_time, char old_templet[][9], char new_templet[][9])
{
	for (int i = 0; i < 9; i++)//���ƾ�ģ�嵽��ģ��
		strcpy(new_templet[i], old_templet[i]);

	while (change_time--)//�任change_time��
	{
		srand((unsigned)time(NULL));//�趨�����������


		int line1, line2;//��Ҫ�任���л��еı��
		line1 = rand() % 8 + 1;
		line2 = rand() % 8 + 1;
		while ((line1 == line2) || (line1/3 != line2/3))//��������line2ֱ����������
		{
			line2 = rand() % 8 + 1;
		}

		int change_way = rand() % 2;//�任�ķ�ʽ��change_way=0������б任��change_way=1������б任
		if (change_way == 0)//�б任
		{
			char tem;//��ʱ�洢����������ĸ
			for (int i = 0; i < 9; i++)//������line1�к�line2�е���ĸ
			{
				tem = new_templet[line1][i];
				new_templet[line1][i] = new_templet[line2][i];
				new_templet[line2][i] = tem;
			}
		}
		else//�б任
		{
			char tem;//��ʱ�洢����������ĸ
			for (int i = 0; i < 9; i++)//������line1�к�line2�е���ĸ
			{
				tem = new_templet[i][line1];
				new_templet[i][line1] = new_templet[i][line2];
				new_templet[i][line2] = tem;
			}
		}
	}

	for (int i = 0; i < 9; i++)//������ģ�嵽��ģ�壬������һ�α任��
		strcpy(old_templet[i], new_templet[i]);
}

//��ӡ�����վֵ��ļ�file_name�С�seΪ���У�templetΪģ�塣
void transform_into_sudo(int* se, char templet[][9], int sudo[][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			sudo[i][j] = se[templet[i][j] - 'a'];
		}
	}
}

//����n�������վֵ��ļ�file_name�С�
void generate_sudoku(int n, char* file_name)
{
	//����ļ�����
	clear_file(file_name);

	//���þ�ģ��Ϊԭʼģ��
	char old_templet[9][9];
	for (int i = 0; i < 9; i++)
		strcpy(old_templet[i], original_templet[i]);


	srand((unsigned)time(NULL));
	int templet_sum = rand() % 25 + 25;//��������Ҫʹ�õ�ģ������ÿ��ģ���������ɵ������վ�Ϊ8!��Լ4*10^4����
									   //�������Ҫ������10^6���վ�,���������Ҫ25��ģ�塣

	int se_sum = n / templet_sum + 1;//ÿ��ģ����Ҫ��������

	int sudo_sum = 0;//�����ɵ������վ���

	for (int x = 1; x <= templet_sum; x++)
	{
		char new_templet[9][9];
		generate_templet(rand() % 10 + 3, old_templet, new_templet);//����һ����ģ�塣

		for (int y = 1; y <= se_sum; y++)
		{
			int se[9] = { 0 };
			generate_sequence(new_templet[0][0], se);//����һ��������

			int sudo[9][9];
			transform_into_sudo(se, new_templet, sudo);

			print_sudoku(file_name, sudo);//��ӡ���ļ�

			sudo_sum++;
			if (sudo_sum == n)//�ж����ɵ��վ����Ƿ��ѴﵽҪ��
				return;
		}
	}
}

//��endgame_filename�ж�ȡ�վ֣�ת��Ϊ��Ŀд��problem_filename
void generate_problem(char* endgame_filename, char* problem_filename)
{
	FILE *fp1;
	if((fp1 = fopen(endgame_filename, "r")) == NULL)
		return;

	clear_file(problem_filename);//�����¼������Ŀ���ļ�

	FILE *fp2;
	fp2 = fopen(problem_filename, "a+");
	

	int sudo[9][9] = { 0 };
	while (1)
	{
		//��ȡ81���������δ��81�������Ѷ����ļ�β�����˳�
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				char tem;
				do
				{
					if (fscanf(fp1, "%c", &tem) == -1)//�����ļ�β
					{
						fclose(fp1);
						fclose(fp2);
						return;
					}

				} while (tem<'0' || tem>'9');

				sudo[i][j] = tem - '0';
			}
		}

		srand((unsigned)time(NULL));
		int blank = rand() % 31 + 30;
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

		print_sudoku(problem_filename, sudo);
	}

	fclose(fp1);
	fclose(fp2);
}

//����ļ�filename�е������Ƿ���ȷ������ӡ���
int is_right(char* filename)
{
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL)//���ļ�ʧ��
		return -1;

	int check_result = 1;//���������ȫ������Ϊ1
	int order_number = 0;//��ǰ���������ı��
	while (1)
	{
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

						if (check_result == 1)
						{
							printf("�����ϣ�����\n");
							return 1;
						}
						else
							return 0;
					}

				} while (tem<'0' || tem>'9');

				sudo[i][j] = tem - '0';
			}
		}

		order_number++;

		int grid_is_check[9] = { 0 };//��grid_is_check[x]=0���ʾ��x����δ������
		for (int i = 0; i < 9; i++)
		{
			int check_line[10] = { 0 };	
			for (int j = 0; j < 9; j++)
			{
				if (check_line[sudo[i][j]] != 0 )
				{
					check_result = 0;
					if (check_line[sudo[i][j]] != 0)
					{
						error_cow = i;
						error_col = j;
					}

					printf("�� %d ��������%d, %d������\n", order_number, error_cow+1, error_col+1);
					
					goto out;
				}
				else
				{
					check_line[sudo[i][j]] = 1;
				}

	
				if (grid_is_check[3 * (i / 3) + (j / 3)] != 0)
					continue;
				else
					grid_is_check[3 * (i / 3) + (j / 3)] = 1;

				//��鵱ǰԪ�����ڹ����Ƿ����ظ�Ԫ��
				int check_grid[10] = { 0 };
				for (int x = 3 * (i / 3); x < 3 * (i / 3) + 3; x++)
				{
					for (int y = 3 * (j / 3); y < 3 * (j / 3) + 3; y++)
					{
						if (check_grid[sudo[x][y]] != 0)
						{
							check_result = 0;
							error_cow = i;
							error_col = j;
							printf("�� %d ��������%d, %d������\n", order_number, error_cow+1, error_col+1);
							goto out;
						}
						else
							check_grid[sudo[x][y]] = 1;
					}
				}
			}
		}

		out:;




		
	}
}