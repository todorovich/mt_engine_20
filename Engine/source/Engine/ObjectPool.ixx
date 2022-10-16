export module ObjectPool;

export import std.core;

export namespace mt 
{
	template<typename T, int number_of_objects> requires std::default_initializable<T>
	class ObjectPool
	{
	private:
		std::vector<T>			_data;
		std::priority_queue<int, std::vector<int>, std::greater<int>> 	_unused_indicies;
		std::set<int>				_used_indicies;

	public:

		ObjectPool()
			: _data(std::vector<T>(number_of_objects))
		{
			for (auto i = 0; i < number_of_objects; i++)
			{
				_unused_indicies.push(i);
			}
		}

		~ObjectPool() = default;

		ObjectPool(const ObjectPool& other) = delete;

		ObjectPool(ObjectPool&& other) = delete;

		ObjectPool operator=(const ObjectPool& other) = delete;
		
		ObjectPool operator=(ObjectPool&& other) = delete;

		T* GetMemory()
		{
			if (_unused_indicies.size() == 0)
			{
			    // To Do: This was previously throw, not sure what that changes.
				return nullptr;
			}

			auto index = _unused_indicies.top();

			_unused_indicies.pop();

			_used_indicies.insert(index);

			return &_data[index];

		}

		void ReleaseMemory(T* returned_memory)
		{
			// Check if we actually own this object.
			int index = static_cast<int>(returned_memory - &_data[0]);
			if (index < 0 || index >= number_of_objects)
			{
				return;
			}

			// zero out the retruned memory
			new(returned_memory) T{};

			_used_indicies.erase(index);

			_unused_indicies.push(index);
		}
	};
}