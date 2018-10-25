#include<cstdio>
#include<iostream>
#include<vector>
#include<cstring>
#include<sstream>
#include<algorithm>
#include<fstream>
using namespace std;
/*******************************************************************代码实现开始*******************************************************/
/**************************************用于优先队列的宏************************************************/
#define InHeap(n,i)				((-1<i)&&(i<n))			//判断PQ[i]是否合法
#define Parent(i)				((i-1)>>1)				//PQ[i]的父节点((floor((i-1)/2),i无论正负)
#define LastInternal(n)			Parent(n-1)				//最后一个内部节点(即末节点的父亲)
#define LChild(i)				(1+(i<<1))				//PQ[i]的左孩子
#define RChild(i)				((1+i)<<1)				//PQ[i]的右孩子
#define ParentValid(i)			(0<i)					//判断PQ[i]是否有父亲
#define LChildValid(n,i)		InHeap(n,LChild(i))		//判断PQ[i]是否有左孩子
#define RChildValid(n,i)		InHeap(n,RChild(i))		//判断PQ[i]是否有两个孩子
#define Bigger(PQ,i,j)			((PQ[i]<PQ[j])?j:i)	//取大者（相等时前者优先）
#define ProperParent(PQ,n,i)	( RChildValid(n,i)?	Bigger( PQ , Bigger( PQ ,i, LChild(i) ), RChild(i) ):\
													( LChildValid(n,i) ? Bigger( PQ ,i , LChild(i) ): i )\
								)						//父子（至多）三者中的大者		
//ifstream input("intest3.txt");
ifstream input_universities("universities3.txt");
ifstream input_students("students3.txt");
ofstream output("outtest1.txt");

//高考成绩信息类
class CEE_GradeStruct
{
public:
	int Total_Score;	//总分
	int Chinese;		//语文
	int Math;			//数学
	int English;		//英语
	int Comprehensive;	//综合成绩

						//默认构造函数
	CEE_GradeStruct()
	{

	}
	//构造函数
	CEE_GradeStruct(int T, int C, int M, int E, int Co) :
		Total_Score(T), Chinese(C), Math(M), English(E), Comprehensive(Co) {}
	//析构函数
	~CEE_GradeStruct() {}
};

//志愿专业信息类
class Volunteer_MajorStruct
{
public:
	string University_Number;		//院校编号
	string Major_Number[5];			//专业编号
	int Major_Num;					//专业数量
									//构造函数
	Volunteer_MajorStruct()
	{

	}
	//析构函数
	~Volunteer_MajorStruct()
	{

	}

	void insert(string Num, string MNum[], int len)
	{
		University_Number = Num;
		for (int i = 0; i < len; ++i)
			Major_Number[i] = MNum[i];
		Major_Num = len;
	}
};

//学生信息类
class StudentStruct
{
private:
	string Student_ID;							//考生号
	string Name;								//学生姓名
	CEE_GradeStruct Grade;						//高考成绩
	Volunteer_MajorStruct Volunteer_Major[6];	//志愿信息
	bool transferred;							//是否服从调剂
	int Major_Universities_Num;					//志愿数
	string Enroll_Universities;					//被录取院校
	string Enroll_Major;						//被录取专业
	int Enroll_Status;							//被提档录取=1，被提档=2，未录取=0

public:
	//默认构造函数
	StudentStruct()
	{

	}
	//构造函数
	StudentStruct(string ID, string name, CEE_GradeStruct C, int n, Volunteer_MajorStruct V[], bool trans)
	{
		Student_ID = ID;
		Name = name;
		Grade = C;
		for (int i = 0; i < n; ++i)
			Volunteer_Major[i] = V[i];
		transferred = trans;
		Major_Universities_Num = n;
		Enroll_Status = 0;
	}

	//析构函数
	~StudentStruct()
	{

	}

	//返回第n个志愿学校中的志愿专业数
	int GetMajor_Num(int n)
	{
		return Volunteer_Major[n].Major_Num;
	}

