#include "NuzPrecompiled.h"
#include "DMSLCompiler.h"
#include "DMSLUtils.h"
#include <stdexcept>
#include <ostream>
#include <algorithm>
#include <cstring>
#include "BKDRHash.h"
#include <Windows.h>

//#define _SHOW_ASM
#ifdef _SHOW_ASM
#include <iostream>
#endif

#define CheckNum(x) if(x != 1) throw invalid_argument("错误的表达式。");
#define CheckBool(x) if(x != 0) throw invalid_argument("错误的表达式。");

using namespace std;
using namespace Dmsl::Utils;
using Basic::BKDRHash;

static stack<vector<uint32_t>> continueStack,breakStack;

void Dmsl::Compiler::PopOprStack(stack<Dmsl::Compiler::ParsedMathWord::Data::Opr>& stk,vector<uint8_t>& out,std::stack<int>& testStk,const Precompiled& in){
    switch (stk.top().opr) {
    case ParsedMathWord::Data::Opr::AND:
        CheckBool(testStk.top());
        testStk.pop();
        CheckBool(testStk.top());
        testStk.pop();
        testStk.push(0);
        PushCmd(VirtualMachine::ElfCode::AND, out);
        break;
    case ParsedMathWord::Data::Opr::OR:
        CheckBool(testStk.top());
        testStk.pop();
        CheckBool(testStk.top());
        testStk.pop();
        testStk.push(0);
        PushCmd(VirtualMachine::ElfCode::OR, out); break;
    case ParsedMathWord::Data::Opr::NOT:
        CheckBool(testStk.top());
        testStk.pop();
        testStk.push(0);
        PushCmd(VirtualMachine::ElfCode::NOT, out); break;

    case ParsedMathWord::Data::Opr::NE:
        CheckNum(testStk.top());
        testStk.pop();
        CheckNum(testStk.top());
        testStk.pop();
        testStk.push(0);
        PushCmd(VirtualMachine::ElfCode::NE, out); break;
    case ParsedMathWord::Data::Opr::EQ:
        CheckNum(testStk.top());
        testStk.pop();
        CheckNum(testStk.top());
        testStk.pop();
        testStk.push(0);
        PushCmd(VirtualMachine::ElfCode::EQ, out); break;
    case ParsedMathWord::Data::Opr::BIG:
        CheckNum(testStk.top());
        testStk.pop();
        CheckNum(testStk.top());
        testStk.pop();
        testStk.push(0);
        PushCmd(VirtualMachine::ElfCode::BIG, out); break;
    case ParsedMathWord::Data::Opr::BIGE:
        CheckNum(testStk.top());
        testStk.pop();
        CheckNum(testStk.top());
        testStk.pop();
        testStk.push(0);
        PushCmd(VirtualMachine::ElfCode::BIGE, out); break;
    case ParsedMathWord::Data::Opr::SML:
        CheckNum(testStk.top());
        testStk.pop();
        CheckNum(testStk.top());
        testStk.pop();
        testStk.push(0);
        PushCmd(VirtualMachine::ElfCode::SML, out); break;
    case ParsedMathWord::Data::Opr::SMLE:
        CheckNum(testStk.top());
        testStk.pop();
        CheckNum(testStk.top());
        testStk.pop();
        testStk.push(0);
        PushCmd(VirtualMachine::ElfCode::SMLE, out); break;

    case ParsedMathWord::Data::Opr::ADD:
        CheckNum(testStk.top());
        testStk.pop();
        CheckNum(testStk.top());
        testStk.pop();
        testStk.push(1);
        PushCmd(VirtualMachine::ElfCode::ADD, out); break;
    case ParsedMathWord::Data::Opr::SUB:
        CheckNum(testStk.top());
        testStk.pop();
        CheckNum(testStk.top());
        testStk.pop();
        testStk.push(1);
        PushCmd(VirtualMachine::ElfCode::SUB, out); break;
    case ParsedMathWord::Data::Opr::MUL:
        CheckNum(testStk.top());
        testStk.pop();
        CheckNum(testStk.top());
        testStk.pop();
        testStk.push(1);
        PushCmd(VirtualMachine::ElfCode::MUL, out); break;
    case ParsedMathWord::Data::Opr::DIV:
        CheckNum(testStk.top());
        testStk.pop();
        CheckNum(testStk.top());
        testStk.pop();
        testStk.push(1);
        PushCmd(VirtualMachine::ElfCode::DIV, out); break;
    case ParsedMathWord::Data::Opr::MOD:
        CheckNum(testStk.top());
        testStk.pop();
        CheckNum(testStk.top());
        testStk.pop();
        testStk.push(1);
        PushCmd(VirtualMachine::ElfCode::MOD, out); break;

    case ParsedMathWord::Data::Opr::CALL_FUNC:{
            PushCmd(VirtualMachine::ElfCode::CFUNC, out);
            PushAddress(stk.top().funcAddress,out);
			PushAddress(in.funcParmNum.at(stk.top().funcAddress), out);
			break;
            /*int params = in.funcParmNum.at(stk.top().funcAddress);    //这里已经在ParseMath里检查
            for(int i = 0;i < params;++i){
                if(testStk.top() != 1) throw runtime_error("给函数传入了错误的参数。");
                testStk.pop();
            }
            testStk.push(1);*/
        }
        break;

    case ParsedMathWord::Data::Opr::NEGA:
        PushCmd(VirtualMachine::ElfCode::NEGA, out);
        break;
    case ParsedMathWord::Data::Opr::COMMA:
        break;
    }
    stk.pop();
}

