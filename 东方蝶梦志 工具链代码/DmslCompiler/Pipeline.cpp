#include "Pipeline.h"
#include "Input.h"
#include "Output.h"

static void optimizeByteCode(std::vector<uint8_t>& byteCode) {
	static bool(*optimizer[5])(std::vector<DmslOptimizer::Input::OprInput>&) = {
		nullptr
	};

	auto inc = DmslOptimizer::Input::GetInput(byteCode);
	DmslOptimizer::Input::RelinkInputCode(inc);

	bool keep = true;
	while (keep) {
		keep = false;
		/*for (auto p : optimizer)
			keep = keep || p(inc);*/
	}

	auto size1 = byteCode.size();
	byteCode.clear();
	//重新生成代码
	byteCode = DmslOptimizer::Output::Output(inc);

	if (size1 != byteCode.size()) abort();
}

void DmslOptimizer::Optimize(Dmsl::Compiler::Compiled & program)
{
	for (auto& p : program.units) {
		optimizeByteCode(p.second.initProgram);
		optimizeByteCode(p.second.mainProgram);
		for (auto& q : p.second.custumProgram)
			optimizeByteCode(q.second);
	}
}
