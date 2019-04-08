#include "BinWriter.h"
#include <fstream>
//From Globals.h
inline void WriteStringToStream(const std::string& str, std::ostream& out) {
	uint32_t size = (uint32_t)str.size();
	out.write((char*)&size, sizeof(size));

	std::vector<char> c;
	for (auto ch : str) {
		c.push_back(~ch);
	}
	out.write(&c[0], size);
}

inline void LoadStringFromStream(std::string& str, std::istream& in) {
	uint32_t size;
	in.read((char*)&size, sizeof(size));
	str.resize(size);
	in.read(&str[0], size);
	for (auto& c : str)
		c = ~c;
}

inline void WriteProgramBinToStream(const std::vector<uint8_t>& s, std::ostream& out) {
	uint32_t size = (uint32_t)s.size();
	out.write((char*)&size, sizeof(size));

	out.write((char*)&s[0], size * sizeof(uint8_t));
}

void WriteBin(Dmsl::Compiler::Compiled & program, std::ofstream & out)
{
	if (out.fail())
		abort();
	//这里写出二进制文件

	out.write((char*)&program.linkMetSize, sizeof(program.linkMetSize));
	out.write((char*)&program.linkFuncSize, sizeof(program.linkMetSize));

	uint16_t size = (uint16_t)program.custumProgramHandles.size();
	out.write((char*)&size, sizeof(size));
	for (auto& p : program.custumProgramHandles) {
		out.write((char*)&p.first, sizeof(p.first));
		out.write((char*)&p.second, sizeof(p.second));
	}

	size = (uint16_t)program.linkTable.size();
	out.write((char*)&size, sizeof(size));
	for (auto& p : program.linkTable) {
		out.write((char*)&p.first, sizeof(p.first));
		out.write((char*)&p.second, sizeof(p.second));
	}


	//out.write((char*)&program.uniformSize, sizeof(program.uniformSize));

	size = (uint16_t)program.units.size();
	out.write((char*)&size, sizeof(size));
	for (auto& pUnit : program.units) {
		out.write((char*)&pUnit.first, sizeof(pUnit.first));
		out.write((char*)&pUnit.second.outVaryingSize, sizeof(uint32_t));
		out.write((char*)&pUnit.second.memSize, sizeof(pUnit.second.memSize));

		WriteProgramBinToStream(pUnit.second.initProgram, out);
		WriteProgramBinToStream(pUnit.second.mainProgram, out);

		size = (uint16_t)pUnit.second.custumProgram.size();
		out.write((char*)&size, sizeof(size));
		for (auto& p : pUnit.second.custumProgram) {
			out.write((char*)&p.first, sizeof(p.first));
			WriteProgramBinToStream(p.second, out);
		}

		size = (uint16_t)pUnit.second.eventProgram.size();
		out.write((char*)&size, sizeof(size));
		for (auto& p : pUnit.second.eventProgram) {
			out.write((char*)&p.first, sizeof(p.first));
			WriteProgramBinToStream(p.second, out);
		}
	}

	out.close();
}