bool Dmsl::Compiler::Compile(const Precompiled & in, Compiled & out,ostream& log)
{
    bool success = true;
    for(auto& p:in.units){
		if (out.units.count(p.num) >= 1) throw std::runtime_error("出现重复的unit:" + std::to_string(p.num));
		out.units[p.num].outVaryingSize = 0;
		auto& outVaryingSize = out.units[p.num].outVaryingSize;
		for (auto& q : p.outVarying)
			if (q.second >= outVaryingSize) outVaryingSize = q.second + 1;
        try{
            Dmsl::Compiler::CompileUnit(in,p,out.units[p.num].initProgram,p.initProgram);
        }catch(exception& e){
            log<<"在程序单元"<<p.num<<"的init块里有错误："<<e.what()<<endl;
            success = false;
        }
        try{
            Dmsl::Compiler::CompileUnit(in,p,out.units[p.num].mainProgram,p.mainProgram);
        }catch(exception& e){
            log<<"在程序单元"<<p.num<<"的main块里有错误："<<e.what()<<endl;
            success = false;
        }

		//out.custumProgramHandles = in.custumProgramTable;

		for (const auto& p : in.custumProgramTable)
		{
			auto hash = BKDRHash(p.first);
			out.custumProgramHandles[hash] = p.second;
		}

		for (auto b : p.custumProgram) {
			if (in.interfaceTable.count(b.first) == 0) continue;	//跳过非Interface单元
			try {
				
				//out.units[p.num].custumProgram.push_back(std::vector<uint8_t>());
				Dmsl::Compiler::CompileUnit(in, p, out.units[p.num].custumProgram[b.first],p.custumProgram.at(b.first));

			}
			catch (exception& e) {
				std::string ret;
				for (auto& k : out.custumProgramHandles) {
					if (k.second == b.first) {
						ret = k.first;
						break;
					}
				}
				log << "在程序单元" << p.num << "的" +ret +"块里有错误：" << e.what() << endl;
				success = false;
			}
		}


		for (auto b : p.eventProgram) {
			try {
				out.units[p.num].eventProgram.push_back({ b.first,std::vector<uint8_t>() });
				Dmsl::Compiler::CompileUnit(in, p,out.units[p.num].eventProgram.back().second, b.second);
			}
			catch (exception& e) {
				log << "在程序事件单元" << p.num << "的" + to_string(b.first) + "帧里有错误：" << e.what() << endl;
				success = false;
			}
		}

		std::sort(out.units[p.num].eventProgram.begin(), out.units[p.num].eventProgram.end());
        out.units[p.num].memSize = p.memSize;
    }

    //out.uniformSize = in.uniformVars.size();

	out.linkFuncSize = in.funcParmNum.size();
	out.linkMetSize = in.metParamNum.size();

    out.linkTable.clear();
    for(auto& p:in.attributeVars) out.linkTable[BKDRHash(p.first)] = p.second;
    /*for(auto& p:in.uniformVars){
        if(out.linkTable.count(p.first) == 1) log<<"Uniform变量 "<<p.first<<" 与已有的attribute变量重名。"<<endl;
        out.linkTable[p.first] = p.second;
    }*/
    for(auto& p:in.cFuncs){
        if(out.linkTable.count(BKDRHash(p.first)) == 1) log<<"函数 "<<p.first<<" 与已有的变量重名。"<<endl;
        out.linkTable[BKDRHash(p.first)] = p.second;
    }
    for(auto& p:in.cMet){
        if(out.linkTable.count(BKDRHash(p.first)) == 1) log<<"方法 "<<p.first<<" 与已有的变量或函数重名。"<<endl;
        out.linkTable[BKDRHash(p.first)] = p.second;
    }
    return success;
}

