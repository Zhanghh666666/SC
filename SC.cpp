//执行txt中任意行的指令
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace std;


bool stop = true; //计算是否要结束while主循环
int lines;

//读取txt文件行内容
int CountLines(string filename) {
	ifstream ReadFile;
	int n = 0;
	string tmp;
	ReadFile.open(filename.c_str());//ios::in 表示以只读的方式读取文件
	if (ReadFile.fail())            //文件打开失败:返回0
	{
		stop = false;
		return 0;
	}
	else//文件存在
	{
		while (getline(ReadFile, tmp, '\n'))
		{
			n++;
		}
		ReadFile.close();
		return n;
	}
}

//读取txt文件中 pc 行的内容
string ReadLine(string filename, int line){
	int i = 0;
	string temp;
	fstream file;
	file.open(filename.c_str());
	lines = CountLines(filename);

	if (line <= 0)
	{
		stop = false;
		return "Error 1: 行数错误，不能为0或负数。";
	}
	if (file.fail())
	{
		stop = false;
		return "Error 2: 文件不存在。";
	}
	if (line > lines)
	{
		stop = false;
		return "Error 3: 行数超出文件长度。";
	}
	while (getline(file, temp) && i < line - 1)
	{
		i++;                                            //也许可以优化，需要遍历txt所有指令
	}
	file.close();
	return temp;

}

//Instruction Decode  译码阶段
//获得 op
int ID_op(string Ins) {
	string op = Ins.substr (0, 6);   // 前 6 bit 的 op
	//cout << op << endl;
	int num = stoi(op, 0, 2);
	return num;
}
//获得指令中指定部分的结果，返回 int 
int ID_get(string Ins, int start, int size) {
	string r = Ins.substr(start, size);
	int num = stoi(r, 0, 2);
	return num;
}
获取指令中的指定部分，返回 string
string ID_getString(string Ins, int start, int size) {
	string r = Ins.substr(start, size);
	return r;
}

//EX
int ADD(int r1, int r2) {
	return r1 + r2;
}
int SUB(int r1, int r2) {
	return r1 - r2;
}


void main()
{
	string Ins;
	string filename = "E:\\VsCode\\SC\\SC\\I.txt";  //读取的文件名字
	
	int pc = 1;
	int R[32];   //模拟32个寄存器，需要根据标准mips库更改成4个int，4个floct等等功能寄存器

	R[1] = R[2] = 10;   //假设初始化值


	while (stop) {
	//IF,取指，指定pc行的指令
		
		Ins= ReadLine(filename, pc);

		//计算pc,要读取的行数，可以放在流水线其他位置，暂定执行递增PC位置指令
		pc = pc + 1;
		if (pc == lines + 1)
			stop = false;              //txt中的指令执行完毕，结束循环



	//译码，解析op，同时取寄存器中值,需要分出来三种指令,r，i和j，根据某一位的不同分别译码
		string type = Ins.substr(0, 2);
		int op = ID_op(Ins);
				
		//三个寄存器
		int r_1;
		int r_2;
	    int r_3; 


		//两个操作数和中间结果
		int num_1 ;
		int num_2 ;  
		int num_result;



		if( type == "00")     //解析 R 型指令
		{
			r_1 = ID_get(Ins,6,5);
			r_2 = ID_get(Ins,11,5);
			r_3 = ID_get(Ins,16,5);

			num_1 = R[r_1];
			num_2 = R[r_2];

			cout << "op=" << op << endl;
			cout << "r1=" << r_1 << endl;
			cout << "r2=" << r_2 << endl;
			cout << "r3=" << r_3 << endl;

		}
		else if (type == "01") //解析 I型指令
		{
			cout << "I型指令"<<endl;

		}
		else if (type == "10") //解析 J型指令
		{
			cout << "J型指令"<<endl;
		}


	//EX   译码阶段已经识别好三种类型 现在直接根据 op 做计算就行了
		switch (op) {
		case 0:            //ADD op=000000
			num_result = ADD(num_1, num_2);
			break;
		case 1:            //SUB op=000001
			num_result = SUB(num_1, num_2);
			break;


		}

		
	//MEM  暂时没有




	//WB  将中间结果写回寄存器
		R[r_3] = num_result;
		cout << "R3=" << R[r_3] << endl;
	
	}




}