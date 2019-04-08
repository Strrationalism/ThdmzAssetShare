#pragma once
#include <string>
#include <vector>

namespace Dmsl {

	namespace Utils {
		//读取最右单词
		std::string ReadLeftWord(const std::string&);

		//读取除最右单词以外的单词
		std::string ReadRightLine(const std::string&);

		//去除右空格
		std::string TrimRight(const std::string&);

		//去除全部空格
		std::string TrimAll(const std::string&);

		//去除注释
		std::string DeleteComments(const std::string&);

		//转换到double
		double ToDouble(const std::string&);

		//转换到int
		int ToInt(const std::string&);

		//切分字符串
		void Split(std::string str, std::vector<std::string>& out,char ch);

		//检查变量/常量名合法性
		void CheckName(const std::string& s);

		//读取最右边的一个变量或运算符，并切割
		std::string ReadToken(std::string& s);
	}
}
