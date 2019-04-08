#include <string>
#include <iostream>
#include <fstream>
#include "../NThgfm/Conversation.h"
#include "../NThgfm/DMSLUtils.h"
#include <codecvt>

using namespace Dmsl::Utils;
using namespace std;
using namespace ConversationScript;

void compileConversation(
	const std::string& code,
	std::map<std::string, std::vector<ConversationScript::Script::Event>>& events,
	std::vector<std::string>& texts,
	std::string& rightLH) {



	//整理代码
	std::vector<std::string> codes;
	Dmsl::Utils::Split(code, codes, '\n');
	for (auto& p : codes)
		p = DeleteComments(p);

	codes.erase(std::remove_if(codes.begin(), codes.end(), [](std::string& p) {
		return p.empty();
	}),
		codes.end()
		);


	std::vector<ConversationScript::Script::Event>* output = nullptr;
	for (auto p : codes) {
		if (p[p.size() - 1] == '\r') p = p.substr(0, p.size() - 1);
		if (p[0] == '-') {
			output = &events[p.substr(1, p.length() - 1)];
		}
		else {
			ConversationScript::Script::Event e;

			std::string cmd = TrimAll(ReadLeftWord(p));

			auto pos = p.find(' ');
			std::string arg;
			if (pos != std::string::npos) arg = p.substr(pos + 1, p.size() - pos - 1);

			bool pushThis = true;

			if (cmd == "show") e.cmd = Script::Event::Cmd::Show;
			else if (cmd == "hide") e.cmd = Script::Event::Cmd::Hide;
			else if (cmd == "act") e.cmd = Script::Event::Cmd::Act;
			else if (cmd == "una") e.cmd = Script::Event::Cmd::Una;
			else if (cmd == "setlnum") e.cmd = Script::Event::Cmd::SetLNum;
			else if (cmd == "setrnum") e.cmd = Script::Event::Cmd::SetRNum;
			else if (cmd == "loadr") {
				pushThis = false;
				rightLH = arg;
			}
			else if (cmd == "color") {
				pushThis = false;
				std::vector<std::string> colors;
				Split(arg, colors, ',');
				e.cmd = Script::Event::Cmd::ColorR;
				e.arg = ToDouble(TrimAll(colors[0]));
				output->push_back(e);
				e.cmd = Script::Event::Cmd::ColorG;
				e.arg = ToDouble(TrimAll(colors[1]));
				output->push_back(e);
				e.cmd = Script::Event::Cmd::ColorB;
				e.arg = ToDouble(TrimAll(colors[2]));
				output->push_back(e);
			}
			else if (cmd == "program")  e.cmd = Script::Event::Cmd::Program;
			else if (cmd == "print") {
				e.cmd = Script::Event::Cmd::Print;
				auto pos1 = arg.find('\"');
				auto pos2 = arg.find('\"', pos1 + 1);
				texts.push_back(arg.substr(pos1 + 1, pos2 - pos1 - 1));
				arg = std::to_string(texts.size() - 1);
			}
			else if (cmd == "showname")e.cmd = Script::Event::Cmd::Showname;
			else if (cmd == "bossbgm")e.cmd = Script::Event::Cmd::BossBGM;
			else if (cmd == "stopbgm") e.cmd = Script::Event::Cmd::StopBGM;
			else if (cmd == "waitclick")e.cmd = Script::Event::Cmd::WaitClick;
			else if (cmd == "waittime")e.cmd = Script::Event::Cmd::WaitTime;
			else if (cmd == "br")e.cmd = Script::Event::Cmd::Br;
			else if (cmd == "cls") e.cmd = Script::Event::Cmd::Cls;
			else if (cmd.empty()) {
				pushThis = false;
			}
			else throw std::runtime_error("错误的指令：" + cmd);

			if (pushThis) {
				arg = TrimAll(arg);
				if (arg == "l") arg = std::to_string(Left);
				else if (arg == "r") arg = std::to_string(Right);
				if (!arg.empty())
					e.arg = ToDouble(arg);
				output->push_back(e);
			}
		}
	}
}

void WriteString(const std::string s, ofstream& out) {
	uint16_t size = (uint16_t)s.length();
	out.write((char*)&size, sizeof(size));
	for (auto c : s) {
		out.put(c ^ 0xF0);
	}
}

void WriteWString(const std::wstring& s, ofstream& out) {
	uint16_t size = (uint16_t)s.length() * sizeof(wchar_t);
	out.write((char*)&size, sizeof(size));

	const char* chr = (const char*)s.c_str();
	
	for (uint16_t i = 0; i < size; ++i) {
		out.put(chr[i] ^ 0xF0);
	}
}

std::wstring Str2WStr(const std::string& str) {
	static const char* GBK_LOCALE_NAME = ".936";
	static std::wstring_convert<std::codecvt_byname<wchar_t, char, mbstate_t>> cv1(new std::codecvt_byname<wchar_t, char, mbstate_t>(GBK_LOCALE_NAME));
	return cv1.from_bytes(str);
}

void WriteOutput(std::ofstream& out, const std::string& in) {
	std::map<std::string, std::vector<ConversationScript::Script::Event>> events;
	std::vector<std::string> texts;
	std::string rightLH;
	compileConversation(in, events, texts, rightLH);

	WriteString(rightLH, out);

	uint16_t size = (uint16_t)texts.size();
	out.write((char*)&size, sizeof(size));
	for (auto& p : texts)
		WriteWString(Str2WStr(p), out);

	size = (uint16_t)events.size();
	out.write((char*)&size, sizeof(size));
	for (auto& p : events) {
		WriteString(p.first, out);

		size = (uint16_t)p.second.size();
		out.write((char*)&size, sizeof(size));

		out.write((char*)&p.second[0], sizeof(p.second[0]) * size);
	}
	out.close();
}

int main(int argc, char** argv) {
	cout << "Conversation Compiler Version:" << __DATE__ << __TIME__ << endl;
	if (argc >= 3) {
		std::ifstream in(argv[1], ios::binary);
		in.seekg(0, ios::end);
		auto size = in.tellg();
		in.seekg(0, ios::beg);
		char* str = new char[(unsigned int)size];
		in.read(str, size);
		std::string s(str);
		delete[] str;
		
		try {
			WriteOutput(ofstream(argv[2], ios::binary), s);
		}
		catch(std::exception& e){
			cout << "Error:" << e.what() << " In " << argv[1] << endl;
			return -1;
		}

		cout << argv[1] << " >> " << argv[2] << endl;
	}
	return 0;
}