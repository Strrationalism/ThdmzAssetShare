#pragma once

#include <functional>

namespace Property
{
	using std::function;
	using std::forward;
	using std::move;

	template<typename T>
	class Auto
	{
	public:
		Auto(T&& value...);

		T& GetVal();
		const T& GetConst() const;
		operator T&();
		operator const T&() const;

		template<typename Arg>
		void SetValue(Arg&&);

		template<typename Arg>
		const T& operator = (Arg&&);
	private:
		T value_;

		Auto(const Auto&) = delete;
		Auto(Auto&&) = delete;
		const Auto& operator=(Auto&&) = delete;
		const Auto& operator=(const Auto&) = delete;
	};

	template<typename T>
	class Get
	{
	public:
		explicit Get(T& boundObject);
		Get(const function<T&()>& getter);

		T& GetVal();
		const T& GetConst() const;
		operator T&();
		operator const T&() const;
	private:
		function<T&()> getter_;

		Get(const Get&) = delete;
		Get(Get&&) = delete;
		const Get& operator=(Get&&) = delete;
		const Get& operator=(const Get&) = delete;
	};

	template<typename T>
	class Set
	{
	public:
		explicit Set(T& boundObject);
		Set(const function<void(const T&)>& setter);
		Set(const function<void(const T&)>& setter, const function<void(T&&)>& setterForMove);

		void SetValue(T&&);
		void SetValue(const T&);
		const T& operator=(T&&);
		const T& operator=(const T&);

		const Set& operator=(Set&&) = delete;
		const Set& operator=(const Set&) = delete;
	private:
		function<void(const T&)> copySetter_;
		function<void(T&&)> moveSetter_;

		Set(Set&&) = delete;
		Set(const Set&) = delete;
	};

	template<typename T>
	class GetSet :
		public Get<T>,
		public Set<T>
	{
	public:
		explicit GetSet(T& boundObject);
		GetSet(const function<T&()>& getter, const function<void(const T&)>& setter);
		GetSet(const function<T&()>& getter, const function<void(const T&)>& setter, const function<void(T&&)>& setterForMove);

		using Set<T>::operator =;
	};


	/* Auto */

	template<typename T>
	inline Auto<T>::Auto(T && value...) :
		value_(std::move(value))
	{
	}

	template<typename T>
	inline Auto<T>::operator T&()
	{
		return GetVal();
	}

	template<typename T>
	inline Auto<T>::operator const T&() const
	{
		return GetConst();
	}

	template<typename T>
	inline T & Auto<T>::GetVal()
	{
		return value_;
	}

	template<typename T>
	inline const T & Auto<T>::GetConst() const
	{
		return value_;
	}


	template<typename T>
	template<typename Arg>
	inline const T& Auto<T>::operator = (Arg&& value)
	{
		SetValue(forward<Arg>(value));
		return GetVal();
	}

	template<typename T>
	template<typename Arg>
	inline void Auto<T>::SetValue(Arg && value)
	{
		value_ = forward<Arg>(value);
	}

	/* Get */

	template<typename T>
	inline Get<T>::Get(T & boundObject)
		:Get([&boundObject]() -> T& { return boundObject; })
	{
	}

	template<typename T>
	inline Get<T>::Get(const function<T&()>& getter) :
		getter_(getter)
	{
	}

	template<typename T>
	inline T & Get<T>::GetVal()
	{
		return getter_();
	}

	template<typename T>
	inline const T & Get<T>::GetConst() const
	{
		return getter_();
	}
	template<typename T>
	inline Get<T>::operator T&()
	{
		return GetVal();
	}

	template<typename T>
	inline Get<T>::operator const T&() const
	{
		return GetConst();
	}

	/* Set */

	template<typename T>
	inline Set<T>::Set(T & boundObject) :
		Set([&boundObject](const T& obj)->void {boundObject = obj; }, [&boundObject](T&& obj)->void {boundObject = move(obj); })
	{
	}

	template<typename T>
	inline Set<T>::Set(const function<void(const T&)>& setter) :
		Set(setter, [this](T&& value)->void {copySetter_(value); })
	{
	}

	template<typename T>
	inline Set<T>::Set(const function<void(const T&)>& setter, const function<void(T&&)>& setterForMove) :
		copySetter_(setter), moveSetter_(setterForMove)
	{
	}

	template<typename T>
	inline void Set<T>::SetValue(T && value)
	{
		moveSetter_(move(value));
	}

	template<typename T>
	inline void Set<T>::SetValue(const T & value)
	{
		copySetter_(value);
	}

	template<typename T>
	inline const T & Set<T>::operator=(T && value)
	{
		SetValue(move(value));
		return value;
	}

	template<typename T>
	inline const T & Set<T>::operator=(const T & value)
	{
		SetValue(value);
		return value;
	}

	/* GetSet */
	template<typename T>
	inline GetSet<T>::GetSet(T & boundObject) :
		Get(boundObject), Set(boundObject)
	{
	}

	template<typename T>
	inline GetSet<T>::GetSet(const function<T&()>& getter, const function<void(const T&)>& setter) :
		Get(getter),
		Set(setter)
	{
	}

	template<typename T>
	inline GetSet<T>::GetSet(const function<T&()>& getter, const function<void(const T&)>& setter, const function<void(T&&)>& setterForMove) :
		Get(getter),
		Set(setter, setterForMove)
	{
	}

}
