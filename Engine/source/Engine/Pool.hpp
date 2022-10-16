#pragma once

#include "precompiled.hpp"

namespace mt
{

	template <typename T>
	class Pool
	{
	public:

		static T* allocate(std::size_t sz)
		{
			return instance()._allocate(sz);
		}

		static void deallocate(void* ptr)
		{
			instance()._deallocate(ptr);
		}

	protected:
		Pool()
		{
			_data = _allocator.allocate(_capacity_number_of_objects);

			for (auto i = 0; i < _capacity_number_of_objects; i++)
			{
				_available.insert(i);
			}
		}

		~Pool()
		{
			_allocator.deallocate(_data);
		}

		static Pool<T>& instance()
		{
			if (!_instance)
			{
				_instance = new Pool<T>;
			}

			return *_instance;

		}

		T* _allocate(std:size_t sz)
		{
			if (_available.size > 0)
			{
				auto next_available = *_available.front();

				_available.pop();

				return _data[next_available];
			}
			else
			{
				// Fallback on malloc
				return malloc(sz)
			}

		}

		void _deallocate(void* ptr)
		{
			// if everything was 1 byte _data - ptr would be the index
			// so if its greater than one we should get a an appropriately adjusted index.
			auto index = (_data - ptr) / sizeof(T);

			assert

			// inside my data_block
			if (index >= 0 && index < _capacity_number_of_objects)
			{
				_in_use.remove(index);

				_available.push(index);
			}
			// not my ptr;
			else
			{
				free ptr;
			}
			
		};


	protected:
		static Pool<T>*		_instance;
		std::allocator<T>	_allocator;
		T*					_data;

		size_t				_capacity_number_of_objects = 4096 / sizeof(T);

		std::queue<int>		_available;
		std::set<int>		_in_use;
	};




	template <typename T>
	class Pool_Allocated
	{
	public:
		void* operator new(std::size_t sz)
		{
			assert(sz == sizeof(T));

			new (Pool<T>::allocate(sz)) T();
		}


		void operator delete(void* ptr)
		{
			Pool<T>::deallocate(ptr);
		}

	
	private:


		static int	_pool_capacity_bytes		= 4096;
		static int	_pool_capacity_num_objects  = _pool_capacity_bytes / sizeof(T);

		static std::set<int> _available;
		static std::set<int> _in_use;
	};
}