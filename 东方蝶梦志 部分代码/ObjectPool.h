#pragma once
#include <stack>
#include <memory>
namespace Basic {
	namespace Utils {
		template<class T>
		class ObjectPool {
		private:
			std::stack<std::unique_ptr<T>> m_objects;
		public:
			T* Alloc() {
				if (m_objects.empty()) {
					return new T;
				}
				else {
					auto p = m_objects.top().release();
					m_objects.pop();
					return p;
				}
			}

			void Free(T* t) {
				m_objects.push(std::unique_ptr<T>(t));
			}
		};
	}
}