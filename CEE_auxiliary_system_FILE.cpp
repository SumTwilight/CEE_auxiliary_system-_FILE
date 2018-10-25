#include<cstdio>
#include<iostream>
#include<vector>
#include<cstring>
#include<sstream>
#include<algorithm>
#include<fstream>
using namespace std;
/*******************************************************************����ʵ�ֿ�ʼ*******************************************************/
/**************************************�������ȶ��еĺ�************************************************/
#define InHeap(n,i)				((-1<i)&&(i<n))			//�ж�PQ[i]�Ƿ�Ϸ�
#define Parent(i)				((i-1)>>1)				//PQ[i]�ĸ��ڵ�((floor((i-1)/2),i��������)
#define LastInternal(n)			Parent(n-1)				//���һ���ڲ��ڵ�(��ĩ�ڵ�ĸ���)
#define LChild(i)				(1+(i<<1))				//PQ[i]������
#define RChild(i)				((1+i)<<1)				//PQ[i]���Һ���
#define ParentValid(i)			(0<i)					//�ж�PQ[i]�Ƿ��и���
#define LChildValid(n,i)		InHeap(n,LChild(i))		//�ж�PQ[i]�Ƿ�������
#define RChildValid(n,i)		InHeap(n,RChild(i))		//�ж�PQ[i]�Ƿ�����������
#define Bigger(PQ,i,j)			((PQ[i]<PQ[j])?j:i)	//ȡ���ߣ����ʱǰ�����ȣ�
#define ProperParent(PQ,n,i)	( RChildValid(n,i)?	Bigger( PQ , Bigger( PQ ,i, LChild(i) ), RChild(i) ):\
													( LChildValid(n,i) ? Bigger( PQ ,i , LChild(i) ): i )\
								)						//���ӣ����ࣩ�����еĴ���		
//ifstream input("intest3.txt");
ifstream input_universities("universities3.txt");
ifstream input_students("students3.txt");
ofstream output("outtest1.txt");

//�߿��ɼ���Ϣ��
class CEE_GradeStruct
{
public:
	int Total_Score;	//�ܷ�
	int Chinese;		//����
	int Math;			//��ѧ
	int English;		//Ӣ��
	int Comprehensive;	//�ۺϳɼ�

						//Ĭ�Ϲ��캯��
	CEE_GradeStruct()
	{

	}
	//���캯��
	CEE_GradeStruct(int T, int C, int M, int E, int Co) :
		Total_Score(T), Chinese(C), Math(M), English(E), Comprehensive(Co) {}
	//��������
	~CEE_GradeStruct() {}
};

//־Ըרҵ��Ϣ��
class Volunteer_MajorStruct
{
public:
	string University_Number;		//ԺУ���
	string Major_Number[5];			//רҵ���
	int Major_Num;					//רҵ����
									//���캯��
	Volunteer_MajorStruct()
	{

	}
	//��������
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

//ѧ����Ϣ��
class StudentStruct
{
private:
	string Student_ID;							//������
	string Name;								//ѧ������
	CEE_GradeStruct Grade;						//�߿��ɼ�
	Volunteer_MajorStruct Volunteer_Major[6];	//־Ը��Ϣ
	bool transferred;							//�Ƿ���ӵ���
	int Major_Universities_Num;					//־Ը��
	string Enroll_Universities;					//��¼ȡԺУ
	string Enroll_Major;						//��¼ȡרҵ
	int Enroll_Status;							//���ᵵ¼ȡ=1�����ᵵ=2��δ¼ȡ=0

public:
	//Ĭ�Ϲ��캯��
	StudentStruct()
	{

	}
	//���캯��
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

	//��������
	~StudentStruct()
	{

	}

	//���ص�n��־ԸѧУ�е�־Ըרҵ��
	int GetMajor_Num(int n)
	{
		return Volunteer_Major[n].Major_Num;
	}

	//����־ԸѧУ��
	int GetMajor_Universities_Num()
	{
		return Major_Universities_Num;
	}

	//���ص�n��־ԸѧУ
	string GetV_Universities(int n)
	{
		return Volunteer_Major[n].University_Number;
	}

	//���ؿ�����n��־ԸѧУ�еĵ�i��־Ըרҵ
	string GetV_Major(int n, int i)
	{
		return Volunteer_Major[n].Major_Number[i];
	}

	//���ظÿ����ĸ߿��ɼ�
	int GetGrade()
	{
		return Grade.Total_Score;
	}

