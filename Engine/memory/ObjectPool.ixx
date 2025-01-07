// Copyright 2023 Micho Todorovich, all rights reserved.
export module ObjectPool;

import std.compat;

import Engine;
import Error;
import PriorityQueue;
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
			mt::Engine& _engine;
            ObjectPool<T, pool_capacity>& _object_pool;

        public:
            Deleter(mt::Engine& engine, ObjectPool<T, pool_capacity>& object_pool) noexcept
                : _engine(engine)
				, _object_pool(object_pool)
            {
            }

			void operator()(T const* pointer) noexcept
            {
                if (pointer) {
                    if (auto released = _object_pool.releaseMemory(pointer); !released) {
						_engine.crash(MakeErrorCondition(ErrorCode::BAD_ALLOCATION));
                    }
                }
            }
        };

    private:
        mt::Engine& _engine;
        Deleter _deleter{ _engine, *this };
        T* _data = static_cast<T*>(operator new[](pool_capacity * sizeof(T)));
        mt::queue::MinPriorityQueue<pool_capacity> _unused_indices{};
        std::set<int> _used_indices{};
        const std::size_t _capacity = pool_capacity;
        std::mutex _mutex;

		[[nodiscard]] bool releaseMemory(T const* returned_memory) noexcept
        {
            std::scoped_lock lock(_mutex);

            // Check if we actually own this object.
            int index = static_cast<int>(returned_memory - _data);
            if (returned_memory == nullptr || index < 0 || index >= _capacity)
            {
                return false;
            }
            else {
                returned_memory->~T();

                _used_indices.erase(index);
                _unused_indices.push(index);

                return true;
            }
        }

    public:
        friend ObjectPool::Deleter;
		
        ObjectPool(mt::Engine& engine, std::error_condition& error) noexcept
            : _engine(engine) 
        {
            if (_data == nullptr)
            {
                Assign(error, mt::error::ErrorCode::BAD_ALLOCATION);
				_engine.crash(error);
            }
            else
            {
                for (auto i = 0; i < _capacity; i++)
                {
                    _unused_indices.push(i);
                }
            }
        }

        ~ObjectPool() noexcept
        {
            for (int index : _used_indices)
            {
                _data[index].~T();
            }

            while (!_unused_indices.empty())
            {
                if (auto expected_index = _unused_indices.top(); expected_index)
                {
                    auto index = expected_index.value();
                    _unused_indices.pop();
                    _data[index].~T();
                }
            }

            operator delete[](_data);
        };

        ObjectPool(const ObjectPool& other) noexcept = delete;
        ObjectPool(ObjectPool&& other) noexcept = default;
        ObjectPool& operator=(const ObjectPool& other) noexcept = delete;
        ObjectPool& operator=(ObjectPool&& other) noexcept = default;

        [[nodiscard]] std::size_t size() const noexcept { return _used_indices.size(); }
        [[nodiscard]] constexpr std::size_t capacity() noexcept { return pool_capacity; }

        using unique_ptr_t = std::unique_ptr<T, Deleter>;

        template<class... Types>
        [[nodiscard]] std::expected<unique_ptr_t, std::error_condition> allocate(Types&&... args) noexcept
        {
            std::scoped_lock lock(_mutex);

            if (_unused_indices.empty()) {
                return std::unexpected(MakeErrorCondition(ErrorCode::OUT_OF_MEMORY));
            }

            auto index = _unused_indices.top();
            _unused_indices.pop();

            if (index)
            {
                int i = index.value();

                try {
                    _used_indices.insert(i);
                    return unique_ptr_t(new (&_data[i]) T(std::forward<Types>(args)...), _deleter);
                }
                catch (...) {
                    _unused_indices.push(i);
                    return std::unexpected(MakeErrorCondition(ErrorCode::BAD_ALLOCATION));
                }
            }
            else
            {
                return std::unexpected(MakeErrorCondition(ErrorCode::NO_SUCH_ELEMENT));
            }
        }
        
        Deleter get_deleter() const noexcept {
            return _deleter;
        }
    };
}
