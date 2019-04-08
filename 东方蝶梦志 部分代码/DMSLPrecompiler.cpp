#include "NuzPrecompiled.h"
#undef ERROR
#include "DMSLPrecompiler.h"
#include "DMSLUtils.h"
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "Globals.h"
#include "BKDRHash.h"
using namespace std;
using Basic::BKDRHash;
using namespace Dmsl::Utils;


namespace Dmsl {
	namespace Compiler {
		//预编译代码
		bool Precompile(const std::string& codeStr, Precompiled& comp, ostream& log) {
			std::map<std::string, uint32_t> inVarying, outVarying;	//全局的in和out的Varying表
			bool success = true;
			int nowLine = 0;
			stringstream code(codeStr);

			uint32_t layer = 0;	//当前代码块层数
			int32_t unitNum = -1;	//当前激活的程序单元

			uint32_t nextUniform = 0;	//下一个uniform的编号
			uint32_t nextAttribute = 0;	//下一个attribute的编号
			bool disableAttribute = false;	//禁止attribute出现
			uint32_t nextCFunc = 0;	//下一个C语言链接编号
			uint32_t nextCMet = 0;	//下一个C语言方法编号

			uint32_t nextVar = 0;	//下一个局部变量编号
			std::vector<Precompiled::Unit>::iterator nowUnit;	//当前的Unit
			enum { INIT, MAIN, ERROR, CUSTUM, EVENT, CANDY_ONCE }nowProgram = ERROR;	//当前程序块

			vector<Precompiled::Unit::Code> candyCode;
			string candyRight;

			std::string custumModule;	//自定义模块的名字
			int custumModuleHandle;	//自定义模块编号

			std::map<int, std::string> handle2module;	//编号对名字的链接



			while (!code.eof()) {
				try {
					string line;
					getline(code, line);
					++nowLine;


					line = TrimRight(DeleteComments(line));

					if (TrimAll(line) == "`") line = "end";

					if (TrimAll(line)[0] == '[') {
						auto pos = line.find('[');
						line = line.substr(pos + 1, line.length() - 1);
						while (1) {
							string app;
							getline(code, app);

							line += ' ' + TrimRight(DeleteComments(app));

							if (TrimAll(app).back() == ']') {
								auto pos = line.find(']');
								line = line.substr(0,pos);
								//Log("%s\n", line.c_str());
								break;
							}
						}
					}
					
					if (line.empty()) continue;
					else if (line[0] == '$') {
						//预编译指令
						line = line.substr(1, line.size() - 1);
						auto cmd = ReadLeftWord(line);
						if (cmd == "resetline") {
							nowLine = 0;
						}

						continue;
					}


					if (layer == 0) {	//全局层
						string left = ReadLeftWord(line);
						string right = ReadRightLine(line);
						if (left == "unit") {
							disableAttribute = true;
							layer++;
							unitNum = ToInt(TrimAll(right));
							comp.units.push_back(Precompiled::Unit());
							nowUnit = comp.units.end();
							--nowUnit;
							nowUnit->num = unitNum;
							nextVar = nextAttribute;
							nowUnit->outVarying = outVarying;
							nowUnit->inVarying = inVarying;
						}
						else if (left == "interface") {
							custumModule = TrimAll(right);
							if (comp.custumProgramTable.count(custumModule)) {
								custumModuleHandle = comp.custumProgramTable[custumModule];
							}
							else {
								custumModuleHandle = 0;
								while (handle2module.count(custumModuleHandle)) {
									custumModuleHandle++;
								}
								comp.custumProgramTable[custumModule] = custumModuleHandle;
								handle2module[custumModuleHandle] = custumModule;
							}
							comp.interfaceTable.insert(custumModuleHandle);
						}
						else if (left == "uniform") {
							/*vector<string> unis;
							Split(right, unis, ',');
							for (auto& s : unis) {
								s = TrimAll(s);
								CheckName(s);
								comp.uniformVars[s] = nextUniform++;
							}*/
							throw runtime_error("uniform型变量已经被移除。");
						}
						else if (left == "attribute") {
							if (disableAttribute) throw runtime_error("此处不应再出现attribute，你应该把它放在最前边。");
							vector<string> attrs;
							Split(right, attrs, ',');
							for (auto& s : attrs) {
								s = TrimAll(s);
								CheckName(s);
								comp.attributeVars[s] = nextAttribute++;
							}
						}
						else if (left == "cfunc") {
							auto pos = right.find(',');
							if (pos == string::npos) throw runtime_error("错误的C语言函数链接：没提供参数个数？");
							auto name = right.substr(0, pos);
							name = TrimAll(name);
							CheckName(name);
							comp.cFuncs[name] = nextCFunc;
							comp.funcParmNum[nextCFunc++] = ToInt(TrimAll(right.substr(pos + 1, right.length() - pos - 1)));
						}
						else if (left == "cmethod") {
							auto pos = right.find(',');
							if (pos == string::npos) throw runtime_error("错误的C语言方法链接：没提供参数个数？");
							auto name = right.substr(0, pos);
							name = TrimAll(name);
							CheckName(name);
							comp.cMet[name] = nextCMet;
							comp.metParamNum[nextCMet++] = ToInt(TrimAll(right.substr(pos + 1, right.length() - pos - 1)));
						}
						else if (left == "const") {
							auto pos = right.find('=');
							auto name = right.substr(0, pos);
							name = TrimAll(name);
							CheckName(name);
							comp.constants[name] = ToDouble(TrimAll(right.substr(pos + 1, right.length() - pos - 1)));
						}
						/*else if (left == "in") {
							auto pos = right.find(':');
							auto name = TrimAll(right.substr(0, pos));
							if (inVarying.count(name)) throw std::runtime_error("重定义InVarying:" + name);
							inVarying[name] = ToInt(right.substr(pos + 1, right.length() - pos - 1));
						}
						else if (left == "out") {
							auto pos = right.find(':');
							auto name = TrimAll(right.substr(0, pos));
							if (outVarying.count(name)) throw std::runtime_error("重定义OutVarying:" + name);
							outVarying[name] = ToInt(right.substr(pos + 1, right.length() - pos - 1));
						}*/
						else throw runtime_error("未知意图。");

					}
					else if (layer == 1) {	//在unit内
						string left = ReadLeftWord(line);
						string right = ReadRightLine(line);

						if (left == "end") {
							--layer;
							nowUnit->memSize = nowUnit->varTable.size() + comp.attributeVars.size();
						}
						else if (left == "init") {
							++layer;
							nowProgram = INIT;
						}
						else if (left == "main") {
							++layer;
							nowProgram = MAIN;
						}
						else if (left == "var" || left == "v") {
							vector<string> vars;
							Split(right, vars, ',');
							for (auto& s : vars) {
								s = TrimAll(s);
								CheckName(s);
								if (nowUnit->varTable.count(s)) throw std::runtime_error("重定义var："+s);
								nowUnit->varTable[s] = nextVar++;
							}
						}
						else if (left == "in") {
							std::vector<string> v;
							Split(right, v, ',');

							for (auto& p : v) {
								auto pos = p.find(':');
								auto name = TrimAll(p.substr(0, pos));
								if (nowUnit->inVarying.count(name)) throw std::runtime_error("重定义InVarying:" + name);
								CheckName(name);

								if (pos == string::npos) {
									Crash(L"语法错误！in后缺少标签。");
								}
								else {
									nowUnit->inVarying[name] = ToInt(p.substr(pos + 1, p.length() - pos - 1));
								}
							}
						}
						else if (left == "incopy" || left == "ic") {
							std::vector<string> v;
							Split(right, v, ',');

							for (auto& p : v) {
								auto pos = p.find(':');
								if(pos == string::npos) 
									Crash(L"语法错误！incopy后缺少标签。");
								auto name = TrimAll(p.substr(0, pos));
								if (nowUnit->inVarying.count(name)) throw std::runtime_error("重定义InVarying:" + name);
								int inPoint = pos == string::npos ? nowUnit->inVarying .size():ToInt(p.substr(pos + 1, p.length() - pos - 1));
								nowUnit->inVarying["DDDDDDDDDmslInCopyCandy"+to_string(inPoint)] = inPoint;
								CheckName(name);
								if (nowUnit->varTable.count(name) == 1) throw std::runtime_error("在incopy基础上多余定义了var：" + name);
								if(comp.attributeVars.count(name) == 0) nowUnit->varTable[name] = nextVar++;
								nowUnit->initProgram.push_back({ name + " = DDDDDDDDDmslInCopyCandy" + to_string(inPoint) });
							}
						}
						else if (left == "out") {
							std::vector<string> v;
							Split(right, v, ',');

							for (auto& p : v) {
								auto pos = p.find(':');
								if(pos == std::string::npos) 
									Crash(L"语法错误out后缺少标签。");
								auto name = TrimAll(p.substr(0, pos));
								if (nowUnit->outVarying.count(name)) throw std::runtime_error("重定义OutVarying:" + name);
								nowUnit->outVarying[name] = pos == string::npos ? nowUnit->outVarying .size():ToInt(p.substr(pos + 1, p.length() - pos - 1));
							}
						}
						else if (left == "event") {
							++layer;
							int time = ToInt(TrimAll(right));
							nowProgram = EVENT;
							nowUnit->eventProgram.push_back({ time,std::vector<Precompiled::Unit::Code>() });
						}
						else if (left == "method") {
							++layer;
							nowProgram = CUSTUM;
							custumModule = TrimAll(right);
							if (comp.custumProgramTable.count(custumModule)) {
								custumModuleHandle = comp.custumProgramTable[custumModule];
							}
							else {
								custumModuleHandle = 0;
								while (handle2module.count(custumModuleHandle)) {
									custumModuleHandle++;
								}
								comp.custumProgramTable[custumModule] = custumModuleHandle;
								handle2module[custumModuleHandle] = custumModule;
							}
						}
						else if (left == "once") {
							++layer;
							nowProgram = CANDY_ONCE;
							candyRight = right;
							candyCode.clear();
						}
						else throw runtime_error("未知块 " + left);
					}
					else if (layer >= 2) {	//在程序块内
						string left = ReadLeftWord(line);
						if (left == "end") {
							--layer;
							if (layer == 1) {
								if (nowProgram == CANDY_ONCE) {
									//Var
									string candyVarName = "DDDDDDdmslOnceBlockCandyVar" + to_string(nextVar);
									nowUnit->varTable[candyVarName] = nextVar++;

									//Init
									nowUnit->initProgram.insert(nowUnit->initProgram.begin(), { candyVarName + "=0",-1 });

									//Main
									auto nextLine = nowUnit->mainProgram.insert(nowUnit->mainProgram.begin(), { "if (" + candyRight + ")&&" + candyVarName + "==0" ,-1 });
									for (auto& p : candyCode)
										nextLine = nowUnit->mainProgram.insert(++nextLine, p);
									nextLine = nowUnit->mainProgram.insert(++nextLine, { candyVarName + "=1",-1 });
									nowUnit->mainProgram.insert(++nextLine, { "end",-1 });

								}
							}
						}
						else if (
							left == "if" ||
							left == "while" ||
							left == "for" ||
							left == "times"
							)
							++layer;
						if (nowProgram == INIT) nowUnit->initProgram.push_back({ line,nowLine });
						else if (nowProgram == MAIN) nowUnit->mainProgram.push_back({ line,nowLine });
						else if (nowProgram == CUSTUM) nowUnit->custumProgram[custumModuleHandle].push_back({ line,nowLine });
						else if (nowProgram == EVENT) nowUnit->eventProgram.back().second.push_back({ line,nowLine });
						else if (nowProgram == CANDY_ONCE) candyCode.push_back({ line,nowLine });
						else {
							std::string warning = "PrecomilerWarning:" + left + "\n";
							OutputDebugString(warning.c_str());
						}
					}
				}
				catch (exception& e) {
					log << "编译错误：在行" + to_string(nowLine) + "有" + e.what() << endl;
					success = false;
				}

			}
			if (layer != 0) { log << "编译错误：存在没配对的代码块首尾！" << endl; success = false; }

			if (nowUnit->initProgram.empty()) nowUnit->initProgram.push_back({ "end",nowLine });
			if (nowUnit->mainProgram.empty()) nowUnit->mainProgram.push_back({ "end",nowLine });
			return success;
		}
	}
}