void Dmsl::Compiler::CompileUnit(const Dmsl::Compiler::Precompiled& in,const Precompiled::Unit& unit,std::vector<uint8_t>& out,const std::vector<Precompiled::Unit::Code>& inProgram){
    #ifdef _SHOW_ASM
    cout<<endl<<"----------------------------------Unit "<<unit.num<<endl;
    #endif // _SHOW_ASM
    while(!continueStack.empty()) continueStack.pop();
    while(!breakStack.empty()) breakStack.pop();
    CompileLines(in,unit,out,inProgram,0,inProgram.size());
    PushCmd(Dmsl::VirtualMachine::ElfCode::END,out);
    //cout<<endl<<"----------------------------------Unit Program End "<<unit.num<<endl;

	//寻找最大的OutVar编号
}

uint32_t Dmsl::Compiler::SearchSameLayerBlockCode(const std::vector<Precompiled::Unit::Code>& code,const std::string& leftWord,uint32_t begin){
    int layer = 0;
    for(;;++begin){
        //cout<<endl<<"SEARCH:"<<layer<<" "<<code[begin];
        if(layer == 0){
            if(ReadLeftWord(code[begin].code) == leftWord) return begin;
        };
		auto left = ReadLeftWord(code[begin].code);
        if(left == "if" || left == "while" || left == "for" ||left == "times")
            ++layer;
        if(left == "end"){
            if(layer == 0) return begin;
            else if(layer > 0) --layer;
            else throw runtime_error("未知的end错误。");
        }
    }
}

