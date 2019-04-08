#include "Task.h"
#include "../NThgfm/DMSLUtils.h"
#include <fstream>
#include <map>
#include <cassert>

std::vector<Task> ParseMakefile(std::istream & make)
{
	if (!make.good()) abort();
	std::vector<Task> tasks;
	std::map<std::string, Task> templateTasks;
	Task* thisTask = nullptr;
	while (1) {
		std::string line;
		make >> line;
		printf("%s\n", line.c_str());
		Dmsl::Utils::DeleteComments(line);
		Dmsl::Utils::TrimAll(line);
		if (line.empty()) {
			break;
		}
		else if (line[0] == '@') {
			thisTask = &templateTasks[line.substr(1, line.length() - 1)];
		}
		else if(line[0] == '!') {
			tasks.push_back(templateTasks[line.substr(1, line.length() - 1)]);
			thisTask = &tasks.back();
		}
		else if (line == "End") {
			break;
		}
		else {
			if (thisTask == nullptr) {
				printf_s("ERROR:thisTask == nullptr");
				abort();
			}
			std::vector<std::string> opr;
			Dmsl::Utils::Split(line, opr, ':');
			if (opr[0] == "Output") {
				thisTask->output = opr[1];
			}
			else if (opr[0] == "Include") {
				std::ifstream in(opr[1].c_str());
				
				while (!in.eof()) {
					std::string lineInDmsl;
					std::getline(in, lineInDmsl);
					thisTask->dmslCode += lineInDmsl;
					thisTask->dmslCode += '\n';
				}
			}
		}
	}
	return tasks;
}
