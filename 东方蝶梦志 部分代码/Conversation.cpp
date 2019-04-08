#include "NuzPrecompiled.h"
#include "Conversation.h"
#include "DmslUtils.h"
#include "Asset.h"
#include "Globals.h"


using namespace Dmsl::Utils;
using namespace ConversationScript;

static inline void loadString(const char*& program, std::string& out) {
	uint16_t size = *(uint16_t*)program;
	program += sizeof(size);
	out.resize(size);
	for (uint16_t i = 0; i < size; ++i) {
		out[i] = *program ^ 0xF0;
		program++;
	}
}

static inline void loadWString(const char*& program, std::wstring& out) {
	uint16_t size = *(uint16_t*)program;
	program += sizeof(size);

	char str[256] = { '\0' };
	uint16_t i;
	for (i = 0; i < size; ++i) {
		str[i] = *program ^ 0xF0;
		program++;
	}

	for (uint16_t p = 0; p < sizeof(wchar_t); ++p)
		str[p + i] = '\0';
	out = (wchar_t*)(str);
}



void ConversationScript::Script::LoadScript(const std::string & name)
{
	m_events.clear();
	m_texts.clear();
	m_rightLHName.clear();

	Basic::Asset code("Elf/Conversations/" + name + ".con");

	const char* program = code;

	loadString(program,m_rightLHName);

	uint16_t size = *(uint16_t*)program;
	program += sizeof(size);
	m_texts.resize(size);
	for (uint16_t i = 0; i < size; ++i) {
		std::wstring s;
		loadWString(program, s);
		m_texts[i] = s;
	}

	size = *(uint16_t*)program;
	program += sizeof(size);

	m_events.clear();
	for (uint16_t i = 0; i < size; ++i) {
		std::string first;
		loadString(program, first);

		uint16_t eveSize = *(uint16_t*)program;
		program += sizeof(eveSize);
		
		auto& eves = m_events;
		eves.resize(eveSize);
		memcpy(&eves[0], program, sizeof(eves[0]) * eveSize);
	}
}

void ConversationScript::Script::GetCommand(std::queue<Event>& e)
{
	auto& eve = m_events;
	for (auto& p : eve)
		e.push(p);
}
