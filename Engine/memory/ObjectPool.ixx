// Copyright 2023 Micho Todorovich, all rights reserved.
export module ObjectPool;

import <ctime>;
import <queue>;
import <set>;

export namespace mt::memory
{
	// TODO: make pool allocated smart pointer that keeps a reference to the pool and releases its memory in destructor
	// template <typename T> class PoolAllocated<T>;

	template<typename T, std::size_t pool_capacity>
	class ObjectPool
	{
	private:
		// Had to resort to malloc to get uninitialized memory. Not ideal, this is not modern cpp.
		T*															_data = (T*)malloc(sizeof(T) * pool_capacity);
		std::priority_queue<int, std::vector<int>, std::greater<>> 	unused_indices;
		std::set<int>												_used_indices;
		const std::size_t _capacity = pool_capacity;
	public:

		// TODO: noexcept all the things.
		// Seems like the dominant strategy to get around failing to allocate is to preallocate all the memory you need
		// up front so you either fail to initialize or never run out of memory. Seems easier said than done but we'll
		// see
		ObjectPool()
		{
			if (_data == nullptr) throw new std::bad_alloc();

			for (auto i = 0; i < _capacity; i++)
			{
				unused_indices.push(i);
			}
		}

		~ObjectPool() noexcept
		{
			for (auto index = unused_indices.top(); !unused_indices.empty(); unused_indices.pop())
			{
				_data[index].~T();
			}
		};

		ObjectPool(const ObjectPool& other) noexcept = delete;
		ObjectPool(ObjectPool&& other) noexcept = delete;
		ObjectPool operator=(const ObjectPool& other) noexcept = delete;
		ObjectPool operator=(ObjectPool&& other) noexcept = delete;

		[[nodiscard]] std::size_t size() const { return _used_indices.size(); }
		[[nodiscard]] constexpr std::size_t capacity() { return pool_capacity; }

		// TODO: noexcept all the things.
		template<class... Types>
		T* allocate(Types&&... args)
		{
			if (unused_indices.empty()) throw std::bad_alloc();

			auto index = unused_indices.top();

			unused_indices.pop();

			_used_indices.insert(index);

			return new (&_data[index]) T(std::forward<Types>(args)...);
		}

		void releaseMemory(T* returned_memory)
		{
			// Check if we actually own this object.
			int index = static_cast<int>(returned_memory - _data);
			if (index < 0 || index >= _capacity)
			{
				throw std::bad_alloc();
			}
			else {
				returned_memory->~T();

				// zero out the returned memory
				//std::memset_s(returned_memory, sizeof(T), 0, sizeof(T));

				_used_indices.erase(index);

				unused_indices.push(index);
			}
		}
	};



}