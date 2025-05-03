#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#define MAXSIZE 1000
//定义表达式结构体
typedef struct _bds
{
	string ys[8]; //存储8个字符串
	int deal; //是否已经处理标记
	_bds()
	{
		deal = 0; //0表示未处理，1表示已处理
	}
}StBds;

typedef struct _subbds
{
	string sub[4];
}StSub;

StSub g_zh[] = {
	{"(A+1)","(B+1)","(C+1)","(D+1)"}, //1
	{"(A+1)","(B+1)","(C+1)","D" }, //2
	{"(A+1)","(B+1)","C","(D+1)"}, //3
	{"(A+1)","(B+1)","C","D"}, //4
	{"(A+1)","B","(C+1)","(D+1)"}, //5
	{"(A+1)","B","(C+1)","D"}, //6
	{"(A+1)","B","C","(D+1)"}, //7
	{"(A+1)","B","C","D"}, //8
	{"A","(B+1)","(C+1)","(D+1)"},//9
	{"A","(B+1)","(C+1)","D"},//10
	{"A","(B+1)","C","(D+1)"},//11
	{"A","(B+1)","C","D"},//12
	{"A","B","(C+1)","(D+1)"},//13
	{"A","B","(C+1)","D"},//14
	{"A","B","C","(D+1)"},//15
	{"A","B","C","D"}//16
};

/*将str字符串插入表达式对应的数组中*/
void InsertStr(StBds &bds, char str[])
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

//读取文件，+前的部分作为一个表达式，每个表达式包含8个子式，表达式按照首字母插入数组
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
			else if ((buf[i] >= 'A' && buf[i] <= 'H') || buf[i] == '1' || buf[i] == '(' || buf[i] == ')' || buf[i]=='+')
			{
				if (buf[i] == '(')
					flag = 1;
				else if (buf[i] == ')')
					flag = 0;
				i++;
			}
		}
		if (i > 0 && (buf[0]!=EOF) && (buf[0] != '+')) //如果表达式最后不是以'+'结束，需要用这部分处理
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

//判断前4项是否一致
int IsCompete(StSub st, StBds bds)
{
	for (int i = 0; i < 4; i++)
	{
		if (st.sub[i].compare(bds.ys[i]) != 0)
			return 0;
	}
	return 1;
}


//处理数据
void DealData(const char* filename, StBds allbds[], int n)
{
	ofstream os(filename); //结果保存到文件
	int flag = 0;

	for (int i = 0; i < 16; i++)
	{
		flag = 0;
		for (int j = 0; j < n; j++)
		{
			if (allbds[j].deal == 0 && IsCompete(g_zh[i], allbds[j]))
			{
				allbds[j].deal = 1;
				if (flag != 0)
				{
					os << "+" << endl;
					cout << "+" << endl;
				}
				else
					flag = 1;
				
				//写文件
				for (int k = 0; k < 8; k++)
				{
					if (k < 7)
					{
						os << allbds[j].ys[k] << "*";
						cout << allbds[j].ys[k] << "*";
					}
					else
					{
						os << allbds[j].ys[k];
						cout << allbds[j].ys[k];
					}
				}
			}
		}
		if (i < 15)
		{
			os << "+" << endl << endl;//每个公因式之间加2个换行符
			cout << "+" << endl << endl;//每个公因式之间加2个换行符
		}
	}
	os.close();
}

int main()
{
	char src[200] = { 0 };// = "data.txt"; //源数据文件
	char dst[200] = { 0 };// "result.txt"; //结果输出文件
	StBds allbds[MAXSIZE]; //保存所有表达式
	int n = 0; //表达式的数量

	cout << "请输入表达式所在的文件（TXT格式）：";
	cin >> src;
	cout << "请输入结果保存文件（TXT格式）：";
	cin >> dst;

	ReadFile(src, allbds, n);
	DealData(dst, allbds, n);
	cout << endl << "处理完成，结果保存在" << dst << "文件中！" << endl;
	return 0;
}