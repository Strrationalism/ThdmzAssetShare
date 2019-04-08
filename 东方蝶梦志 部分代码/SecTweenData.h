#pragma once
#include "Globals.h"
#include <optional>

namespace Basic
{
	template<typename T>
	class SecTweenData
	{
	public:
		SecTweenData();
		SecTweenData(const T&);

		void Update(float deltaTime);
		void Tween(T target, float time, int funcID = 0);
		
		operator T() const;
		const SecTweenData<T>& operator = (const T&);
	private:
		T m_value;
		std::optional<T> m_target;
		T m_begin;
		float m_passedTime;
		float m_allTime;
		int m_funcID;
	};

	template<typename T>
	inline SecTweenData<T>::SecTweenData()
	{
		m_value = 0;
	}

	template<typename T>
	inline SecTweenData<T>::SecTweenData(const T & v)
	{
		m_value = v;
	}

	template<typename T>
	inline void SecTweenData<T>::Update(float deltaTime)
	{
		if (m_target.has_value())
		{
			m_passedTime += deltaTime;
			if (m_passedTime >= m_allTime)
			{
				m_value = m_target.value();
				m_target.reset();
			}
			else
			{
				float per = m_passedTime / m_allTime;
				switch (m_funcID)
				{
				case 1:
					per = SlowMotion(per); break;
				case 2:
					per = SlowMotion2(per); break;
				case 3:
					per = SlowMotion3(per); break;
				case 4:
					per = SlowMotion4(per); break;
				case 5:
					per = SlowMotion5(per); break;
				default:
					break;
				}

				m_value = m_begin + (m_target.value() - m_begin)*per;
			}
		}
	}
	template<typename T>
	inline void SecTweenData<T>::Tween(T target, float time, int funcID)
	{
		m_begin = m_value;
		m_target = target;
		m_passedTime = 0;
		m_allTime = time;
		m_funcID = funcID;
	}
	template<typename T>
	inline SecTweenData<T>::operator T() const
	{
		return m_value;
	}
	template<typename T>
	inline const SecTweenData<T>& SecTweenData<T>::operator=(const T & v)
	{
		m_target.reset();
		m_value = v;
		return *this;
	}
}