	//���ظÿ����Ƿ���ӵ���
	bool Gettransferred()
	{
		return transferred;
	}

	//���¸ÿ�����¼ȡ��ԺУ��רҵ
	void SetEnroll(string U, string M)
	{
		Enroll_Universities = U;
		Enroll_Major = M;
		if (Enroll_Major == "00000")	//�ᵵδ¼ȡ
			Enroll_Status = 2;
		else Enroll_Status = 1;		//¼ȡ�ɹ�
	}

	//��ӡ�����ţ����������������ܷ�
	void ToPrintf()
	{
		output << Student_ID << "\t" << Name << "\t" << Grade.Total_Score << endl;
		cout << Student_ID << "\t" << Name << "\t" << Grade.Total_Score << endl;
	}

	//��ӡ�����ţ����������������ܷ֣��Լ���¼ȡԺУ,���ҷ��ر�¼ȡԺУ
	string ToPrintf_Enroll()
	{
		output << "����������\n�����ţ�" << Student_ID << "\t����������" << Name << "\t�����ܷ֣�" << Grade.Total_Score << "��¼ȡԺУ��" << Enroll_Universities << endl;
		cout << "����������\n�����ţ�" << Student_ID << "\t����������" << Name << "\t�����ܷ֣�" << Grade.Total_Score << "��¼ȡԺУ��" << Enroll_Universities << endl;
		return Enroll_Universities;
	}

	//���� < ��
	bool operator<(const StudentStruct& s)
	{
		//�Ƚϳɼ�
		if (Grade.Total_Score < s.Grade.Total_Score)
			return true;
		else if (Grade.Total_Score > s.Grade.Total_Score)
			return false;
		else if (Grade.Total_Score == s.Grade.Total_Score)	//�ܷ���ͬ
		{
			//�Ƚ�����
			if (Grade.Chinese < s.Grade.Chinese)
				return true;
			else if (Grade.Chinese > s.Grade.Chinese)
				return false;
			else if (Grade.Chinese == s.Grade.Chinese)	//������ͬ
			{
				//�Ƚ���ѧ
				if (Grade.Math < s.Grade.Total_Score)
					return true;
				else if (Grade.Math > s.Grade.Math)
					return false;
				else if (Grade.Math == s.Grade.Math)	//��ѧ��ͬ
				{
					//�Ƚ�Ӣ��
					if (Grade.English < s.Grade.English)
						return true;
					else if (Grade.English > s.Grade.English)
						return false;
					else if (Grade.English == s.Grade.English)	//������ͬ
					{
						//�Ƚ��ۺ�
						if (Grade.Comprehensive < s.Grade.Comprehensive)
							return true;
						else if (Grade.Comprehensive > s.Grade.Comprehensive)
							return false;
						else if (Grade.Comprehensive == s.Grade.Comprehensive)//ȫ���ɼ���ͬ
							return true;
					}
				}

			}

		}

	}
};

//����רҵ��Ϣ��
class MajorStruct
{
public:
	string Major_Number;					//רҵ���
	string Major_Name;						//רҵ����
	int NumberOfPlan;						//�ƻ���������
	int Grade;								//רҵ�ɼ�
	int NumOfEnrolled;						//��¼ȡ����
	vector<StudentStruct> Stu_Enrolled;		//��¼ȡ�Ŀ���
	MajorStruct *Next;
	//���캯��1
	MajorStruct()
	{
		Major_Number = "000";
		Major_Name = "NULL";
		NumberOfPlan = 0;
		Grade = 0;
		NumOfEnrolled = 0;
		Next = nullptr;
	}

	//���캯��2��רҵ��Ϣ��ʼ��
	MajorStruct(string MNum, string Name, int NOP, int Gra)
	{
		Major_Number = MNum;
		Major_Name = Name;
		NumberOfPlan = NOP;
		Grade = Gra;
		Next = nullptr;
		NumOfEnrolled = 0;
	}

	//������רҵ
	void NewMajor(MajorStruct Major)
	{
		Major_Number = Major.Major_Number;
		Major_Name = Major.Major_Name;
		NumberOfPlan = Major.NumberOfPlan;
		Grade = Major.Grade;
		Next = Major.Next;
	}

	//¼ȡѧ��
	bool EnrollStu(StudentStruct S)
	{
		if (!Check_Enroll_Plan())
		{
			Stu_Enrolled.push_back(S);
			NumOfEnrolled++;
			return true;		//¼ȡ�ɹ�
		}
		else return false;		//¼ȡʧ��
	}

