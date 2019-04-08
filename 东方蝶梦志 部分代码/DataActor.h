#pragma once
#include <algorithm>
#include <vector>
#include <utility>

namespace Basic {
	class DataActor
	{
	private:
		struct Data{
			double* data;
			double start, end;
			unsigned timer;
			unsigned len;
			int slowMotion;	//»º¶¯
		};
		std::vector<Data> m_datas;

		//std::mutex m_lock;
	public:
		inline void Add(double& data, double end, unsigned frames,int slowMotion) {
			//std::lock_guard<std::mutex> g(m_lock);
			m_datas.push_back({ &data,data,end,0,frames ,slowMotion });
		}
		void Update();

		inline void Clear() {
			m_datas.clear();
		}
	};

}