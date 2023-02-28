//执行txt中任意行的指令
//2.27    mips 执行完成
//2.28    更改 RISC-V的部分指令集，需要找到更好的ID方式，现在的方式形式不统一，且很复杂，是按op-funct3-funct7分类的，如果是按照六种类型分类也许更好

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


//RISC-V 指令集译码
//处理立即数时需要先拓展成32位

//需要31-0处理，是正着读机器码

//算数、移位、逻辑操作的op
// 判定逻辑 op -> funct3 -> funct7
//立即数op：0010011   
//寄存器op：0110011

//访存指令
//立即数：0000011
//寄存器：0100011

//J type
//1100011
//1100111

//csr
//1110011




//Instruction Decode  译码阶段
//获得指令中指定部分的结果，返回 int 
int ID_get(string Ins, int start, int size) {
	string r = Ins.substr(start, size);
	int num = stoi(r, 0, 2);
	return num;
}
//获取指令中的指定部分，返回 string
string ID_getString(string Ins, int start, int size) {
	string r = Ins.substr(start, size);
	return r;
}

//EX   执行阶段
int ADD(int r1, int r2) {
	return r1 + r2;
}
int SUB(int r1, int r2) {
	return r1 - r2;
}

int xori(int num, int imm) {
	int result= imm ^ num;
	return result;
}
int ori(int num, int imm){
	int result = imm | num;
	return result;
}
int andi(int num, int imm){
	int result = imm & num;
	return result;
}
int xor_(int num1, int num2){
	int result = num1 ^ num2;
	return result;
}
int or_(int num1, int num2){
	int result = num1 | num2;
	return result;
}
int and_(int num1, int num2){
	int result = num1 & num2;
	return result;
}





