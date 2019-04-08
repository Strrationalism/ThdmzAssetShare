#pragma once
#include "Asset.h"
#include <string>
#include <iostream>
#include "Log.h"
#include <codecvt>
#include <cmath>
#include <memory>
#include "CtxFont.h"

#define LPS 100

//#define OUTPUT_COMPILED_BIN	//输出编译后的文件到asset_runtime_compiled目录

//#define USE_COMPILED_BIN	//使用编译出的二进制数据

#define CHECK (x) if(!(x)) Crash(L"Assert!");	//断言

#define AT(x,i) x.at(i)		//从容器访问数据

//逻辑屏幕大小
const int WIDTH = 1280;
const int HEIGHT = 720;
const int MAX_LINE_IN_SCREEN = 1469;

const unsigned VERSION = 0xFF000000;

/* 版本格式：
 * 第一字节为FF则为开发中的版本
 * 第二字节为游戏的编号
 * 第三和第四字节为游戏发布的版本号。
 */


//弹出错误并退出
void Crash(const std::wstring&);

inline double Clamp(double smallVar, double bigVar, double x) {
	x = x > smallVar ? x : smallVar;
	x = x < bigVar ? x : bigVar;
	return x;
}

constexpr double Mix(double smallVar, double bigVar, double x) {	//计算插值
	return x * (bigVar - smallVar) + smallVar;
}

inline double SlowMotion(double p) {	//对一个0~1的浮点数进行缓动处理
	return -p * p + 2 * p;
}

inline double SlowMotion2(double x) {	//圆形缓动
	return sqrt(1 - (x - 1)*(x - 1));
}

inline double SlowMotion3(double x) {
	return 1 - SlowMotion2(1 - x);	//反向圆形缓动
}

inline double SlowMotion4(double x) {
	return 1 - SlowMotion(1 - x);
}

inline double SlowMotion5(double x) {
	return sin(M_PI / 2 * x);
}

Basic::Video::Renderer2D::CtxFont & GetCtxFont1();

inline double Rand() {
	return double(rand()) / RAND_MAX;
}

std::wstring Str2WStr(const std::string& s);


//字符串IO
inline void WriteStringToStream(const std::string& str, std::ostream& out) {
	uint32_t size = (uint32_t)str.size();
	out.write((char*)&size, sizeof(size));

	std::vector<char> c;
	for (auto ch : str) {
		c.push_back(~ch);
	}
	out.write(&c[0], size);
}

inline void LoadStringFromStream(std::string& str, std::istream& in) {
	uint32_t size;
	in.read((char*)&size, sizeof(size));
	str.resize(size);
	in.read(&str[0], size);
	for (auto& c : str)
		c = ~c;
}