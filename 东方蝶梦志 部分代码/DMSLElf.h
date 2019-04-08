#pragma once
namespace Dmsl {
	namespace VirtualMachine {
		enum  class ElfCode {
			//控制指令
			DMOVFS = 0x00,	//多重赋值，后跟参数：uint16_t 地址,n;double datas[n];将虚拟机指定的一个double赋值到地址为k的变量上*
			MOV = 0x01,	//从栈顶取值，赋值给参数（指针）指向的变量*
			CFUNC = 0x02,	//调用函数，参数为函数地址，注意有返回值，栈表现不同*
			CMET = 0x03,	//调用方法，参数为方法地址，没有返回值，栈表现不同*
			PUSH = 0x04,	//压栈，double*
			IF = 0x05,	//如果栈顶为bool = true，则无视此指令，否则跳转到指令参数指明的代码位置*
			JMP = 0x06,	//无条件跳转代码到参数位置*
			END = 0x07,	//结束执行*

			DUNPA = 0x08,   //直接解引用出Attr和Var并压栈*
			//DUNPU = 0x09,   //直接解引用出Uniform并压栈*

			//运算指令
			NEGA = 0x0A,	//栈顶取反压栈*

			//要求栈顶两个数为double，且压回double
			ADD = 0x0B,	//加法*
			SUB = 0x0C,	//减法*
			MUL = 0x0D,	//乘法*
			DIV = 0x0E,	//除法*
			MOD = 0x0F,	//取整后求余*

			//要求栈顶两个数为double，且压回BOOL
			BIG = 0x10,	//大于*
			BIGE = 0x11,	//大于等于*
			SML = 0x12,	//小于*
			SMLE = 0x13,	//小于等于*
			EQ = 0x14,	//等于*
			NE = 0x15,	//不等于*

			//要求栈顶两个数为BOOL，且压回BOOL
			AND = 0x16,	//与*
			OR = 0x17,	//或*
			NOT = 0x18,	//非*

			//VARYING用指令
			VARYING_MOV = 0x19,	//从栈顶写入值到Varying Out区域，参数为一个Address
			VARYING_READ_IN = 0x1A,	//从Varying In取出一个值压栈，参数为一个Address
			VARYING_UNP = 0x1B,	//从Varying Out区域取出一个值，压栈，参数为一个Address

			//基于时间轴的暂停
			PAUSE = 0x1C,	//无参数

			//DataActor操作
			//参数：ADDRESS var
			//栈顶：(top)FLOAT end,FLOAT time,FLOAT function
			DATAACTOR_ADD_ATTR = 0x1D,	//添加Attribute到DataActor
			DATAACTOR_ADD_VARYING = 0x1E	//添加Varying到DataActor
		};
	}
}