	//����Ƿ���¼��
	//¼������true��δ¼������false
	bool Check_Enroll_Plan()
	{
		if (NumOfEnrolled < NumberOfPlan)
			return false;
		return true;
	}

	//��ӡ��¼ȡ��ѧ��
	void ToprintfStudent()
	{
		output << "רҵ����" << Major_Name << endl;
		output << "������\t\t����\t�ܷ�" << endl;
		cout << "רҵ����" << Major_Name << endl;
		cout << "������\t\t����\t�ܷ�" << endl;
		for (int i = 0; i < NumOfEnrolled; ++i)
		{
			Stu_Enrolled[i].ToPrintf();
		}
		output << "����Ϊ��רҵ¼ȡ��Ա" << endl << endl;
		cout << "����Ϊ��רҵ¼ȡ��Ա" << endl << endl;
	}

	//��ӡ��רҵ��Ϣ
	void ToprintfInf()
	{
		output << "\tרҵ��ţ�" << Major_Number << "\tרҵ���ƣ�" << Major_Name << "\t����¼ȡ������" << NumberOfPlan - NumOfEnrolled << endl;
		cout << "\tרҵ��ţ�" << Major_Number << "\tרҵ���ƣ�" << Major_Name << "\t����¼ȡ������" << NumberOfPlan - NumOfEnrolled << endl;
	}
};

//��У��Ϣ��
class UniversityStruct
{
private:
	MajorStruct * Majors_header;		//רҵ��Ϣ
	int length;						//רҵ����
	string University_Number;		//ԺУ���
	string University_Name;			//ԺУ����
									//	int Lowest_Grade;				//ԺУ���¼ȡ��

public:
	//���캯����ԺУ��Ϣ��ʼ��
	UniversityStruct(string Num, string Name)
	{
		Majors_header = new MajorStruct;

		length = 0;
		University_Name = Name;
		University_Number = Num;
	}

	//��������
	~UniversityStruct()
	{

	}

	//����רҵ����
	int GetLength()
	{
		return length;
	}

	//����ԺУ���
	string GetUniversity_Number()
	{
		return University_Number;
	}

	//����רҵ
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

	//��ӡ��¼ȡ����
	void ToPrintf()
	{
		output << "ԺУ����" << University_Name << endl;
		output << "������Ҫ��ӡ��¼ȡ������רҵ��ţ���Ҫ���������¼ȡ����������1��" << endl;
		cout << "ԺУ����" << University_Name << endl;
		cout << "������Ҫ��ӡ��¼ȡ������רҵ��ţ���Ҫ���������¼ȡ����������1��" << endl;
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

	//�ڱ�ԺУ¼ȡ�ÿ���
	//nΪ�����ڱ�ԺУ��־Ըרҵ��
	//¼ȡ����1��δ¼ȡ����0���ᵵδ¼ȡ����2
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
			if (node->Grade <= Grade && !node->Check_Enroll_Plan())	//�����ﵽ��¼ȡ����
			{
				Stu.SetEnroll(University_Number, node->Major_Number);
				node->EnrollStu(Stu);
				return 1;
			}
		}
		if (Majors_header->Next->Grade <= Grade && Stu.Gettransferred())	//�ᵵδ¼ȡ
		{
			Stu.SetEnroll(University_Number, "00000");
			return 2;
		}
		else return 0;	//δ¼ȡ

	}

	//¼ȡ��������
	//����1¼ȡ�ɹ�������0¼ȡʧ��
	int EnrollTransStu(StudentStruct &Stu)
	{
		int Grade = Stu.GetGrade();
		MajorStruct *node = new MajorStruct;
		node = Majors_header->Next;
		output << "��¼ȡ�ÿ�����רҵ���£�" << endl;
		cout<< "��¼ȡ�ÿ�����רҵ���£�" << endl;
		while (node != nullptr&&node->Grade <= Grade)	//������пɵ���רҵ		
		{
			output << "***";
			cout << "***";
			node->ToprintfInf();
			node = node->Next;
		}
		output << "���������רҵ��ţ�";
		cout << "���������רҵ��ţ�";
		string Num;
		cin >> Num;
		node = Majors_header->Next;
		while (node->Major_Number != Num)
			node = node->Next;
		if (node->EnrollStu(Stu))
		{
			output << "¼ȡ�ɹ���" << endl;
			cout << "¼ȡ�ɹ���" << endl;
			return 1;
		}
		else
		{
			output << "¼ȡʧ�ܣ�" << endl;
			cout << "¼ȡʧ�ܣ�" << endl;
			return 0;
		}
	}

