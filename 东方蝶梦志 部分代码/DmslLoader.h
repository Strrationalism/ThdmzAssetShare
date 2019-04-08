#pragma once
#include "Dmsl.h"
#include <memory>

namespace Dmsl {
	class DmslLoader
	{
	private:
		std::string m_code, m_errorPath;

		Dmsl::DmslVirtualMachine* compile();
	public:
		void Include(const std::string& header);
		void IncludeMain(const std::string& dmslPath);
		inline void AddCode(const std::string& code)
		{
			m_code += code;
		}
		inline void Clear()
		{
			m_code.clear();
		}

		std::shared_ptr<Dmsl::DmslVirtualMachine> CompileShared();
		std::unique_ptr<Dmsl::DmslVirtualMachine> CompileUnique();
	};

}