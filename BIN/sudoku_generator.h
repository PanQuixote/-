#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include<math.h>

//����ļ��ڵ�����
void g_clear_file(char* file_name)
{
	FILE *fp;
	fp = fopen(file_name, "w");

	fclose(fp);
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
		while ((line1 == line2) || (line1 / 3 != line2 / 3))//��������line2ֱ����������
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

//����n�������վֵ��ļ�file_name�С�
void generate_sudoku(int n, char* file_name)
{
	//����ļ�����
	g_clear_file(file_name);

	FILE *fp;
	fp = fopen(file_name, "a+");//��׷�ӷ�ʽд��

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


			//����ģ������У�ת��Ϊ�վֲ����
			char sudo[10][18] = { 0 };
			for (int i = 0; i < 10; i++)
			{
				if (i != 9)
				{
					for (int j = 0; j < 9; j++)
					{
						sudo[i][j*2] = se[new_templet[i][j] - 'a'] + '0';

						sudo[i][j * 2 + 1] = ' ';

						if (j != 8)
							sudo[i][j * 2 + 1] = ' ';
						else
							sudo[i][j * 2 + 1] = '\n';
					}
				}
				else
				{
					if (sudo_sum + 1 == n)
						break;
					else
						sudo[9][0] = '\n';
				}

				fputs(sudo[i], fp);
			}




			sudo_sum++;
			if (sudo_sum == n)//�ж����ɵ��վ����Ƿ��ѴﵽҪ��
			{
				fclose(fp);
				return;
			}
		}
	}
}

//��endgame_filename�ж�ȡ�վ֣�ת��Ϊ��Ŀд��problem_filename
int generate_problem(char* endgame_filename, char* problem_filename)
{
	FILE *fp1;
	if ((fp1 = fopen(endgame_filename, "r")) == NULL)
		return -1;

	g_clear_file(problem_filename);//�����¼������Ŀ���ļ�
	FILE *fp2;
	fp2 = fopen(problem_filename, "a+");

	int sudoku_sum = 0;

	while (1)
	{	
		int sudo[9][9] = { 0 };
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
						return sudoku_sum;
					}

				} while (tem<'0' || tem>'9');

				sudo[i][j] = tem - '0';
			}
		}

		//����ڿ�
		srand((unsigned)time(NULL));
		int blank = rand() % 31 + 30;//��Ҫ�ڿյ�����
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


		char c_sudo[10][18] = { 0 };
		for (int i = 0; i < 10; i++)
		{
			if (i != 9)
			{
				for (int j = 0; j < 9; j++)
				{
					c_sudo[i][j * 2] = sudo[i][j] + '0';

					sudo[i][j * 2 + 1] = ' ';

					if (j != 8)
						sudo[i][j * 2 + 1] = ' ';
					else
						sudo[i][j * 2 + 1] = '\n';
				}
			}
			else
			{
				sudo[9][0] = '\n';
			}

			fputs(c_sudo[i], fp2);
		}

		
		sudoku_sum++;
	}

	fclose(fp1);
	fclose(fp2);
}