void Dmsl::Compiler::CompileLines(const Dmsl::Compiler::Precompiled& in, const Precompiled::Unit& unit, std::vector<uint8_t>& out, const std::vector<Precompiled::Unit::Code>& inProgram, const uint32_t begin, const uint32_t end) {
	for (uint32_t code_num = begin; code_num < end; ++code_num) {
#ifdef _SHOW_ASM
		cout << endl << ">>" << inProgram[code_num];
#endif // _SHOW_ASM
		const string& s = inProgram[code_num].code;
		try {
			string left = TrimAll(ReadLeftWord(s));
			string right = TrimRight(ReadRightLine(s));

			int leftType = WhatsTheVar(in, unit, TrimAll(left));
			if (leftType == 1 || leftType == 2 || leftType == 8 || leftType == 9) {
				if (right[0] == '<' && right[1] == '-') { //多重赋值意图
					auto doubles = TrimAll(right.substr(2, right.length() - 2));
					vector<string> doubleStrs;
					Split(doubles, doubleStrs, ':');
					PushCmd(Dmsl::VirtualMachine::ElfCode::DMOVFS, out);
					if (leftType == 1) PushAddress(in.attributeVars.at(left), out);
					else PushAddress(unit.varTable.at(left), out);
					PushAddress(doubleStrs.size(), out);
					for (auto& s : doubleStrs)
						PushDouble(ToDouble(s), out);
				}
				else if (right[0] == '=') { //表达式赋值意图
					auto bds = TrimAll(right.substr(1, right.length() - 1));
					left = TrimAll(left);
					int result = ParseMath(in, unit, bds, out);
					if (result != 1) throw invalid_argument("表达式的值不能被赋到变量上。");

					if (leftType == 1) {
						PushCmd(Dmsl::VirtualMachine::ElfCode::MOV, out);
						PushAddress(in.attributeVars.at(left), out);
					}
					else if (leftType == 2) {
						PushCmd(Dmsl::VirtualMachine::ElfCode::MOV, out);
						PushAddress(unit.varTable.at(left), out);
					}
					else if (leftType == 9) {
						PushCmd(Dmsl::VirtualMachine::ElfCode::VARYING_MOV, out);
						PushAddress(unit.outVarying.at(left), out);
					}
					else if (leftType == 8) {
						throw std::runtime_error("In Varying 不得被写入。");
					}
				}
			}
			else if (leftType == 7) {    //方法调用意图
				if (in.cMet.count(left) == 0) throw("未找到方法" + left + "。");
				if (in.metParamNum.at(in.cMet.at(left)) != 0)
					if (in.metParamNum.at(in.cMet.at(left)) != (uint32_t)ParseMath(in, unit, TrimAll(right), out))
						throw runtime_error("调用" + left + "方法时参数数量不一致。");
				PushCmd(Dmsl::VirtualMachine::ElfCode::CMET, out);
				PushAddress(in.cMet.at(left), out);
				PushAddress(in.metParamNum.at(in.cMet.at(left)), out);
			}
			else {
				if (left == "if") {   //IF意图
					//递归求if
					//任何if都是if/else这样最简单的形式
					auto newEnd = SearchSameLayerBlockCode(inProgram, "end", code_num + 1);
#ifdef _SHOW_ASM
					cout << endl << "----------IF BLOCK-----------" << endl;
					for (auto i = code_num; i < newEnd; ++i)
						cout << inProgram[i] << endl;
					cout << endl << "----------IF BLOCK END-----------" << endl;
#endif
					CompileIfLines(in, unit, out, inProgram, code_num, newEnd);
					code_num = newEnd;
					continue;
				}
				else if (left == "while") { //WHILE意图
					breakStack.push(vector<uint32_t>());
					continueStack.push(vector<uint32_t>());

					//条件代码
					auto continueAddress = out.size();
					if (ParseMath(in, unit, right, out) != 0) throw runtime_error("表达式不适合while。");
					PushCmd(Dmsl::VirtualMachine::ElfCode::IF, out);
					breakStack.top().push_back(PushAddress(0, out));

					//while内容
					auto newBegin = code_num + 1;  //搜索代码块上限
					auto newEnd = SearchSameLayerBlockCode(inProgram, "end", newBegin);  //搜索代码块下限
#ifdef _SHOW_ASM
					cout << endl << "----------WHILE BLOCK----------" << endl;
					for (auto i = newBegin; i < newEnd; ++i) cout << inProgram[i] << endl;
					cout << endl << "----------WHILE BLOCK END----------" << endl;
#endif
					CompileLines(in, unit, out, inProgram, newBegin, newEnd);  //编译代码块
					code_num = newEnd;

					//while结尾
					PushCmd(Dmsl::VirtualMachine::ElfCode::JMP, out);
					PushAddress(continueAddress, out);

					auto breakAddress = out.size();

					for (auto link : breakStack.top()) ResetAddress(link, breakAddress, out);
					for (auto link : continueStack.top()) ResetAddress(link, continueAddress, out);

					breakStack.pop();
					continueStack.pop();
				}
				else if (left == "for") {  //FOR意图
					breakStack.push(vector<uint32_t>());
					continueStack.push(vector<uint32_t>());

					//分割代码
					string init, test, step;
					vector<Precompiled::Unit::Code> vec;
					vector<string> split;

					Split(right, split, ',');
					if (split.size() != 3) {
						throw runtime_error("for语句必须有两个逗号和三个子语句。");
					}
					init = TrimRight(split[0]);
					test = TrimAll(split[1]);
					step = TrimRight(split[2]);


					//编译初始化代码
					//cout<<init<<endl;
					vec.clear();
					vec.push_back({ init,inProgram[code_num].line });
					CompileLines(in, unit, out, vec, 0, 1);


					auto testAddress = out.size();  //条件代码地址
					//编译条件代码
					if (ParseMath(in, unit, test, out) != 0) throw runtime_error("该表达式不适用于for的条件表达式。");
					PushCmd(Dmsl::VirtualMachine::ElfCode::IF, out);
					breakStack.top().push_back(PushAddress(0, out)); //IF为假，作为break链接

					//搜索上下限，编译for内容
					auto newBegin = code_num + 1;  //搜索代码块上限
					auto newEnd = SearchSameLayerBlockCode(inProgram, "end", newBegin);  //搜索代码块下限

#ifdef _SHOW_ASM
					cout << endl << "---------FOR BLOCK-----------" << endl;
					for (auto i = newBegin; i < newEnd; ++i) cout << inProgram[i] << endl;
					cout << "---------FOR BLOCK END-----------" << endl;
#endif // _SHOW_ASM

					CompileLines(in, unit, out, inProgram, newBegin, newEnd);  //编译代码块
					code_num = newEnd;

					//记录步进代码位置
					auto continueAddress = out.size();

					//编译步进代码
					vec.clear();
					vec.push_back({ step,inProgram[code_num].line });
					CompileLines(in, unit, out, vec, 0, 1);

					//跳转到条件代码
					PushCmd(Dmsl::VirtualMachine::ElfCode::JMP, out);
					PushAddress(testAddress, out);

					//记录break地址
					auto breakAddress = out.size();

					for (auto link : breakStack.top()) ResetAddress(link, breakAddress, out);
					for (auto link : continueStack.top()) ResetAddress(link, continueAddress, out);

					breakStack.pop();
					continueStack.pop();
				}
				else if (left == "times") {
					breakStack.push(vector<uint32_t>());
					continueStack.push(vector<uint32_t>());
					int times = ToInt(right);
					auto newBegin = code_num + 1;  //搜索代码块上限
					auto newEnd = SearchSameLayerBlockCode(inProgram, "end", newBegin);  //搜索代码块下限

					CompileTimesLines(in, unit, out, inProgram, newBegin, newEnd, times);
					code_num = newEnd;

					breakStack.pop();
					continueStack.pop();
				}
				else if (left == "exit") { //EXIT意图
					PushCmd(Dmsl::VirtualMachine::ElfCode::END, out);
				}
				else if (left == "pause") {	//暂停意图
					ParseMath(in, unit, right, out);
					PushCmd(Dmsl::VirtualMachine::ElfCode::PAUSE, out);
				}
				else if (left == "actor") {
					std::vector<std::string> params;
					Dmsl::Utils::Split(TrimAll(right), params, ',');

					ParseMath(in, unit, params.at(1), out);
					ParseMath(in, unit, params.at(2), out);
					if (params.size() >= 4)
						ParseMath(in, unit, params[3], out);
					else
						ParseMath(in, unit, "0", out);

					int type = WhatsTheVar(in, unit, params[0]);
					if (type == 1 || type == 2) {
						//变换Attribute
						PushCmd(Dmsl::VirtualMachine::ElfCode::DATAACTOR_ADD_ATTR, out);

						if (type == 1)
							PushAddress(in.attributeVars.at(params[0]), out);
						else if (type == 2)
							PushAddress(unit.varTable.at(params[0]), out);
					}
					else if (type == 9) {
						//变换VaryingOut
						PushCmd(Dmsl::VirtualMachine::ElfCode::DATAACTOR_ADD_VARYING, out);
						PushAddress(unit.outVarying.at(params[0]), out);
					}
					else throw("无法变换标识符：" + right);

				}
				else if (left == "continue") { //CONTINUE意图
					if (continueStack.empty()) throw runtime_error("continue不在循环块中。");
					PushCmd(Dmsl::VirtualMachine::ElfCode::JMP, out);
					continueStack.top().push_back(PushAddress(0, out));
				}
				else if (left == "break") { //BREAK意图
					if (breakStack.empty()) throw runtime_error("break不在循环块中。");
					PushCmd(Dmsl::VirtualMachine::ElfCode::JMP, out);
					breakStack.top().push_back(PushAddress(0, out));
				}
				else if (in.custumProgramTable.count(left)) {
					auto methodHandle = in.custumProgramTable.at(left);
					if (unit.custumProgram.count(methodHandle) == 0) {
						throw runtime_error("单元 " + std::to_string(unit.num) + " 中不存在方法 " + left + "。");
					}
					else {
						CompileLines(in, unit, out, unit.custumProgram.at(methodHandle), 0, unit.custumProgram.at(methodHandle).size());
					}
				}

				else
				{
					if (left != "end")
					{
						std::string warning = "Unknown met:" + left + "\n";
						//OutputDebugString(warning.c_str());
					}
				}
			}
		}
		catch (std::exception e) {
			throw runtime_error("在行" + std::to_string(inProgram[code_num].line) + "有错误：" + e.what());
		}
	}
}

