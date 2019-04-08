#pragma once

#include <set>
#include <stack>
#include <memory>

template<typename T,size_t growth>
class MemoryPool
{
private:
	std::set<T*> heads_;
	std::stack<T*> unusedObjects_;
	std::allocator<T> alloc_;

	void grow() 
	{
		auto head = alloc_.allocate(growth);
		for (int i = 0; i < growth; ++i)
		{
			unusedObjects_.push(head + i);
		}
		heads_.insert(head);
	}
public:
	MemoryPool()
	{
		grow();
	}

	~MemoryPool()
	{
		for (auto i : heads_)
			alloc_.deallocate(i,growth);
	}

	void* Get()
	{
		if (unusedObjects_.empty()) grow();
		auto obj = unusedObjects_.top();
		unusedObjects_.pop();
		return static_cast<void*>(obj);
	}

	void Return(void* obj)
	{
		unusedObjects_.push(static_cast<T*>(obj));
	}

	/*T* New()
	{
		auto unused = static_cast<T*>(GetUnusedObject());
		alloc_.construct(unused);
		return unused;
	}

	void Delete(T* obj)
	{
		ReturnUnusedObject(static_cast<void*>(alloc_.destroy(obj)));
	}*/
};