	//返回志愿学校数
	int GetMajor_Universities_Num()
	{
		return Major_Universities_Num;
	}

	//返回第n个志愿学校
	string GetV_Universities(int n)
	{
		return Volunteer_Major[n].University_Number;
	}

	//返回考生第n个志愿学校中的第i个志愿专业
	string GetV_Major(int n, int i)
	{
		return Volunteer_Major[n].Major_Number[i];
	}

	//返回该考生的高考成绩
	int GetGrade()
	{
		return Grade.Total_Score;
	}

	//返回该考生是否服从调剂
	bool Gettransferred()
	{
		return transferred;
	}

	//更新该考生被录取的院校与专业
	void SetEnroll(string U, string M)
	{
		Enroll_Universities = U;
		Enroll_Major = M;
		if (Enroll_Major == "00000")	//提档未录取
			Enroll_Status = 2;
		else Enroll_Status = 1;		//录取成功
	}

	//打印考生号，考生姓名，考生总分
	void ToPrintf()
	{
		output << Student_ID << "\t" << Name << "\t" << Grade.Total_Score << endl;
		cout << Student_ID << "\t" << Name << "\t" << Grade.Total_Score << endl;
	}

	//打印考生号，考生姓名，考生总分，以及被录取院校,并且返回被录取院校
	string ToPrintf_Enroll()
	{
		output << "待调剂考生\n考生号：" << Student_ID << "\t考生姓名：" << Name << "\t考生总分：" << Grade.Total_Score << "被录取院校：" << Enroll_Universities << endl;
		cout << "待调剂考生\n考生号：" << Student_ID << "\t考生姓名：" << Name << "\t考生总分：" << Grade.Total_Score << "被录取院校：" << Enroll_Universities << endl;
		return Enroll_Universities;
	}

	//重载 < 号
	bool operator<(const StudentStruct& s)
	{
		//比较成绩
		if (Grade.Total_Score < s.Grade.Total_Score)
			return true;
		else if (Grade.Total_Score > s.Grade.Total_Score)
			return false;
		else if (Grade.Total_Score == s.Grade.Total_Score)	//总分相同
		{
			//比较语文
			if (Grade.Chinese < s.Grade.Chinese)
				return true;
			else if (Grade.Chinese > s.Grade.Chinese)
				return false;
			else if (Grade.Chinese == s.Grade.Chinese)	//语文相同
			{
				//比较数学
				if (Grade.Math < s.Grade.Total_Score)
					return true;
				else if (Grade.Math > s.Grade.Math)
					return false;
				else if (Grade.Math == s.Grade.Math)	//数学相同
				{
					//比较英语
					if (Grade.English < s.Grade.English)
						return true;
					else if (Grade.English > s.Grade.English)
						return false;
					else if (Grade.English == s.Grade.English)	//语文相同
					{
						//比较综合
						if (Grade.Comprehensive < s.Grade.Comprehensive)
							return true;
						else if (Grade.Comprehensive > s.Grade.Comprehensive)
							return false;
						else if (Grade.Comprehensive == s.Grade.Comprehensive)//全部成绩相同
							return true;
					}
				}

			}

		}

	}
};

//主修专业信息类
class MajorStruct
{
public:
	string Major_Number;					//专业编号
	string Major_Name;						//专业名称
	int NumberOfPlan;						//计划招收人数
	int Grade;								//专业成绩
	int NumOfEnrolled;						//已录取人数
	vector<StudentStruct> Stu_Enrolled;		//已录取的考生
	MajorStruct *Next;
	//构造函数1
	MajorStruct()
	{
		Major_Number = "000";
		Major_Name = "NULL";
		NumberOfPlan = 0;
		Grade = 0;
		NumOfEnrolled = 0;
		Next = nullptr;
	}

	//构造函数2：专业信息初始化
	MajorStruct(string MNum, string Name, int NOP, int Gra)
	{
		Major_Number = MNum;
		Major_Name = Name;
		NumberOfPlan = NOP;
		Grade = Gra;
		Next = nullptr;
		NumOfEnrolled = 0;
	}