void Dmsl::Compiler::PushDouble(double a, std::vector<uint8_t>& out)
{
	for (uint16_t i = 0; i < sizeof(double); ++i) {
		uint8_t* c = (uint8_t*)&a;
		out.push_back(c[i]);
	}
	#ifdef _SHOW_ASM
	cout<<" "<<a;
	#endif // _SHOW_ASM
}

uintptr_t Dmsl::Compiler::PushAddress(uintptr_t a, std::vector<uint8_t>& out)
{
	uintptr_t addressAddress = out.size();
	for (uint16_t i = 0; i < sizeof(uintptr_t); ++i) {
		uint8_t* p = (uint8_t*)&a;
		out.push_back(p[i]);
	}
	#ifdef _SHOW_ASM
	cout<<" (ADDRESS)"<<a;
	#endif // _SH
	return addressAddress;
}

void Dmsl::Compiler::ResetAddress(uintptr_t addressSaddress, uintptr_t address, std::vector<uint8_t>& out){
	for (uint32_t i = 0; i < sizeof(uintptr_t); ++i) {
		uint8_t* p = (uint8_t*)&address;
		out[addressSaddress+i] = p[i];
	}
	#ifdef _SHOW_ASM
	cout<<endl<<"Link address:"<<addressSaddress<<"->"<<address;
	#endif // _SHOW_ASM
}

void Dmsl::Compiler::PushCmd(Dmsl::VirtualMachine::ElfCode c, std::vector<uint8_t>& out)
{
	out.push_back((uint8_t)c);
	/*for (unsigned i = 0; i < sizeof(void*) - 1; ++i)
		out.push_back(0);*/
	#ifdef _SHOW_ASM
	cout<<endl<<hex<<(int)c;
	#endif // _SHOW_ASM
}

