#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#define MAXSIZE 1000
//定义表达式结构体
typedef struct _bds
{
	string ys[8]; //存储8个字符串
}StBds;
//定义情况结构体
typedef struct _datainfo
{
	string gs[4]; //提取公因式后的表达式
}StDataInfo;

//16种可能的情况
StDataInfo g_qk[] = {
	{"E","F","G","H"}, //1
	{"E","F","G","(H+1)"}, //2
	{"E","F","(G+1)","H"}, //3
	{"E","(F+1)","G","H"}, //4
	{"(E+1)","F","G","H"}, //5
	{"E","F","(G+1)","(H+1)"}, //6
	{"E","(F+1)","G","(H+1)"}, //7
	{"(E+1)","F","G","(H+1)"}, //8
	{"E","(F+1)","(G+1)","H"}, //9
	{"(E+1)","F","(G+1)","H"}, //10
	{"(E+1)","(F+1)","G","H"}, //11
	{"(E+1)","(F+1)","(G+1)","H"}, //12
	{"(E+1)","(F+1)","G","(H+1)"}, //13
	{"(E+1)","F","(G+1)","(H+1)"}, //14
	{"E","(F+1)","(G+1)","(H+1)"}, //15
	{"(E+1)","(F+1)","(G+1)","(H+1)"} //16
};




/*将str字符串插入表达式对应的数组中*/
void InsertStr(StBds& bds, char str[])
{
	if (str[0] == '(')
	{
		int t = str[1] - 'A';
		bds.ys[t] = str;
	}
	else
	{
		int t = str[0] - 'A';
		bds.ys[t] = str;
	}
}

//读取文件（文件为提取公因式后的文件）
void ReadFile(const char* fileName, StBds allbds[], int& n)
{
	ifstream is(fileName);
	char buf[10] = { 0 };
	int i = 0;
	int flag = 0;
	int ret = 0;
	n = 0;
	if (is.is_open())
	{
		while (!is.eof())
		{
			buf[i] = is.get();
			if (buf[i] == EOF)
				break;

			if (buf[i] == '*') //表达式的一个子式
			{
				buf[i] = '\0';
				InsertStr(allbds[n], buf);
				i = 0;
			}
			else if (flag == 0 && buf[i] == '+')//+前的部分结束
			{
				buf[i] = '\0';
				InsertStr(allbds[n], buf);
				i = 0;
				n++;
			}
			else if ((buf[i] >= 'A' && buf[i] <= 'H') || buf[i] == '1' || buf[i] == '(' || buf[i] == ')' || buf[i] == '+')
			{
				if (buf[i] == '(')
					flag = 1;
				else if (buf[i] == ')')
					flag = 0;
				i++;
			}
		}
		if (i > 0 && (buf[0] != EOF) && (buf[0] != '+')) //如果表达式最后不是以'+'结束，需要用这部分处理
		{
			buf[i] = '\0';
			InsertStr(allbds[n], buf);
			n++;
		}
		is.close();
	}
	else
		cout << "文件打开失败！" << endl;
}

//显示后4位
void show(StDataInfo qk,ofstream &os)
{
	for (int i = 0; i < 4; i++)
	{
		if (i < 3)
		{
			cout << qk.gs[i] << "*";
			os << qk.gs[i] << "*";
		}
		else
		{
			cout << qk.gs[i];
			os << qk.gs[i];
		}
	}
}


//判断两个表达式的公因式部分是否一致
int IsCompete(StBds s1, StBds s2)
{
	for (int i = 0; i < 4; i++)
	{
		if (s1.ys[i].compare(s2.ys[i]) != 0)
			return 0;
	}
	return 1;
}

//判断表达式的后4个子式是否是需要的
int IsNeeded(StBds s, StDataInfo info)
{
	for (int i = 0; i < 4; i++)
	{
		if (s.ys[i + 4].compare(info.gs[i]) != 0)
			return 0;
	}
	return 1;
}

//处理数据
void DealData(StBds allbds[], int n,const char* dst)
{
	ofstream os(dst); //结果保存到文件
	int cnt = 0;
	for (int i = 0; i < 16; i++)
	{
		for (int j = i + 1; j < 16; j++)
		{
			cnt = 0; //次数置0
			int start = 0;
			int end = 0;
			while (start < n)
			{
				//找出公因式相同的表达式
				for (end = start + 1; end < n; end++)
				{
					if (IsCompete(allbds[start], allbds[end]))
						continue;
					else
						break;
				}
				//找到start到end区间，g_qk[i]与g_qk[j]搭配的组合出现的次数
				int tmp = 0;
				for (int k = start; k < end; k++)
				{
					if (IsNeeded(allbds[k], g_qk[i]) || IsNeeded(allbds[k], g_qk[j]))
						tmp++;
				}
				if (tmp == 2) //两种情况都出现了
				{
					cnt++; //次数+1
				}
				start = end; //继续判断下一组
			}
			
			//输出--只输出出现过的组合，如果未出现过的组合也要输出的话，就把if(cnt>0)这一句注释掉
			if (cnt > 0)
			{
				show(g_qk[i],os);
				cout << "与";
				os << "与";
				show(g_qk[j],os);
				cout << "组合，出现的次数：" << cnt << endl;
				os << "组合，出现的次数：" << cnt << endl;
			}
		}
	}
	os.close();
	cout << "结果已保存到" << dst << "文件！" << endl;
}

int main()
{
	char src[200] = { 0 };// "result.txt"; //提取公因式后的表达式文件
	char dst[200] = { 0 };// "tongji.txt"; //统计结果保存文件
	StBds allbds[MAXSIZE];
	int n = 0;

	cout << "请输入提取公因式后的表达式所在文件：";
	cin >> src;
	cout << "请输入统计结果保存文件：";
	cin >> dst;

	ReadFile(src, allbds, n);
	DealData(allbds, n,dst);
	return 0;
}