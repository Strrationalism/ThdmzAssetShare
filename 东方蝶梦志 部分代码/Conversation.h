#pragma once
#include <string>
#include <map>
#include <queue>
namespace ConversationScript {
	const double Left = 0.0f;
	const double Right = 1.0f;

	class Script {
	public:
		struct Event {
			double arg;
			enum class Cmd {
				Show,
				Hide,
				Act,
				Una,
				SetLNum,
				SetRNum,
				ColorR,
				ColorG,
				ColorB,
				Program,
				Print,
				WaitClick,
				WaitTime,
				Br,
				Cls,
				Showname,
				BossBGM,
				StopBGM
			}cmd;
		};
	private:
		std::string m_rightLHName;
		std::vector<std::wstring> m_texts;
		std::vector<Event> m_events;
	public:

		void LoadScript(const std::string& name);	//加载脚本

		inline const std::string& GetRightLHName()
		{
			return m_rightLHName;
		}	//获取右立绘文件名

		inline const std::wstring& GetText(int textNum) {	//根据文字编号获取文字
			return m_texts[textNum];
		}


		void GetCommand(std::queue<Event>&);	//获取指令队列
	};
}