	//加入新专业
	void NewMajor(MajorStruct Major)
	{
		Major_Number = Major.Major_Number;
		Major_Name = Major.Major_Name;
		NumberOfPlan = Major.NumberOfPlan;
		Grade = Major.Grade;
		Next = Major.Next;
	}

	//录取学生
	bool EnrollStu(StudentStruct S)
	{
		if (!Check_Enroll_Plan())
		{
			Stu_Enrolled.push_back(S);
			NumOfEnrolled++;
			return true;		//录取成功
		}
		else return false;		//录取失败
	}

	//检查是否已录满
	//录满返回true，未录满返回false
	bool Check_Enroll_Plan()
	{
		if (NumOfEnrolled < NumberOfPlan)
			return false;
		return true;
	}

	//打印已录取的学生
	void ToprintfStudent()
	{
		output << "专业名：" << Major_Name << endl;
		output << "考生号\t\t姓名\t总分" << endl;
		cout << "专业名：" << Major_Name << endl;
		cout << "考生号\t\t姓名\t总分" << endl;
		for (int i = 0; i < NumOfEnrolled; ++i)
		{
			Stu_Enrolled[i].ToPrintf();
		}
		output << "以上为本专业录取人员" << endl << endl;
		cout << "以上为本专业录取人员" << endl << endl;
	}

	//打印本专业信息
	void ToprintfInf()
	{
		output << "\t专业编号：" << Major_Number << "\t专业名称：" << Major_Name << "\t还能录取人数：" << NumberOfPlan - NumOfEnrolled << endl;
		cout << "\t专业编号：" << Major_Number << "\t专业名称：" << Major_Name << "\t还能录取人数：" << NumberOfPlan - NumOfEnrolled << endl;
	}
};

//高校信息类
class UniversityStruct
{
private:
	MajorStruct * Majors_header;		//专业信息
	int length;						//专业数量
	string University_Number;		//院校编号
	string University_Name;			//院校名称
									//	int Lowest_Grade;				//院校最低录取分

public:
	//构造函数：院校信息初始化
	UniversityStruct(string Num, string Name)
	{
		Majors_header = new MajorStruct;

		length = 0;
		University_Name = Name;
		University_Number = Num;
	}

	//析构函数
	~UniversityStruct()
	{

	}

	//返回专业数量
	int GetLength()
	{
		return length;
	}

	//返回院校编号
	string GetUniversity_Number()
	{
		return University_Number;
	}

	//插入专业
	void insert(const MajorStruct &Major)
	{
		MajorStruct *newnode = new MajorStruct;
		newnode->NewMajor(Major);
		MajorStruct *node = Majors_header;
		/*	while (node->Next != nullptr)
		node = node->Next;*/
		while (1)
		{
			if (node->Next == nullptr)
			{
				node->Next = newnode;
				break;
			}
			else if (node != Majors_header &&
				node->Grade <= newnode->Grade &&
				(node->Next->Grade <= newnode->Grade || node->Next == nullptr))
			{
				node = node->Next;
			}
			else if ((node->Grade <= newnode->Grade || node == Majors_header) &&
				node->Next->Grade > newnode->Grade)
			{
				newnode->Next = node->Next;
				node->Next = newnode;
				break;
			}
			else if (node == Majors_header)
			{
				node = node->Next;
			}
		}
		length++;
	}

	//打印已录取考生
	void ToPrintf()
	{
		output << "院校名：" << University_Name << endl;
		output << "请输入要打印已录取考生的专业编号，若要输出所有已录取考生请输入1：" << endl;
		cout << "院校名：" << University_Name << endl;
		cout << "请输入要打印已录取考生的专业编号，若要输出所有已录取考生请输入1：" << endl;
		string Num;
		cin >> Num;
		output << Num << endl;
		MajorStruct *node = new MajorStruct;
		node = Majors_header->Next;
		if (Num == "1")
		{
			while (node != nullptr)
			{
				if (node->NumOfEnrolled != 0)
					node->ToprintfStudent();
				node = node->Next;
			}
			output << endl;
		}
		else
		{
			while ((node->Major_Number != Num) && node != nullptr)
			{
				node = node->Next;
			}
			node->ToprintfStudent();
		}
	}

