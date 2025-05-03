#include <iostream>
#include <fstream>
using namespace std;
#define MAXSIZE 1000
typedef struct _datainfo
{
	int x; //值1
	int y; //值2
	int v; //替换值
}StDataInfo;


//插入数组
void AddData(StDataInfo data[], int& n, int x, int y, int v)
{
	int t;
	if (x > y) //保证存入数组中的x-y对，x是小的
	{
		t = x;
		x = y;
		y = t;
	}
	data[n].x = x;
	data[n].y = y;
	data[n].v = v;
	n++;
}



//读取降序文件表
void ReadFile(const char* filename, StDataInfo data[], int& n)
{
	ifstream is(filename);
	int x, y, v;
	n = 0;
	if (is.is_open())
	{
		while (!is.eof())
		{
			if (is >> x >> y >> v)
				AddData(data, n, x, y, v);
		}
		
		is.close();
	}
	else
		cout << "文件读取失败！" << endl;
}





/**判断x,y是否在数组input中，如在，从数组中删除，并返回1
* input 数组
* n 数组元素个数
* x x值，保证y>x
* y y值
*/
int IsInArray(int input[], int &n, int x,int y)
{
	int i = 0, j = 0;
	for (i = 0; i < n; i++)
	{
		if (input[i] == x) //x在input中
		{
			for (j = i + 1; j < n; j++)
			{
				if (input[j] == y)//y在input中
				{
					int k = 0, m = 0;
					//将x，y从input中删除
					for(;k<n;k++)
					{
						if (k != i && k != j)
							input[m++] = input[k];
					}
					n -= 2;
					return 1;
				}
			}
		}
	}
	return 0;
}


/**处理数据
 * data 替换组合对
 * n data数组元素个数
 * input 需要处理的数组
 * nmb input数组元素个数
 */ 
void DealData(StDataInfo data[], int n, int input[], int nmb)
{
	int out[MAXSIZE];
	int cnt = 0; //保存个数
	cout << "    >> 数字移除顺序：" << endl;
	while (nmb >= 2)
	{
		for (int i = 0; i < n; i++)
		{
			if (IsInArray(input, nmb, data[i].x, data[i].y))
			{
				cout << "        ->移除: " << data[i].x << "  " << data[i].y << endl;
				out[cnt++] = data[i].v;
				//cout << data[i].v << " ";
			}
		}
	}
	cout << "    >> 输出结果：" ;
	if (nmb == 1)
		cout << input[0] << " ";
	for (int i = 0; i < cnt; i++)
	{
		if (i < cnt - 1)
			cout << out[i] << " ";
		else
			cout << out[i];
	}
	cout << endl;
}

int main()
{
	StDataInfo data[MAXSIZE];//保存替换数值对
	int n = 0; //记录条数
	const char* filename = "data.txt";
	ReadFile(filename, data, n);

	int input[20]; //保存需要处理的数字
	int k = 0; //需要处理的数据组数
	int nmb = 0; //每组数据的个数
	cout << "请输入需要处理的数据组数：";
	cin >> k;
	for (int i = 0; i < k; i++)
	{
		cout << "请输入第" << i + 1 << "组数据，数据以空格间隔，回车结束：";
		nmb = 0;
		while (1)
		{
			cin >> input[nmb];
			nmb++;
			char ch = cin.get();
			if (ch == '\n')
				break;
		}
		//处理数据
		DealData(data, n, input, nmb);
	}



	return 0;
}