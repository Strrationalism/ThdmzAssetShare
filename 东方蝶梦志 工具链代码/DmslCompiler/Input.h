#pragma once
#include "../NThgfm/DMSLElf.h"
#include <vector>

namespace DmslOptimizer {
	namespace Input {
		struct OprInput {
			Dmsl::VirtualMachine::ElfCode opr;	//操作码

			struct Arg {
				union  { double val; uintptr_t address; };
				enum{T_VAL,T_ADDRESS}type;
			};
			std::vector<Arg> args;	//参数列表
			uintptr_t oprAddress;	//字节码地址
		};


		std::vector<OprInput> GetInput(const std::vector<uint8_t>& code);

		void RelinkInputCode(std::vector<OprInput>&);	//将代码中的跳转更改为OprInput下标的跳转，仅用于优化
	}
}