int Dmsl::Compiler::ParseMath(const Precompiled & in,const Precompiled::Unit& unit, std::string  s, std::vector<uint8_t>& out)
{
    #ifdef _SHOW_ASM
    cout<<endl<<"ParseMath:"<<s;
    #endif // _SHOW_ASM
    stack<int> testStk;   //0为bool，1为double

    stack<string> funcStack;    //函数名栈
    stack<int> funcLayerStack;  //函数括号层栈
    stack <bool> negaThisStack;  //括号前是否取反当前值

	if (s.empty()) throw runtime_error("无效的表达式。");
	s = TrimAll(s);

    bool nextSubIsNega = true;  //下一个减号为负号
    bool negaThis = false;  //下一个取负值
    int baseLevel = 0;  //当前基本优先级
    int khLayer = 0;
    bool nextIsFunc = false;

    ParsedMathWord word;
    stack<ParsedMathWord::Data::Opr> oprStk;
    while(1){
        string token = ReadToken(s);
        if(token.empty()) break;
		 token = TrimAll(token);
        //cout<<"Token:"<<token<<endl;

        if(token == "-" && nextSubIsNega){negaThis = true;nextSubIsNega = false;continue;}
        nextSubIsNega = false;

        if(token == "("){
                baseLevel += 10;
                nextSubIsNega=true;
                khLayer++;
                nextIsFunc = false;
                negaThisStack.push(negaThis);
                negaThis = false;
                continue;
        }
        else if(token == ")"){
            baseLevel -= 10;
            nextSubIsNega=false;
            khLayer--;
            if(!funcLayerStack.empty()){
                if(funcLayerStack.top() == khLayer){
                    auto funcAddress = in.cFuncs.at(funcStack.top());
                    auto funcArgs = in.funcParmNum.at(funcAddress);
                    for(unsigned i = 0;i < funcArgs;++i){
                        CheckNum(testStk.top());
                        testStk.pop();
                    }
                    word.type = ParsedMathWord::OPR;
                    word.data.opr.level = baseLevel+8;
                    word.data.opr.opr = ParsedMathWord::Data::Opr::CALL_FUNC;
                    word.data.opr.funcAddress = funcAddress;
                    testStk.push(1);
                    ParseOperator(in,word,out,oprStk,testStk);

                    funcLayerStack.pop();
                    funcStack.pop();
                }
            }
            if(negaThisStack.top()){
                word.type = ParsedMathWord::OPR;
                word.data.opr.opr = ParsedMathWord::Data::Opr::NEGA;
                word.data.opr.level = baseLevel + 7;
                ParseOperator(in,word,out,oprStk,testStk);
            }
            negaThisStack.pop();
            continue;
        }
        if(nextIsFunc) throw runtime_error("函数调用后面没有括号。");

        if(negaThis) word.nega = true;
        else word.nega = false;
        negaThis = false;

        if(isdigit(token[0])){  //读取到数字
            word.type = ParsedMathWord::NUMBER;
            word.data.number = ToDouble(token);
            ParseData(in,unit,word,out,testStk);
            continue;
        }

        if(isalpha(token[0])){  //读到英文
            if(WhatsTheVar(in,unit,token) == 6){    //如果是函数
                funcStack.push(token);
                funcLayerStack.push(khLayer);
                nextIsFunc = true;
                if(word.nega) negaThis = true;
                continue;
            }
            else if((WhatsTheVar(in,unit,token) >= 1 && WhatsTheVar(in,unit,token) <= 4) || WhatsTheVar(in, unit, token) == 9 || WhatsTheVar(in, unit, token) == 8){   //否则是量
                word.type = ParsedMathWord::VARNAME;
                word.data.varName = token;
                ParseData(in,unit,word,out,testStk);
                continue;
            }
        }

        //此处以后为操作符
        word.type = ParsedMathWord::OPR;
        if(token == "+"){
            word.data.opr.level = 5 + baseLevel;
            word.data.opr.opr = ParsedMathWord::Data::Opr::ADD;
            nextSubIsNega = true;
        }else if(token == "-"){
            word.data.opr.level = 5 + baseLevel;
            word.data.opr.opr = ParsedMathWord::Data::Opr::SUB;
            nextSubIsNega = true;
        }else if(token == "*"){
            word.data.opr.level = 6 + baseLevel;
            word.data.opr.opr = ParsedMathWord::Data::Opr::MUL;
            nextSubIsNega = true;
        }else if(token == "/"){
            word.data.opr.level = 6 + baseLevel;
            word.data.opr.opr = ParsedMathWord::Data::Opr::DIV;
            nextSubIsNega = true;
        }else if(token == "%"){
            word.data.opr.level = 6 + baseLevel;
            word.data.opr.opr = ParsedMathWord::Data::Opr::MOD;
        }else if(token == ">"){
            word.data.opr.level = 3 + baseLevel;
            word.data.opr.opr = ParsedMathWord::Data::Opr::BIG;
            nextSubIsNega = true;
            nextSubIsNega = true;
        }else if(token == ">="){
            word.data.opr.level = 3 + baseLevel;
            word.data.opr.opr = ParsedMathWord::Data::Opr::BIGE;
            nextSubIsNega = true;
        }else if(token == "<"){
            word.data.opr.level = 3 + baseLevel;
            word.data.opr.opr = ParsedMathWord::Data::Opr::SML;
            nextSubIsNega = true;
        }else if(token == "<="){
            word.data.opr.level = 3 + baseLevel;
            word.data.opr.opr = ParsedMathWord::Data::Opr::SMLE;
            nextSubIsNega = true;
        }else if(token == "=="){
            word.data.opr.level = 3 + baseLevel;
            word.data.opr.opr = ParsedMathWord::Data::Opr::EQ;
            nextSubIsNega = true;
        }else if(token == "!="){
            word.data.opr.level = 3 + baseLevel;
            word.data.opr.opr = ParsedMathWord::Data::Opr::NE;
            nextSubIsNega = true;
        }else if(token == "!"){
            word.data.opr.level = 2 + baseLevel;
            word.data.opr.opr = ParsedMathWord::Data::Opr::NOT;
            nextSubIsNega = true;
        }else if(token == "&&"){
            word.data.opr.level = 1 + baseLevel;
            word.data.opr.opr = ParsedMathWord::Data::Opr::AND;
            nextSubIsNega = true;
        }else if(token == "||"){
            word.data.opr.level = 1 + baseLevel;
            word.data.opr.opr = ParsedMathWord::Data::Opr::OR;
            nextSubIsNega = true;
        }else if(token == ","){
            word.data.opr.level = baseLevel;
            word.data.opr.opr = ParsedMathWord::Data::Opr::COMMA;
            nextSubIsNega = true;
		}
		else {
			int v = WhatsTheVar(in, unit, token);
			throw invalid_argument("错误的表达式。");
		}
        ParseOperator(in,word,out,oprStk,testStk);
    }
    while(!oprStk.empty()) PopOprStack(oprStk,out,testStk,in);

    if(testStk.size() == 1){
        return testStk.top();
    }else{
        int ret = testStk.size();
        while(!testStk.empty()){
            if(testStk.top() == 0) throw invalid_argument("表达式的结果错误。");
            testStk.pop();
        }
        return ret;
    }
}