	//���¶��塰<����
	bool operator<(const UniversityStruct& s)
	{
		stringstream s1, s2;	//����ʹ�����ַ����������ַ����е�����ת��Ϊ����
		int Num1, Num2;
		s1 << University_Number;
		s1 >> Num1;
		s2 << s.University_Number;
		s1 >> Num2;
		return Num1 < Num2;
	}
};

//���ȶ���
class PQ_Student	//Priority_Queue_Student
{
private:
	vector<StudentStruct>PQ;
	int size;
protected:
	//����
	int percolateDown(int n, int i)
	{
		int j;	//i���䣨���������������еĸ���
		while (i != (j = ProperParent(PQ, size, i)))
		{
			swap(PQ[i], PQ[j]);
			i = j;	//���߻�λ�������������½����i
		}
		return i;//�������˵ִ��λ��
	}
	//����
	int percolateUp(int i)
	{
		while (ParentValid(i))	//ֻҪi���и��ף���δ�ִ�Ѷ�������
		{
			int j = Parent(i);	//��i�ĸ��׼���j
			if (PQ[i] < PQ[j])	break;	//һ����ǰ���Ӳ����������˾���ɡ�
			swap(PQ[i], PQ[j]);
			i = j;		//�����ӽ���λ�ã�������������һ��
		}
		return i;//�����������յִ��λ�á�
	}

public:
	//Ĭ�Ϲ��캯��
	PQ_Student() { size = 0; }

	//����һ��Ԫ��
	void insert(StudentStruct Stu)
	{
		PQ.push_back(Stu);	//���Ƚ���Ԫ�ز���ĩβ
		size++;
		percolateUp(size - 1);	//�ٽ���Ԫ��ʵʩ���˵���
	}

	//�������ȶ����еĵ�һ��Ԫ��(�����ȶ��зǿ�)
	StudentStruct getMax()
	{
		if (size != 0)
			return PQ[0];
	}

	//ɾ�����ȶ����еĵ�һ��Ԫ��(�����ȶ��зǿ�)
	StudentStruct delMax()
	{
		if (size != 0)
		{
			StudentStruct maxElem = PQ[0];
			PQ[0] = PQ[--size];
			percolateDown(size, 0);//���¶Ѷ�ʵʩ����
			PQ.pop_back();
			return maxElem;	//���ش�ǰ���ݵ����Ԫ��
		}
	}