	//在本院校录取该考生
	//n为考生在本院校的志愿专业数
	//录取返回1，未录取返回0，提档未录取返回2
	int EnrollStu(StudentStruct &Stu, int n)
	{
		int Grade = Stu.GetGrade();
		int Major_Num = Stu.GetMajor_Num(n);
		for (int i = 0; i < Major_Num; ++i)
		{
			MajorStruct *node = new MajorStruct;
			node = Majors_header;
			string V_Major = Stu.GetV_Major(n, i);
			while (node->Major_Number != V_Major)
			{
				node = node->Next;
			}
			if (node->Grade <= Grade && !node->Check_Enroll_Plan())	//分数达到，录取该生
			{
				Stu.SetEnroll(University_Number, node->Major_Number);
				node->EnrollStu(Stu);
				return 1;
			}
		}
		if (Majors_header->Next->Grade <= Grade && Stu.Gettransferred())	//提档未录取
		{
			Stu.SetEnroll(University_Number, "00000");
			return 2;
		}
		else return 0;	//未录取

	}

	//录取调剂考生
	//返回1录取成功，返回0录取失败
	int EnrollTransStu(StudentStruct &Stu)
	{
		int Grade = Stu.GetGrade();
		MajorStruct *node = new MajorStruct;
		node = Majors_header->Next;
		output << "可录取该考生的专业如下：" << endl;
		cout<< "可录取该考生的专业如下：" << endl;
		while (node != nullptr&&node->Grade <= Grade)	//输出所有可调剂专业		
		{
			output << "***";
			cout << "***";
			node->ToprintfInf();
			node = node->Next;
		}
		output << "请输入调剂专业编号：";
		cout << "请输入调剂专业编号：";
		string Num;
		cin >> Num;
		node = Majors_header->Next;
		while (node->Major_Number != Num)
			node = node->Next;
		if (node->EnrollStu(Stu))
		{
			output << "录取成功！" << endl;
			cout << "录取成功！" << endl;
			return 1;
		}
		else
		{
			output << "录取失败！" << endl;
			cout << "录取失败！" << endl;
			return 0;
		}
	}

	//重新定义“<”号
	bool operator<(const UniversityStruct& s)
	{
		stringstream s1, s2;	//这里使用了字符串流，将字符串中的数字转换为整形
		int Num1, Num2;
		s1 << University_Number;
		s1 >> Num1;
		s2 << s.University_Number;
		s1 >> Num2;
		return Num1 < Num2;
	}
};

//优先队列
class PQ_Student	//Priority_Queue_Student
{
private:
	vector<StudentStruct>PQ;
	int size;
protected:
	//下滤
	int percolateDown(int n, int i)
	{
		int j;	//i及其（至多两个）孩子中的父亲
		while (i != (j = ProperParent(PQ, size, i)))
		{
			swap(PQ[i], PQ[j]);
			i = j;	//二者换位，并继续考察下降后的i
		}
		return i;//返回下滤抵达的位置
	}
	//上滤
	int percolateUp(int i)
	{
		while (ParentValid(i))	//只要i还有父亲（还未抵达堆顶），则
		{
			int j = Parent(i);	//将i的父亲记作j
			if (PQ[i] < PQ[j])	break;	//一旦当前父子不再逆序，上滤就完成。
			swap(PQ[i], PQ[j]);
			i = j;		//否则父子交换位置，并继续考查上一层
		}
		return i;//返回上滤最终抵达的位置。
	}

public:
	//默认构造函数
	PQ_Student() { size = 0; }