void Dmsl::Compiler::ParseData(const Precompiled & in,const Precompiled::Unit& unit,const ParsedMathWord & s, std::vector<uint8_t>& out,std::stack<int>& testStk)
{
    testStk.push(1);
	if (s.type == ParsedMathWord::NUMBER) {
		PushCmd(VirtualMachine::ElfCode::PUSH, out);
		if(s.nega) PushDouble(-s.data.number, out);
		else PushDouble(s.data.number, out);
	}
	else if (s.type == ParsedMathWord::VARNAME) {
		switch (WhatsTheVar(in, unit, s.data.varName)) {
		case 1:
			PushCmd(VirtualMachine::ElfCode::DUNPA, out);
			PushAddress(in.attributeVars.at(s.data.varName), out);
			if(s.nega) PushCmd(Dmsl::VirtualMachine::ElfCode::NEGA,out);
			break;
		case 2:
			PushCmd(VirtualMachine::ElfCode::DUNPA, out);
			PushAddress(unit.varTable.at(s.data.varName), out);
			if(s.nega) PushCmd(Dmsl::VirtualMachine::ElfCode::NEGA,out);
			break;
		/*case 3:
			PushCmd(VirtualMachine::ElfCode::DUNPU, out);
			PushAddress(in.uniformVars.at(s.data.varName), out);
			if(s.nega) PushCmd(Dmsl::VirtualMachine::ElfCode::NEGA,out);
			break;*/
		case 4:
			PushCmd(VirtualMachine::ElfCode::PUSH, out);
			if(s.nega) PushDouble(-in.constants.at(s.data.varName), out);
			else PushDouble(in.constants.at(s.data.varName), out);
			//cout<<"SSSSSSSSSSSSS"<<endl;
			break;
		case 8:
			PushCmd(VirtualMachine::ElfCode::VARYING_READ_IN, out);
			PushAddress(unit.inVarying.at(s.data.varName), out);
			break;
		case 9:
			PushCmd(VirtualMachine::ElfCode::VARYING_UNP, out);
			PushAddress(unit.outVarying.at(s.data.varName), out);
			break;
		default:
			throw runtime_error("虽然不知道发生了什么，但是这里表达式不正常。");
		}
	}
}



void Dmsl::Compiler::ParseOperator(const Precompiled & in, const ParsedMathWord & word, std::vector<uint8_t>& out, std::stack<ParsedMathWord::Data::Opr>& stk,std::stack<int>& testStk)
{
	if (stk.empty()) {
		stk.push(word.data.opr);
	}
	else if (stk.top().level < word.data.opr.level) {
		stk.push(word.data.opr);
	}
	else {
		while (stk.top().level >= word.data.opr.level) {
            PopOprStack(stk,out,testStk,in);
            if (stk.empty()) break;
		}
		stk.push(word.data.opr);
	}
}

