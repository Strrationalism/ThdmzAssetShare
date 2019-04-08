#include "NuzPrecompiled.h"
#include "Globals.h"
#include "DataActor.h"
void Basic::DataActor::Update() {
	for (auto& p : m_datas) {

		//安全检查
		if (IsBadWritePtr(p.data, sizeof(*p.data)))
			throw std::exception("DataActor:Bad ptr write.");

		++p.timer;
		switch (p.slowMotion) {
		case 0:
			*p.data = Mix(p.start, p.end, double(p.timer) / p.len);
			break;
		case 1:
			*p.data = Mix(p.start, p.end, SlowMotion(double(p.timer) / p.len));
			break;
		case 2:
			*p.data = Mix(p.start, p.end, SlowMotion2(double(p.timer) / p.len));
			break;
		case 3:
			*p.data = Mix(p.start, p.end, SlowMotion3(double(p.timer) / p.len));
			break;
		case 4:
			*p.data = Mix(p.start, p.end, SlowMotion4(double(p.timer) / p.len));
			break;
		case 5:
			*p.data = Mix(p.start, p.end, SlowMotion5(double(p.timer) / p.len));
			break;
		};
	}
	auto nend = std::remove_if(m_datas.begin(), m_datas.end(), [](const Data& i) {
		return i.timer >= i.len;
	});
	m_datas.erase(nend, m_datas.end());

	//性能优化
	/*auto size = m_datas.size();
	auto lastNum = size-1;
	if (size == 0) return;
	for (unsigned i = 0; i <= lastNum; ++i) {
	auto& p = m_datas[i];
	if (p.timer <= p.len) {
	if (p.slowMotion)
	*p.data = Mix(p.start, p.end, SlowMotion(double(p.timer) / p.len));
	else
	*p.data = Mix(p.start, p.end, double(p.timer) / p.len);
	++p.timer;
	}
	else {
	if (lastNum != i) {
	std::swap(p, m_datas[lastNum]);
	--i;
	}
	lastNum--;
	}
	}
	m_datas.resize(lastNum + 1);*/
}