	//插入一个元素
	void insert(StudentStruct Stu)
	{
		PQ.push_back(Stu);	//首先将新元素插入末尾
		size++;
		percolateUp(size - 1);	//再将该元素实施上滤调整
	}

	//返回优先队列中的第一个元素(若优先队列非空)
	StudentStruct getMax()
	{
		if (size != 0)
			return PQ[0];
	}

	//删除优先队列中的第一个元素(若优先队列非空)
	StudentStruct delMax()
	{
		if (size != 0)
		{
			StudentStruct maxElem = PQ[0];
			PQ[0] = PQ[--size];
			percolateDown(size, 0);//对新堆顶实施下滤
			PQ.pop_back();
			return maxElem;	//返回此前备份的最大元素
		}
	}

	//检查优先队列是否为空
	//为空返回1，不为空返回0
	int empty()
	{
		if (PQ.size() == 0)
			return 1;
		return 0;
	}

	int Getsize()
	{
		return size;
	}
};

//初始化Universities;
void InitUniversities(vector<UniversityStruct> &Universities)
{
	cout<<"*************进入录入院校信息功能***********************\n\t手动录入请输入1，文件读入请输入2。" << endl;
	int flag;
	cin >> flag;
	if (flag == 1) 
	{
		cout << "请输入录入院校数" << endl;
		int n;
		cin>> n;
		cout << "\t请依次输入院校信息" << endl;
		for (int i = 0; i < n; ++i)
		{
			string Num, Name;
			cin >> Num >> Name;
			UniversityStruct U = UniversityStruct(Num, Name);
			cout << "\t请输入院校专业数" << endl;
			int m;
			cin >> m;
			for (int j = 0; j < m; ++j)
			{
				string MNum, MName;
				int NOP, Gra;
				cin >> MNum >> MName >> NOP >> Gra;
				MajorStruct Major(MNum, MName, NOP, Gra);	//调用构造函数2
				U.insert(Major);
			}
			Universities.push_back(U);
		}
	}
	else if (flag == 2)
	{
		int n;
		input_universities >> n;
		for (int i = 0; i < n; ++i)
		{
			string Num, Name;
			input_universities >> Num >> Name;
			UniversityStruct U = UniversityStruct(Num, Name);
			int m;
			input_universities >> m;
			for (int j = 0; j < m; ++j)
			{
				string MNum, MName;
				int NOP, Gra;
				input_universities >> MNum >> MName >> NOP >> Gra;
				MajorStruct Major(MNum, MName, NOP, Gra);	//调用构造函数2
				U.insert(Major);
			}
			Universities.push_back(U);
		}
	}
	sort(Universities.begin(), Universities.end());
	cout << "*************录入院校信息成功***************************\n\t返回主菜单" << endl;
}

