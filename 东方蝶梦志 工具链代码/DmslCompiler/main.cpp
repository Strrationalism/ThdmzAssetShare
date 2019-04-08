#define SDL_MAIN_HANDLED
#include <iostream>
#include <string>
#include "BinWriter.h"
#include "../NThgfm/Dmsl.h"
#include <fstream>
#include "Disasm.h"
#include "Task.h"
#include <sstream>
#include "Pipeline.h"

void Crash(const std::wstring& error)
{
	std::wcout << error << std::endl;
	std::terminate();
}


int main(int argc, char** argv) {
	std::cout<<"Dmsl Compiler " << (__DATE__)<< " " << (__TIME__)<<std::endl;

	if (argc <= 1) {
		return 0;
	}
	else if(argc >= 2){
		std::string make(argv[1]);
		bool asmOutput = false;

		if (argc >= 3) {
			std::string make2(argv[2]);
			if (make2 == "-asm") {
				asmOutput = true;
			}
		}

		std::vector<Task> tasks = ParseMakefile(std::ifstream(make.c_str(),std::ios::binary));
		
		for (auto& t : tasks) {
			std::cout << ">>" << t.output << std::endl;
			std::stringstream log;
			Dmsl::DmslVirtualMachine d(t.dmslCode, log);
			if (!d.Good()) {
				std::cout << "Error:" << std::endl << log.str() << std::endl;
			}
			else {
				DmslOptimizer::Optimize(d.GetProgram());
				WriteBin(d.GetProgram(),std::ofstream(t.output.c_str(), std::ios::binary));
				if (asmOutput) {
					std::ofstream outasm((std::string(t.output + ".asm")).c_str(), std::ios::out);
					if (outasm.fail())
						abort();
					WriteDisasm(d.GetProgram(),outasm);
				}
			}
		}


	}
	return 0;
}