/*
if 条件1                  if 条件1
	代码1                     代码1
else                       end
	代码2
end
============================
;条件                     ;条件
if e                        if e
代码1                     ;代码1
jmp endif               jmp endif
--e                         --e
代码2                     --endif
--endif

*/
using namespace Dmsl::Compiler;
void Dmsl::Compiler::CompileIfLines(const Dmsl::Compiler::Precompiled& in, const Precompiled::Unit& unit, std::vector<uint8_t>& out, const std::vector<Precompiled::Unit::Code>& inProgram, uint32_t begin, uint32_t end)
{
    vector<Precompiled::Unit::Code> subProgram;

    //cout<<"IF:"<<TrimRight(ReadRightLine(inProgram[begin]))<<endl;
    ParseMath(in,unit,TrimRight(ReadRightLine(inProgram[begin].code)),out);
    PushCmd(Dmsl::VirtualMachine::ElfCode::IF,out);
    auto linkTo_e = PushAddress(0,out);
    auto newBegin = begin+1;
    auto newEnd = SearchSameLayerBlockCode(inProgram,"else",newBegin);
    #ifdef _SHOW_ASM
    cout<<endl<<endl<<"---------IF CODE BLOCK----------"<<endl;
    for(auto m = newBegin;m < newEnd;++m){
        cout<<inProgram[m]<<endl;
    }
    cout<<endl<<endl<<"---------IF CODE BLOCK END----------"<<endl;
    #endif // _SHOW_ASM
    CompileLines(in,unit,out,inProgram,newBegin,newEnd);
    PushCmd(Dmsl::VirtualMachine::ElfCode::JMP,out);
    auto linkTo_endif = PushAddress(0,out);
    ResetAddress(linkTo_e,out.size(),out);

    if(ReadLeftWord(TrimRight(inProgram[newEnd].code)) == "else"){
        auto line = ReadRightLine(TrimRight(inProgram[newEnd].code));
        bool isSubIf = false;
        if(TrimAll(ReadLeftWord(TrimRight(line))) == "if"){
			subProgram.push_back({ TrimRight(line),inProgram[newEnd].line });
            isSubIf = true;
        }

        auto endPos = SearchSameLayerBlockCode(inProgram,"end",newBegin);
        for(uint32_t i = newEnd + 1;i < endPos;++i) subProgram.push_back(inProgram[i]);

		if (isSubIf) subProgram.push_back({ "end",-1 });

        if(isSubIf) CompileIfLines(in,unit,out,subProgram,0,subProgram.size());
        else CompileLines(in,unit,out,subProgram,0,subProgram.size());
    }
    #ifdef _SHOW_ASM
    cout<<endl<<"-----------ELSE BLOCK-----------"<<endl;
    for(auto& s:subProgram){
        cout<<s<<endl;
    }
    cout<<endl<<"-----------ELSE BLOCK END-----------"<<endl;
    #endif // _SHOW_ASM
    ResetAddress(linkTo_endif,out.size(),out);
}

void Dmsl::Compiler::CompileTimesLines(const Dmsl::Compiler::Precompiled & in, const Precompiled::Unit & unit, std::vector<uint8_t>& out, const std::vector<Precompiled::Unit::Code>& inProgram, uint32_t begin, uint32_t end,int times)
{
	for (int i = 0; i < times; ++i) {
		CompileLines(in, unit, out, inProgram, begin, end);

		//链接continue语句
		for(auto& p:continueStack.top())
			ResetAddress(p, out.size(), out);
		continueStack.top().clear();
	}

	//链接break语句
	for (auto& p : breakStack.top())
		ResetAddress(p, out.size(), out);
}


int Dmsl::Compiler::WhatsTheVar(const Precompiled & in, const Precompiled::Unit& unit, const std::string & name)
{
	if (in.attributeVars.count(name)) return 1;
	else if (in.cFuncs.count(name)) return 6;
	else if (in.cMet.count(name)) return 7;
	else if (in.constants.count(name)) return 4;
	//else if (in.uniformVars.count(name)) return 3;
	else if (unit.varTable.count(name)) return 2;
	else if (unit.inVarying.count(name)) return 8;
	else if (unit.outVarying.count(name)) return 9;
	else {
		bool isNum = true;
		for (char ch : name)
			if (!isdigit(ch) || ch != '.') isNum = false;
		if (isNum) return 5;
	}
	//throw invalid_argument("未知的标识符：" + name);
	return 0;
}