//初始化Students
void InitStudents(PQ_Student &Students)
{
	cout << "*************进入录入考生信息功能***********************\n\t手动录入请输入1，文件读入请输入2。" << endl;
	int flag;
	cin >> flag;
	if (flag == 1)
	{
		cout << "\t请输入考生数：" << endl;
		int n;
		cin>> n;
		cout << "\t请依次输入考生信息" << endl;
		for (int i = 0; i < n; ++i)
		{
			cout << "考生"<<i+1<<"：考生号\t姓名\t服从调剂\t总分\t语文\t数学\t英语\t综合成绩\t志愿数" << endl;
			string id, name;
			bool transferred;	//服从调剂
			int Total_Score;	//总分
			int Chinese;		//语文
			int Math;			//数学
			int English;		//英语
			int Comprehensive;	//综合成绩
			int m;				//m个志愿
			cin>> id >> name >> transferred >> Total_Score >> Chinese >> Math >> English >> Comprehensive >> m;
			CEE_GradeStruct C(Total_Score, Chinese, Math, English, Comprehensive);	//调用构造函数，初始化考生高考成绩

			Volunteer_MajorStruct V[6];
			for (int j = 0; j < m; ++j)
			{
				string Num, MNum[5];	//院校编号，志愿专业
				int len;	//k个志愿专业
				cout << "院校编号\t志愿专业数" << endl;
				cin>> Num >> len;
				for (int q = 0; q < len; ++q)
				{
					cout << "志愿专业" << q + 1 <<"：";
					cin >> MNum[q];
				}
				V[j].insert(Num, MNum, len);	//插入志愿j
			}
			StudentStruct Stu(id, name, C, m, V, transferred);
			Students.insert(Stu);
		}
	}
	else if (flag == 2)
	{
		int n;
		input_students >> n;
		for (int i = 0; i < n; ++i)
		{
			string id, name;
			bool transferred;	//服从调剂
			int Total_Score;	//总分
			int Chinese;		//语文
			int Math;			//数学
			int English;		//英语
			int Comprehensive;	//综合成绩
			int m;				//m个志愿
			input_students >> id >> name >> transferred >> Total_Score >> Chinese >> Math >> English >> Comprehensive >> m;
			CEE_GradeStruct C(Total_Score, Chinese, Math, English, Comprehensive);	//调用构造函数，初始化考生高考成绩

			Volunteer_MajorStruct V[6];
			for (int j = 0; j < m; ++j)
			{
				string Num, MNum[5];	//院校编号，志愿专业
				int len;	//k个志愿专业
				input_students >> Num >> len;
				for (int q = 0; q < len; ++q)
					input_students >> MNum[q];
				V[j].insert(Num, MNum, len);	//插入志愿j
			}
			StudentStruct Stu(id, name, C, m, V, transferred);
			Students.insert(Stu);
		}
	}
	cout << "*************录入考生信息成功***************************\n\t返回主菜单" << endl;
}

//录取考生
void EnrollStu(vector<UniversityStruct> &Universities, PQ_Student &Students, PQ_Student &Enrolling_Stu, PQ_Student &Enroll_Fail_Stu)
{
	output << "*************进入自动化录取考生模块*********************" << endl;
	cout << "*************进入自动化录取考生模块*********************" << endl;
	while (!Students.empty())
	{
		int status = 0;//录取状态
		StudentStruct Stu = Students.getMax();
		int NumOfStu_Major_Universities = Stu.GetMajor_Universities_Num();
		for (int i = 0; i < NumOfStu_Major_Universities; ++i)
		{
			int index = 0;
			while (Universities[index].GetUniversity_Number() != Stu.GetV_Universities(i))
				++index;
			status = Universities[index].EnrollStu(Stu, i);
			if (status == 1)	//提档成功，录取成功
			{
				output << "考生：";
				cout << "考生：";
				Stu.ToPrintf();
				output << "录取成功" << endl;
				cout << "录取成功" << endl;
				break;
			}
			else if (status == 2)	//提档成功，等待调剂
			{
				output << "考生：";
				cout << "考生：";
				Stu.ToPrintf();
				output << "*提档成功*" << endl;
				cout << " 提档成功" << endl;
				Enrolling_Stu.insert(Stu);
				break;
			}
			//未提档，继续下一志愿院校录取
		}
		if (status == 0)	//该考生志愿期望过高，录取失败。
		{
			output << "考生：";
			cout << "考生：";
			Stu.ToPrintf();
			output << "*录取失败，退档*" << endl;
			cout << "*录取失败，退档*" << endl;
			Enroll_Fail_Stu.insert(Stu);
		}
		Students.delMax();
	}
	output << "*************自动化录取考生结束*************************\n" << endl;
	cout<< "*************自动化录取考生结束*************************\n\t返回主菜单" << endl;
}

