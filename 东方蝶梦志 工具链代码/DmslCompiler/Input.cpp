#include "Input.h"
#include <queue>

enum ArgType { VAL, ADDRESS,DMOVFS };

std::vector<DmslOptimizer::Input::OprInput> DmslOptimizer::Input::GetInput(const std::vector<uint8_t>& code)
{
	std::vector<DmslOptimizer::Input::OprInput> ret;	//要返回的反汇编表
	std::queue<uintptr_t> linkingAddress;	//待链接的地址
	const static std::vector<ArgType> oprArgList[0x1F] = {
		{DMOVFS},	//DMOVFS
		{ADDRESS},	//MOV
		{ADDRESS,ADDRESS},	//CFUNC
		{ADDRESS,ADDRESS},	//CMET
		{VAL},	//PUSH
		{ADDRESS},	//IF
		{ADDRESS},	//JMP
		{},	//END
		{ADDRESS},	//DUNPA
		{ADDRESS},	//DUNPU
		{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},
		{ADDRESS},
		{ADDRESS},
		{ADDRESS},
		{},	//PAUSE
		{ ADDRESS },
		{ ADDRESS }
	};

	for (uintptr_t i = 0; i < code.size();) {
		DmslOptimizer::Input::OprInput output;
		output.oprAddress = i;

		output.opr = (Dmsl::VirtualMachine::ElfCode)code[i++];

		if (!oprArgList[(int)output.opr].empty()) {
			//收集参数
			if (oprArgList[(int)output.opr][0] == DMOVFS) {
				//if (i + 1 >= code.size()) continue;
				//收集DMOVFS参数
				OprInput::Arg arg;

				arg.type = OprInput::Arg::T_ADDRESS;
				arg.address = (uintptr_t&)code[i];
				i += sizeof(uintptr_t);
				output.args.push_back(arg);

				auto num = arg.address = (uintptr_t&)code[i];
				i += sizeof(uintptr_t);
				output.args.push_back(arg);

				arg.type = OprInput::Arg::T_VAL;
				for (uint16_t j = 0; j < num;++j) {
					arg.val = (double&)code[i];
					i += sizeof(double);
					output.args.push_back(arg);
				}
			}
			else {
				//收集其它参数
				
				for (auto type : oprArgList[int(output.opr)]) {
					OprInput::Arg arg;
					if (type == ADDRESS) {
						arg.type = OprInput::Arg::T_ADDRESS;
						arg.address = *(uintptr_t*)&code[i];
						i += sizeof(uintptr_t);
					}
					else if (type == VAL) {
						arg.type = OprInput::Arg::T_VAL;
						arg.val = *(double*)&(code[i]);
						i += sizeof(double);
					}
					output.args.push_back(arg);
				}
			}
		}
		ret.push_back(output);
	}

	
	return ret;
}

void DmslOptimizer::Input::RelinkInputCode(std::vector<OprInput>& code)
{

	struct LinkPoint {
		uintptr_t* codeAddress;
		uintptr_t jumpAddress;

		inline bool operator < (const LinkPoint& t) const {
			return jumpAddress < t.jumpAddress;
		}
	};

	std::vector<LinkPoint> linkPoints;
	for (auto& p : code) {
		if (p.opr == Dmsl::VirtualMachine::ElfCode::IF || p.opr == Dmsl::VirtualMachine::ElfCode::JMP) {
			linkPoints.push_back({
				&p.args[0].address,
				p.args[0].address
			});
		}
	}

	std::sort(linkPoints.begin(), linkPoints.end());

	auto p = linkPoints.begin();
	for (uintptr_t i = 0; i < code.size(); ++i) {
		if (p == linkPoints.end()) break;
		while (code[i].oprAddress == p->jumpAddress) {
			*p->codeAddress = i;
			++p;
			if (p == linkPoints.end()) break;
		}
	}

	if (p != linkPoints.end()) abort();
}
