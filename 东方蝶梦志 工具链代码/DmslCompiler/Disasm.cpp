#include "Disasm.h"
#include "Input.h"
#include <string>
using namespace std;

static const string oprName[] = {
	"DMOVFS",
	"MOV",
	"CFUNC",
	"CMET",
	"PUSH",
	"IF",
	"JMP",
	"END",
	"DUNPA",
	"DUNPU",
	"NEGA",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"MOD",
	"BIG",
	"BIGE",
	"SML",
	"SMLE",
	"EQ",
	"NE",
	"AND",
	"OR",
	"NOT",
	"VARYING_MOV",
	"VARYING_READ_IN",
	"VARYING_UNP",
	"PAUSE",
	"DATAACTOR_ADD_ATTR",
	"DATAACTOR_ADD_VARYING"
};

static void writeProgramText(const std::vector<uint8_t>& byteCode, std::ostream & out) {
	out.setf(ios::fixed);
	auto inp = DmslOptimizer::Input::GetInput(byteCode);
	for (auto& p : inp) {
		out << hex << p.oprAddress << "\t"<<(unsigned)p.opr<<"\t"<<oprName[(int)p.opr];
		for (auto& arg : p.args) {
			if (arg.type == DmslOptimizer::Input::OprInput::Arg::T_ADDRESS) {
				out <<"\t" <<arg.address << "(INT)";
			}
			else if (arg.type == DmslOptimizer::Input::OprInput::Arg::T_VAL) {
				out << "\t" <<arg.val;
			}
		}
		out << endl;
	}
}

void WriteDisasm(Dmsl::Compiler::Compiled & program, std::ofstream & out)
{
	out << "-Link Size:" << dec << program.linkFuncSize << "," << program.linkMetSize << endl;
	//out << "-Uniform Size:" << dec << program.uniformSize << endl;
	out << endl;

	out << "-Custom Method Handles:" << endl;
	out << hex;
	for (auto& p : program.custumProgramHandles)
		out << p.second << "\t" << p.first << endl;
	out << endl;

	out << "-LinkTable:" << endl;
	for (auto& p : program.linkTable)
		out << p.first << "\t" << p.second << endl;
	out << endl;
	
	out << "-Text:" << endl;
	out << endl << endl << endl;
	for (auto& p : program.units) {
		out << "--Unit " << dec << p.first << "\tMemSize:" << p.second.memSize <<"\tVaryingSize:"<<p.second.outVaryingSize<< "\tCustom:"<< p.second.custumProgram.size()<< endl;
		
		out << "---Init:" << endl;
		writeProgramText(p.second.initProgram,out);
		out << endl;
		out << "---Main:" << endl;
		writeProgramText(p.second.mainProgram,out);
		out << endl;
		
		for (auto& q : p.second.custumProgram) {
			out << "---Interface " << hex << q.first << endl;
			writeProgramText(q.second,out);
			out << endl;
		}

		for (auto& q : p.second.eventProgram) {
			out << "---Event " << dec << q.first << endl;
			writeProgramText(q.second, out);
			out << endl;
		}
		out << endl << endl<<endl;
	}

}