void main()
{
	string Ins;
	string filename = "E:\\VsCode\\SC\\SC\\I.txt";  //读取的文件名字
	
	int pc = 1;
	int R[32];   //模拟32个寄存器，需要根据标准mips库更改成4个int，4个floct等等功能寄存器

	
	for (int i = 0; i < 32; i++)
		R[i] = i;
	
	//假设初始化值


	while (stop) {
	//IF,取指，指定pc行的指令
		
		Ins= ReadLine(filename, pc);

		//计算pc,要读取的行数，可以放在流水线其他位置，暂定执行递增PC位置指令
		pc = pc + 1;
		if (pc == lines + 1)
			stop = false;              //txt中的指令执行完毕，结束循环



	//译码，解析op，同时取寄存器中值,需要分出来三种指令,r，i和j，根据某一位的不同分别译码
	//执行，switch后直接执行，或者计算一个 6 bit + 多选器选择 bit 的译码结果，在后期统一执行，或许在乘法运算时需要
		unsigned short int op = ID_get(Ins,25,7);
				
		//三个寄存器的号码
		unsigned short int rs1;
		unsigned short int rs2;
		unsigned short int rd;

		//两个funct 
		unsigned short int funct_3, funct_7;

		//立即数,所有指令最多一共4个立即数，从高位到低位数
		unsigned short int imm1,imm2,imm3,imm4;



		//两个取寄存器的操作数和 中间运算结果
		int num_1 ;
		int num_2 ;  
		int num_result;

		switch (op) {
			case 19:                  //op=0010011       R型
				funct_3 = ID_get(Ins, 17, 3);
				switch (funct_3) {
					case	0:		//funct3=000  addi
						cout << "Instruction:addi" << endl;
						break;
					case	2:		//funct3=010  slti
						cout << "Instruction:slti" << endl;
						break;
					case	3:		//funct3=011  sltiu
						cout << "Instruction:sltiu" << endl;
						break;
					case	1:		//funct3=001  slli
						cout << "Instruction:slli" << endl;
						break;
					case	5:		//funct3=101  srli/srai
						funct_7 = ID_get(Ins, 0, 7);
						if(funct_7==0)
							cout << "Instruction:srli" << endl;
						else
							cout << "Instruction:srai" << endl;
						break;
					
					case	4:		//funct3=100  xori
						cout << "Instruction:xori" << endl;
						imm1 = ID_get(Ins, 0, 12);
						rs1 = ID_get(Ins, 12, 5);
						rd = ID_get(Ins, 20, 5);
						num_1 = R[rs1];
						num_result = xori(num_1, imm1);
						R[rd] = num_result;
						cout <<"result=" << num_result << endl;
						break;
					case	6:		//funct3=110  ori
						cout << "Instruction:ori" << endl;
						imm1 = ID_get(Ins, 0, 12);
						rs1 = ID_get(Ins, 12, 5);
						rd = ID_get(Ins, 20, 5);
						num_1 = R[rs1];
						num_result = ori(num_1, imm1);
						R[rd] = num_result;
						cout << "result=" << num_result << endl;
						break;
					case	7:		//funct3=111  andi
						cout << "Instruction:andi" << endl;
						imm1 = ID_get(Ins, 0, 12);
						rs1 = ID_get(Ins, 12, 5);
						rd = ID_get(Ins, 20, 5);
						num_1 = R[rs1];
						num_result = andi(num_1, imm1);
						R[rd] = num_result;
						cout << "result=" << num_result << endl;
						break;
				
				}
				break;
				 
			case 51:					//OP=0110011     I型
				funct_3 = ID_get(Ins, 17, 3);
				switch (funct_3) {

					case	0:		//funct3=000  add/sub
						funct_7 = ID_get(Ins, 0, 7);
						if (funct_7 == 0)
							cout << "Instruction:add" << endl;
						else
							cout << "Instruction:sub" << endl;
						break;

					case	2:		//funct3=010  slt
						cout << "Instruction:xori" << endl;
						break;
					case	3:		//funct3=011  sltu
						cout << "Instruction:xori" << endl;
						break;
					case	1:		//funct3=001  sll
						cout << "Instruction:sll" << endl;
						break;
					case	5:		//funct3=101  srl/sra
						funct_7 = ID_get(Ins, 0, 7);
						if (funct_7 == 0)
							cout << "Instruction:srl" << endl;
						else
							cout << "Instruction:sra" << endl;
						break;

					case	4:		//funct3=100  xor
						cout << "Instruction:xor" << endl;
						rs1 = ID_get(Ins, 12, 5);
						rs2 = ID_get(Ins, 7, 5);
						rd = ID_get(Ins, 20, 5);
						num_1 = R[rs1];
						num_2 = R[rs2];
						num_result = xor_ (num_1, num_2);
						R[rd] = num_result;
						cout << "result=" << num_result << endl;
						break;
					case	6:		//funct3=110  or
						cout << "Instruction:or" << endl;
						rs1 = ID_get(Ins, 12, 5);
						rs2 = ID_get(Ins, 7, 5);
						rd = ID_get(Ins, 20, 5);
						num_1 = R[rs1];
						num_2 = R[rs2];
						num_result = or_ (num_1, num_2);
						R[rd] = num_result;
						cout << "result=" << num_result << endl;
						break;
					case	7:		//funct3=111  and
						cout << "Instruction:and" << endl;
						rs1 = ID_get(Ins, 12, 5);
						rs2 = ID_get(Ins, 7, 5);
						rd = ID_get(Ins, 20, 5);
						num_1 = R[rs1];
						num_2 = R[rs2];
						num_result = and_ (num_1, num_2);
						R[rd] = num_result;
						cout << "result=" << num_result << endl;
						break;
				}
				break;


			case 3:					//op=0000011  load/store
				cout << "Instruction:load/store" << endl;
				break;
			case 35:					//op=0100011  store    S型
				cout << "Instruction:load/store" << endl;
				break;
			case 99:					//op=1100011  J      B型
				cout << "Instruction:J type" << endl;
				break;
			case 103:				//op=1100111  J         J型
				cout << "Instruction:J type" << endl;
				break;
			case 115:				//op=1110011   csr
				cout << "Instruction:csre" << endl;
				break;
			default:
				cout << "Please Check Your Instruction" << endl;
				stop = false;
		}





		/*
		//if( type == "00")     //解析 R 型指令
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
*/
		
	//MEM  暂时没有




	//WB  将中间结果写回寄存器，可以根据译码结果确定是否写会，暂时放在译码的switch中，紧接ex之后
	//	R[r_3] = num_result;
	//	cout << "R3=" << R[r_3] << endl;
	
	}




}