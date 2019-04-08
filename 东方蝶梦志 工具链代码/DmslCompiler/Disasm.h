#pragma once
#include <fstream>
#include "../NThgfm/DmslCompiler.h"

void WriteDisasm(Dmsl::Compiler::Compiled & program, std::ofstream & out);