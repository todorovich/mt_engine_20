export module ObjectPool;

import <ctime>;
import <queue>;
import <set>;

export namespace mt 
{
	template<typename T, std::size_t number_of_objects>
	class ObjectPool
	{
	private:
		// Had to resort to malloc to get uninitialized memory. Not ideal, this is not modern cpp.
		T*																_data = (T*)malloc(sizeof(T) * number_of_objects);
		std::priority_queue<int, std::vector<int>, std::greater<int>> 	_unused_indicies;
		std::set<int>													_used_indicies;

	public:

		ObjectPool() noexcept
		{
			for (auto i = 0; i < number_of_objects; i++)
			{
				_unused_indicies.push(i);
			}
		}

		~ObjectPool() noexcept
		{
			for (auto index = _unused_indicies.top(); !_unused_indicies.empty(); _unused_indicies.pop())
			{
				_data[index].~T();
			}
		};

		ObjectPool(const ObjectPool& other) noexcept = delete;

		ObjectPool(ObjectPool&& other) noexcept = delete;

		ObjectPool operator=(const ObjectPool& other) noexcept = delete;
		
		ObjectPool operator=(ObjectPool&& other) noexcept = delete;

		template<class... Types>
		T* allocate(Types&&... args)
		{
			if (_unused_indicies.size() == 0)
			{
			    // To Do: This was previously throw, not sure what that changes.
				throw std::bad_alloc();
			}

			auto index = _unused_indicies.top();

			_unused_indicies.pop();

			_used_indicies.insert(index);

			return new (&_data[index]) T(std::forward<Types>(args)...);
		}

		void releaseMemory(T* returned_memory)
		{
			// Check if we actually own this object.
			int index = static_cast<int>(returned_memory - _data);
			if (index < 0 || index >= number_of_objects)
			{
				throw std::bad_alloc();
			}
			else {
				returned_memory->~T();

				// zero out the retruned memory
				//std::memset_s(returned_memory, sizeof(T), 0, sizeof(T));

				_used_indicies.erase(index);

				_unused_indicies.push(index);
			}
		}
	};
}