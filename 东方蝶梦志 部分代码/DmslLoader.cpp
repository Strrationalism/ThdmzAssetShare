#include "NuzPrecompiled.h"
#include "DmslLoader.h"
#include "Asset.h"
#include "Globals.h"

//#define _USE_ELF_CODE_

Dmsl::DmslVirtualMachine * Dmsl::DmslLoader::compile()
{
#ifdef _USE_ELF_CODE_
	return new Dmsl::DmslVirtualMachine(Basic::Asset(m_code));
#else
	std::stringstream log;
	auto p = new Dmsl::DmslVirtualMachine(m_code, log);
	if (!p->Good()) {
		Crash(L"Dmsl≥Ã–Ú±‡“Î¥ÌŒÛ£∫\n"/*+Str2WStr(m_errorPath)+L"\n" */+ Str2WStr(log.str()));
	}
	return p;
#endif
}

void Dmsl::DmslLoader::Include(const std::string & dmslPath)
{
#ifndef _USE_ELF_CODE_
	AddCode(std::string(Basic::Asset("Dmsl/Header/" + dmslPath + ".dmsl")));
#endif
}

void Dmsl::DmslLoader::IncludeMain(const std::string & dmslPath) {
#ifdef _USE_ELF_CODE_
	m_code = "Elf/" + dmslPath + ".elf";
#else
	AddCode("\n$resetline\n");
	AddCode(std::string(Basic::Asset(dmslPath + ".dmsl")));
#endif
	m_errorPath = dmslPath;
}

std::shared_ptr<Dmsl::DmslVirtualMachine> Dmsl::DmslLoader::CompileShared()
{
	return std::shared_ptr<Dmsl::DmslVirtualMachine>(compile());
}

std::unique_ptr<Dmsl::DmslVirtualMachine> Dmsl::DmslLoader::CompileUnique()
{
	return std::unique_ptr<Dmsl::DmslVirtualMachine>(compile());
}
