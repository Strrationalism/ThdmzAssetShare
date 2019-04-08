#include "NuzPrecompiled.h"
#include "CSSReader.h"
#include "DMSLUtils.h"
#include "Asset.h"


using namespace std;
using namespace Dmsl::Utils;

void Basic::CSSReader::LoadCSS(const std::string & f)
{
	Basic::Asset ass(f + ".css");
	
	std::string code = ass;

	std::string codeTrimed;

	bool com = false;	//当前是否为注释
	for (auto p : code) {
		if (p == '/') com = !com;
		if (isgraph(static_cast<unsigned char>(p)) && !com) codeTrimed += p;
	}

	while (!codeTrimed.empty()) {
		auto leftPos = codeTrimed.find('{'), rightPos = codeTrimed.find('}');

		std::string labelName = codeTrimed.substr(0, leftPos);
		std::string labelAttrs = codeTrimed.substr(leftPos + 1, rightPos - leftPos - 1);
		codeTrimed = codeTrimed.substr(rightPos + 1, codeTrimed.length() - rightPos - 1);

		auto& p = (m_labels[labelName] = make_shared<CSSLabel>());
		
		std::vector<std::string> attrStrings;
		Dmsl::Utils::Split(labelAttrs, attrStrings, ';');
		attrStrings.pop_back();
		
		for (auto& attr : attrStrings) {
			auto pos = attr.find(':');
			p->m_attrs[attr.substr(0, pos)] = attr.substr(pos + 1, attr.length() - pos - 1);
		}
	}
}