	//������ȶ����Ƿ�Ϊ��
	//Ϊ�շ���1����Ϊ�շ���0
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

//��ʼ��Universities;
void InitUniversities(vector<UniversityStruct> &Universities)
{
	cout<<"*************����¼��ԺУ��Ϣ����***********************\n\t�ֶ�¼��������1���ļ�����������2��" << endl;
	int flag;
	cin >> flag;
	if (flag == 1) 
	{
		cout << "������¼��ԺУ��" << endl;
		int n;
		cin>> n;
		cout << "\t����������ԺУ��Ϣ" << endl;
		for (int i = 0; i < n; ++i)
		{
			string Num, Name;
			cin >> Num >> Name;
			UniversityStruct U = UniversityStruct(Num, Name);
			cout << "\t������ԺУרҵ��" << endl;
			int m;
			cin >> m;
			for (int j = 0; j < m; ++j)
			{
				string MNum, MName;
				int NOP, Gra;
				cin >> MNum >> MName >> NOP >> Gra;
				MajorStruct Major(MNum, MName, NOP, Gra);	//���ù��캯��2
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
				MajorStruct Major(MNum, MName, NOP, Gra);	//���ù��캯��2
				U.insert(Major);
			}
			Universities.push_back(U);
		}
	}
	sort(Universities.begin(), Universities.end());
	cout << "*************¼��ԺУ��Ϣ�ɹ�***************************\n\t�������˵�" << endl;
}

//��ʼ��Students
void InitStudents(PQ_Student &Students)
{
	cout << "*************����¼�뿼����Ϣ����***********************\n\t�ֶ�¼��������1���ļ�����������2��" << endl;
	int flag;
	cin >> flag;
	if (flag == 1)
	{
		cout << "\t�����뿼������" << endl;
		int n;
		cin>> n;
		cout << "\t���������뿼����Ϣ" << endl;
		for (int i = 0; i < n; ++i)
		{
			cout << "����"<<i+1<<"��������\t����\t���ӵ���\t�ܷ�\t����\t��ѧ\tӢ��\t�ۺϳɼ�\t־Ը��" << endl;
			string id, name;
			bool transferred;	//���ӵ���
			int Total_Score;	//�ܷ�
			int Chinese;		//����
			int Math;			//��ѧ
			int English;		//Ӣ��
			int Comprehensive;	//�ۺϳɼ�
			int m;				//m��־Ը
			cin>> id >> name >> transferred >> Total_Score >> Chinese >> Math >> English >> Comprehensive >> m;
			CEE_GradeStruct C(Total_Score, Chinese, Math, English, Comprehensive);	//���ù��캯������ʼ�������߿��ɼ�

			Volunteer_MajorStruct V[6];
			for (int j = 0; j < m; ++j)
			{
				string Num, MNum[5];	//ԺУ��ţ�־Ըרҵ
				int len;	//k��־Ըרҵ
				cout << "ԺУ���\t־Ըרҵ��" << endl;
				cin>> Num >> len;
				for (int q = 0; q < len; ++q)
				{
					cout << "־Ըרҵ" << q + 1 <<"��";
					cin >> MNum[q];
				}
				V[j].insert(Num, MNum, len);	//����־Ըj
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
			bool transferred;	//���ӵ���
			int Total_Score;	//�ܷ�
			int Chinese;		//����
			int Math;			//��ѧ
			int English;		//Ӣ��
			int Comprehensive;	//�ۺϳɼ�
			int m;				//m��־Ը
			input_students >> id >> name >> transferred >> Total_Score >> Chinese >> Math >> English >> Comprehensive >> m;
			CEE_GradeStruct C(Total_Score, Chinese, Math, English, Comprehensive);	//���ù��캯������ʼ�������߿��ɼ�

			Volunteer_MajorStruct V[6];
			for (int j = 0; j < m; ++j)
			{
				string Num, MNum[5];	//ԺУ��ţ�־Ըרҵ
				int len;	//k��־Ըרҵ
				input_students >> Num >> len;
				for (int q = 0; q < len; ++q)
					input_students >> MNum[q];
				V[j].insert(Num, MNum, len);	//����־Ըj
			}
			StudentStruct Stu(id, name, C, m, V, transferred);
			Students.insert(Stu);
		}
	}
	cout << "*************¼�뿼����Ϣ�ɹ�***************************\n\t�������˵�" << endl;
}

//¼ȡ����
void EnrollStu(vector<UniversityStruct> &Universities, PQ_Student &Students, PQ_Student &Enrolling_Stu, PQ_Student &Enroll_Fail_Stu)
{
	output << "*************�����Զ���¼ȡ����ģ��*********************" << endl;
	cout << "*************�����Զ���¼ȡ����ģ��*********************" << endl;
	while (!Students.empty())
	{
		int status = 0;//¼ȡ״̬
		StudentStruct Stu = Students.getMax();
		int NumOfStu_Major_Universities = Stu.GetMajor_Universities_Num();
		for (int i = 0; i < NumOfStu_Major_Universities; ++i)
		{
			int index = 0;
			while (Universities[index].GetUniversity_Number() != Stu.GetV_Universities(i))
				++index;
			status = Universities[index].EnrollStu(Stu, i);
			if (status == 1)	//�ᵵ�ɹ���¼ȡ�ɹ�
			{
				output << "������";
				cout << "������";
				Stu.ToPrintf();
				output << "¼ȡ�ɹ�" << endl;
				cout << "¼ȡ�ɹ�" << endl;
				break;
			}
			else if (status == 2)	//�ᵵ�ɹ����ȴ�����
			{
				output << "������";
				cout << "������";
				Stu.ToPrintf();
				output << "*�ᵵ�ɹ�*" << endl;
				cout << " �ᵵ�ɹ�" << endl;
				Enrolling_Stu.insert(Stu);
				break;
			}
			//δ�ᵵ��������һ־ԸԺУ¼ȡ
		}
		if (status == 0)	//�ÿ���־Ը�������ߣ�¼ȡʧ�ܡ�
		{
			output << "������";
			cout << "������";
			Stu.ToPrintf();
			output << "*¼ȡʧ�ܣ��˵�*" << endl;
			cout << "*¼ȡʧ�ܣ��˵�*" << endl;
			Enroll_Fail_Stu.insert(Stu);
		}
		Students.delMax();
	}
	output << "*************�Զ���¼ȡ��������*************************\n" << endl;
	cout<< "*************�Զ���¼ȡ��������*************************\n\t�������˵�" << endl;
}

//�ֶ�¼ȡ��������
void EnrollTransStu(vector<UniversityStruct> &Universities, PQ_Student &Enroll_Fail_Stu)
{
	output << "*************�����ֶ�¼ȡ����¼ȡ����ģ��***************" << endl;
	cout << "*************�����ֶ�¼ȡ����¼ȡ����ģ��***************" << endl;
	output << "��" << Enroll_Fail_Stu.Getsize() << "��ѧ����Ҫ������" << endl;
	cout << "��" << Enroll_Fail_Stu.Getsize() << "��ѧ����Ҫ������" << endl;
	while (!Enroll_Fail_Stu.empty())	//����ѧ��
	{
		StudentStruct Stu = Enroll_Fail_Stu.getMax();
		string University = Stu.ToPrintf_Enroll();
		int index = 0;
		while (Universities[index].GetUniversity_Number() != University)
			index++;
		if (Universities[index].EnrollTransStu(Stu))
			Enroll_Fail_Stu.delMax();

	}
	output << "*************�ֶ�¼ȡ����¼ȡ����ģ�����***************\n" << endl;
	cout << "*************�ֶ�¼ȡ����¼ȡ����ģ�����***************\n\t�������˵�" << endl;
}

//��ӡ����¼ȡ���
void Printf(vector<UniversityStruct> &Universities, PQ_Student Enroll_Fail_Stu)
{
	output << "*************�����ӡ����¼ȡ���ģ��*******************" << endl;
	cout<< "*************�����ӡ����¼ȡ���ģ��*******************" << endl;
	output << "��������Ҫ��ѯ¼ȡ�����ԺУרҵ���ţ���Ҫ��ӡ����ԺУ¼ȡ�����������1����";
	cout<< "��������Ҫ��ѯ¼ȡ�����ԺУרҵ���ţ���Ҫ��ӡ����ԺУ¼ȡ�����������1����";
	string Num;
	cin >> Num;
	output << Num << endl;
	int n = Universities.size();
	if (Num == "1")
	{
		for (int i = 0; i < n; ++i)
		{
			Universities[i].ToPrintf();
			output << "����Ϊ��ԺУ¼ȡѧ��" << endl;
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
		output << "����Ϊ����������\n������\t����\t�ܷ�" << endl;
		cout<< "����Ϊ����������\n������\t����\t�ܷ�" << endl;
		while (!Enroll_Fail_Stu.empty())
		{
			Enroll_Fail_Stu.getMax().ToPrintf();
			Enroll_Fail_Stu.delMax();
		}
	}
	else 
	{ 
		output << "�޻�������" << endl; 
		cout << "�޻�������" << endl;
	}
	output << "*************��ӡ����¼ȡ���ģ�����*******************\n" << endl;
	cout << "*************��ӡ����¼ȡ���ģ�����*******************\n\t�������˵�" << endl;
}


int main()
{
	vector<UniversityStruct> Universities;	//ԺУ��Ϣ
	PQ_Student Students;					//������Ϣ
	PQ_Student Enrolling_Stu;				//�ᵵδ¼ȡ����
	PQ_Student Enroll_Fail_Stu;				//��������
	int op;
	//output << "*************��У�߿������Զ���¼ȡ����ϵͳ*************" << endl;
	cout << "*************��У�߿������Զ���¼ȡ����ϵͳ*************" << endl;
	while (1)
	{
		//output << "*************�����빦��ѡ��***************************\n\t1��¼���У��Ϣ\n\t2��¼�뿼����Ϣ\n\t3���Զ���¼ȡ����\n\t4���ֶ�¼ȡ��������\n\t5����ӡ����¼ȡ���\n\t6���˳�ϵͳ" << endl;
		cout << "*************�����빦��ѡ��***************************\n\t1��¼���У��Ϣ\n\t2��¼�뿼����Ϣ\n\t3���Զ���¼ȡ����\n\t4���ֶ�¼ȡ��������\n\t5����ӡ����¼ȡ���\n\t6���˳�ϵͳ" << endl;
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
	//�ر��ļ�
	//input.close();
	input_universities.close();
	input_students.close();

	output.close();
	return 0;
}