//手动录取调剂考生
void EnrollTransStu(vector<UniversityStruct> &Universities, PQ_Student &Enroll_Fail_Stu)
{
	output << "*************进入手动录取调剂录取考生模块***************" << endl;
	cout << "*************进入手动录取调剂录取考生模块***************" << endl;
	output << "有" << Enroll_Fail_Stu.Getsize() << "个学生需要被调剂" << endl;
	cout << "有" << Enroll_Fail_Stu.Getsize() << "个学生需要被调剂" << endl;
	while (!Enroll_Fail_Stu.empty())	//调剂学生
	{
		StudentStruct Stu = Enroll_Fail_Stu.getMax();
		string University = Stu.ToPrintf_Enroll();
		int index = 0;
		while (Universities[index].GetUniversity_Number() != University)
			index++;
		if (Universities[index].EnrollTransStu(Stu))
			Enroll_Fail_Stu.delMax();

	}
	output << "*************手动录取调剂录取考生模块结束***************\n" << endl;
	cout << "*************手动录取调剂录取考生模块结束***************\n\t返回主菜单" << endl;
}

//打印考生录取情况
void Printf(vector<UniversityStruct> &Universities, PQ_Student Enroll_Fail_Stu)
{
	output << "*************进入打印考生录取情况模块*******************" << endl;
	cout<< "*************进入打印考生录取情况模块*******************" << endl;
	output << "请输入需要查询录取情况的院校专业代号（若要打印所有院校录取情况，请输入1）：";
	cout<< "请输入需要查询录取情况的院校专业代号（若要打印所有院校录取情况，请输入1）：";
	string Num;
	cin >> Num;
	output << Num << endl;
	int n = Universities.size();
	if (Num == "1")
	{
		for (int i = 0; i < n; ++i)
		{
			Universities[i].ToPrintf();
			output << "以上为本院校录取学生" << endl;
		}
	}
	else
	{
		for (int i = 0; i < n; ++i)
		{
			if (Num == Universities[i].GetUniversity_Number())
				Universities[i].ToPrintf();
		}
	}
	if (!Enroll_Fail_Stu.empty())
	{
		output << "以下为滑档考生：\n考生号\t姓名\t总分" << endl;
		cout<< "以下为滑档考生：\n考生号\t姓名\t总分" << endl;
		while (!Enroll_Fail_Stu.empty())
		{
			Enroll_Fail_Stu.getMax().ToPrintf();
			Enroll_Fail_Stu.delMax();
		}
	}
	else 
	{ 
		output << "无滑档考生" << endl; 
		cout << "无滑档考生" << endl;
	}
	output << "*************打印考生录取情况模块结束*******************\n" << endl;
	cout << "*************打印考生录取情况模块结束*******************\n\t返回主菜单" << endl;
}


int main()
{
	vector<UniversityStruct> Universities;	//院校信息
	PQ_Student Students;					//考生信息
	PQ_Student Enrolling_Stu;				//提档未录取考生
	PQ_Student Enroll_Fail_Stu;				//滑档考生
	int op;
	//output << "*************高校高考招生自动化录取辅助系统*************" << endl;
	cout << "*************高校高考招生自动化录取辅助系统*************" << endl;
	while (1)
	{
		//output << "*************请输入功能选择：***************************\n\t1：录入高校信息\n\t2：录入考生信息\n\t3：自动化录取考生\n\t4：手动录取调剂考生\n\t5：打印考生录取情况\n\t6：退出系统" << endl;
		cout << "*************请输入功能选择：***************************\n\t1：录入高校信息\n\t2：录入考生信息\n\t3：自动化录取考生\n\t4：手动录取调剂考生\n\t5：打印考生录取情况\n\t6：退出系统" << endl;
		cin >> op;
		if (op == 1)
			InitUniversities(Universities);
		else if (op == 2)
			InitStudents(Students);
		else if (op == 3)
			EnrollStu(Universities, Students, Enrolling_Stu, Enroll_Fail_Stu);
		else if (op == 4)
			EnrollTransStu(Universities, Enrolling_Stu);
		else if (op == 5)
			Printf(Universities, Enroll_Fail_Stu);
		else if (op == 6)
			break;
	}
	//关闭文件
	//input.close();
	input_universities.close();
	input_students.close();

	output.close();
	return 0;
}