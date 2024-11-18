// Copyright 2023 Micho Todorovich, all rights reserved.
export module ObjectPool;

import std.compat;

import Error;
import MakeUnique;

using namespace mt::error;

export namespace mt::memory
{
	template<typename T, std::size_t pool_capacity>
	class ObjectPool
	{
	public:
		class Deleter
		{
			ObjectPool<T, pool_capacity>& _object_pool;

		public:
			Deleter(ObjectPool<T, pool_capacity>& object_pool)
				: _object_pool(object_pool)
			{}

			void operator()(T const * pointer)
			{
				if (pointer) {
					if (auto expected = _object_pool.releaseMemory(pointer); !expected) return;
				}
			}
		};

	private:
		// Had to resort to malloc to get uninitialized memory. Not ideal, this is not modern cpp.
		T*															_data = (T*)::malloc(sizeof(T) * pool_capacity);
		std::priority_queue<int, std::vector<int>, std::greater<>> 	unused_indices;
		std::set<int>												_used_indices;
		const std::size_t _capacity = pool_capacity;
		Deleter deleter {*this};
		std::mutex mutex;

		[[nodiscard]] bool releaseMemory(T const * returned_memory)
		{
			std::scoped_lock lock(mutex);

			// Check if we actually own this object.
			int index = static_cast<int>(returned_memory - _data);
			if (returned_memory == nullptr || index < 0 || index >= _capacity)
			{
				return false;
			}
			else {
				returned_memory->~T();

				//if (mt::IS_DEBUG) std::memset_s(returned_memory, sizeof(T), 0, sizeof(T));

				_used_indices.erase(index);

				unused_indices.push(index);

				return true;
			}
		}

	public:
		friend ObjectPool::Deleter;
		friend std::unique_ptr<ObjectPool<T,pool_capacity>> mt::memory::make_unique_nothrow(Error&& error) noexcept;

		ObjectPool(std::error_condition& error) noexcept
		{
			if (_data == nullptr)
			{
				Assign(error, mt::error::ErrorCode::BAD_ALLOCATION);
			}
			else
			{
				for (auto i = 0; i < _capacity; i++)
				{
					unused_indices.push(i);
				}
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
		ObjectPool(ObjectPool&& other) noexcept = default;
		ObjectPool& operator=(const ObjectPool& other) noexcept = delete;
		ObjectPool& operator=(ObjectPool&& other) noexcept = default;

		[[nodiscard]] std::size_t size() const noexcept { return _used_indices.size(); }
		[[nodiscard]] constexpr std::size_t capacity() noexcept { return pool_capacity; }

		using unique_ptr_t = std::unique_ptr<T, Deleter>;

		template<class... Types>
		unique_ptr_t allocate(Types&&... args)
		{
			std::scoped_lock lock(mutex);

			if (unused_indices.empty()) return unique_ptr_t{nullptr, deleter};

			auto index = unused_indices.top();

			unused_indices.pop();

			_used_indices.insert(index);

			return unique_ptr_t(new (&_data[index]) T(std::forward<Types>(args)...), deleter);
		}


	};

	/*namespace factory
	{
		template<typename T, std::size_t pool_capacity>
		std::expected<std::unique_ptr<ObjectPool<T, pool_capacity>>, Error> ObjectPool() noexcept
		{
			Error error;
			if (auto pointer = make_unique_nothrow<mt::memory::ObjectPool<T, pool_capacity>>(error);
				pointer
			)
			{
				if (error.getErrorCode() == ErrorCode::ERROR_UNINITIALIZED)
					return std::unexpected{error};
				else
					return std::move(pointer);
			}
			else
			{
				return std::unexpected(
					mt::error::Error{
						std::wstring_view{ L"Unable to reset the command list allocator." },
						mt::error::ErrorCode::GRAPHICS_FAILURE,
						__func__, __FILE__, __LINE__
					}
				);
			}
		};
	}*/
}