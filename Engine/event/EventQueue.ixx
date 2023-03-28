module;

// TODO: importing breaks intellisense, recursively
#include <expected>

export module EventQueue;

import <cstdlib>;
import <memory>;
import <mutex>;

import EventPackageInterface;

export namespace mt::event
{
	class EventQueue
	{
		std::unique_ptr<std::byte, decltype(std::free)*> _data =
			std::unique_ptr<std::byte, decltype(std::free)*>((std::byte*)nullptr, std::free);
		const std::size_t _capacity;
		std::byte * const _start{};
		std::byte * const _end{};
		std::byte * _front{};
		std::byte * _back{};
		std::byte * _rollover{};
		std::mutex _back_lock{};

	public:
		explicit EventQueue(std::size_t size_of_queue = 1024 * 5) noexcept
			: _data(static_cast<std::byte*>(std::malloc(size_of_queue)), std::free)
			, _capacity(size_of_queue)
			, _start(_data.get())
			, _end(_data.get() + size_of_queue + 1)
			, _front(_data.get())
			, _back(_data.get())
			, _rollover(_data.get())
		{}

		~EventQueue() = default;
		EventQueue(EventQueue&&) = delete;
		EventQueue(const EventQueue&) = delete;
		EventQueue& operator=(EventQueue&&) = delete;
		EventQueue& operator=(const EventQueue&) = delete;

		template<typename EventPackageType> requires std::derived_from<EventPackageType, EventPackageInterface>
		[[nodiscard]] std::expected<void, std::error_condition> push(EventPackageType&& event_package)
		{
			[[maybe_unused]] auto lock = std::lock_guard(_back_lock);

			void* aligned = reinterpret_cast<void*>(_back);

			std::size_t space = (_end - _back) - std::size_t{1};
			// Do we fit at the back?
			if (auto expected = std::align(alignof(EventPackageType), sizeof(EventPackageType), aligned, space);
				expected == nullptr
				)
			{
				aligned = const_cast<void*>(reinterpret_cast<const void*>(_start));
				// b
				// ------f/b---------E
				// ------FxxxxB------E
				// xxxxxxB----FxxxxxxE
				if (_front < _back)
					space = _front - _start;
				else
					space = _back - _start;

				_rollover = _back;

				// How about wrapped at the front?
				if (expected = std::align(alignof(std::max_align_t), sizeof(EventPackageType), aligned, space);
					!expected
					)
				{
					return std::unexpected(std::make_error_condition(std::errc::not_enough_memory)); // TODO: fail
				}
			}

			_back = static_cast<std::byte*>(aligned) + sizeof(EventPackageType);

			memcpy_s(aligned, _end - reinterpret_cast<std::byte*>(aligned), &event_package , sizeof(EventPackageType));

			return {};
		}

		[[nodiscard]] constexpr std::size_t getCapacity() const
		{
			return _capacity;
		}

		[[nodiscard]] std::size_t getUsedSpace()
		{
			// b
			// ooooooFxxxxBooooooE B-F
			// xxxxxxBooooFxxxxxxE E-F + B-b
			if (_front == _back)
				return 0;
			else if (_front < _back)
					return _back - _front;
				else
					return (_end - _front) + (_back - _start) - std::size_t{1};
		}

		[[nodiscard]] std::size_t getFreeSpace()
		{
			if (_front == _back)
				return (_end - _start) - std::size_t{1};
			else if (_front < _back)
					return (_front - _start) + (_end - _back) - std::size_t{1};
				else
					return _front - _back;
		}

		// TODO: need a pop? for multithreaded.
		// Not thread safe, must only ever be called from one thread (tick thread?).
		void processTriggeredEvents() noexcept
		{
			auto back = _back;
			while (_front != back)
			{
				auto event_package = reinterpret_cast<EventPackageInterface*>(_front);

				(*event_package)();

				_front = _front + event_package->size();
				if (_front == _rollover || _front == _end)
				{
					_front = _start;
					_rollover = _end;
				}
			}

			if ([[maybe_unused]] auto lock = std::lock_guard(_back_lock);
				_front == _back
			)
			{
				_front = _start;
				_back = _start;
			}
		}
	};
}