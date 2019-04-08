#pragma once
#include <vector>
#include <string>
#include <map>
#include <cstdint>
#include <string>
#include <set>


namespace Dmsl {
	namespace Compiler {

		//预编译成果
		struct Precompiled final{
			struct Unit {
				struct Code {
					std::string code;
					int line;
				};
				std::vector<Code> initProgram, mainProgram;	//初始化程序，主程序代码
				std::map<uint32_t, std::vector<Code>> custumProgram;	//自定义程序
				//std::set<uint32_t> customProgramLinkToC;	//要链接到C的自定义程序
				std::vector<std::pair<int, std::vector<Code>>> eventProgram;
				std::map<std::string, uint32_t> varTable;	//私有变量链接表
				std::map<std::string, uint32_t> inVarying,outVarying;	//varying符号表
				uint32_t memSize;	//内存占用大小
				uint32_t num;	//程序单元编号
			};
			std::map<std::string, uint32_t> attributeVars,cFuncs,cMet;	//属性变量，非易变变量链接表，C语言函数链接表，C语言方法
			std::map<std::string, double> constants;	//常量
			std::map<uint32_t, uint32_t> metParamNum, funcParmNum;	//方法参数数量和函数方法数量
			std::vector<Unit> units;	//程序单元
			std::map<std::string, uint32_t> custumProgramTable;	//自定义程序表
			std::set<uint32_t> interfaceTable;	//存在于其中的均为interface，否则需要从字节码中剔除
		};

		//预编译代码，返回true为成功
		bool Precompile(const std::string& codeStr,Precompiled&,std::ostream& log);

	}
}
