#pragma once
#include <map>
#include <vector>
#include <string>
#include <stack>
#include "DMSLPrecompiler.h"
#include "DMSLElf.h"
#include "DMSL.h"

namespace Dmsl {
	namespace Compiler {
		/*struct Compiled {
			struct Unit {
				uint32_t memSize;
				std::vector<uint8_t> initProgram, mainProgram;
			};
			std::map<std::string, uint32_t> linkTable;
			std::map<uint32_t,uint32_t> metParamNum,funcParamNum;
			std::map<uint32_t, Unit> units;
			uint32_t uniformSize,cFuncSize,cMetSize;
		};*/

		//编译，成功则返回true
		bool Compile(const Precompiled& in, Compiled& out,std::ostream& log);

		//编译程序单元
		void CompileUnit(const Dmsl::Compiler::Precompiled& in,const Precompiled::Unit&,std::vector<uint8_t>& out,const std::vector<Precompiled::Unit::Code>& inProgram);

		//按行编译
		void CompileLines(const Dmsl::Compiler::Precompiled& in,const Precompiled::Unit&,std::vector<uint8_t>& out,const std::vector<Precompiled::Unit::Code>& inProgram,uint32_t begin,uint32_t end);

		//从begin开始搜索同一层代码中下个与leftWord相同的左词语句，遇到跳出开始处最低层代码的则直接返回end语句位置
		uint32_t SearchSameLayerBlockCode(const std::vector<Precompiled::Unit::Code>& code,const std::string& leftWord,uint32_t begin);

		//把double压进代码
		void PushDouble(double, std::vector<uint8_t>& out);

		//把地址压入代码，返回压入的地址的地址
		uintptr_t PushAddress(uintptr_t, std::vector<uint8_t>& out);

		//根据二级地址重新设置已经被压入的地址
		void ResetAddress(uintptr_t addressSaddress, uintptr_t address, std::vector<uint8_t>& out);

		//把指令压入代码
		void PushCmd(Dmsl::VirtualMachine::ElfCode, std::vector<uint8_t>& out);

		//解析出的表达式结构体
		struct ParsedMathWord final{
			enum {
				NUMBER, VARNAME, OPR, FUNC_NAME
			}type;
			bool nega = false;	//相反数
			struct Data{
				std::string varName;
				double number;
				struct Opr{
					int level;	//优先级记录
					enum {
						COMMA,	//1级优先级
						AND,
						OR,
						NOT,	//2级优先级
						NE,	//3级优先级
						EQ,
						BIG,
						BIGE,
						SML,
						SMLE,
						ADD,	//4级
						SUB,
						MUL,	//5
						DIV,
						MOD,
						CALL_FUNC,	//7
						NEGA    //6
					}opr;	//操作符
					uint32_t funcAddress;
					//遇见左括号，优先级+10，右括号-10
				}opr;
			}data;
		};

		//解析表达式到代码
		//返回值意义：
		//0 - 表达式返回1个Bool值
		//n(>=1) - 表达式返回n个double
		int ParseMath(const Precompiled& in,const Precompiled::Unit& unit, std::string, std::vector<uint8_t>& out);

		//解析表达式下层函数 -- 压入扫描到的数据
		void ParseData(const Precompiled& in, const Precompiled::Unit& unit,const ParsedMathWord&, std::vector<uint8_t>& out,std::stack<int>& testStk);

		//解析表达式下层函数 -- 压入扫描到的操作符
		void ParseOperator(const Precompiled& in,const ParsedMathWord&, std::vector<uint8_t>& out,std::stack<ParsedMathWord::Data::Opr>& stk,std::stack<int>& testStk);

		//弹出和写出操作符栈
		void PopOprStack(std::stack<ParsedMathWord::Data::Opr>& stk,std::vector<uint8_t>& out,std::stack<int>& testStk,const Precompiled& in);

		//查询物体类型
		//0 - 不知道
		//1 - attribute
		//2 - unit::var
		//3 - uniform
		//4 - constant_define
		//5 - number
		//6 - cfunc
		//7 - cmethod
		//8 - inVarying
		//9 - outVarying
		int WhatsTheVar(const Precompiled& in,const Precompiled::Unit& unit, const std::string& name);

		//递归处理if结构
		void CompileIfLines(const Dmsl::Compiler::Precompiled& in,const Precompiled::Unit&,std::vector<uint8_t>& out,const std::vector<Precompiled::Unit::Code>& inProgram,uint32_t begin,uint32_t end);

		//处理Times结构
		void CompileTimesLines(const Dmsl::Compiler::Precompiled& in, const Precompiled::Unit&, std::vector<uint8_t>& out, const std::vector<Precompiled::Unit::Code>& inProgram, uint32_t begin, uint32_t end,int times);
	}
}
