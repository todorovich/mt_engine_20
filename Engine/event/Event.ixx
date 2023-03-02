export module Event;

export import <cstdlib>;
export import <cstddef>;
export import <expected>;
export import <set>;
export import <memory>;
export import <stdlib.h>;

import Error;
import Task;

export namespace mt::event
{
	struct EventName
	{
		int hash;
		//  TODO: murmur hash
	};

	template<typename ... Parameters>
	struct EventHandler
	{
		virtual ~EventHandler() = default;
		virtual void operator()(Parameters ...) = 0;
	};

	struct EventPackageInterface
	{
		virtual ~EventPackageInterface() = default;

		[[nodiscard]] virtual EventPackageInterface* getNext() const = 0;
		virtual void setNext(EventPackageInterface*) = 0;
		virtual void operator()() = 0;
	};

	template<typename ... ParameterTypes> class Event;

	class EventManager
	{
		std::unique_ptr<std::byte, decltype(std::free)*> _data;
		const std::byte* _begin;
		const std::byte* _end;

		std::byte* _front;
		std::atomic<std::byte*> _back;

	public:

		EventManager(std::size_t size_of_queue = 1024 * 5) noexcept
			: _data(static_cast<std::byte*>(std::malloc(size_of_queue)), std::free)
			, _begin(_data.get())
			, _end(_data.get() + size_of_queue + 1)
			, _front(_data.get())
			, _back(_data.get())
		{}

		~EventManager() = default;
		EventManager(EventManager&&) = default;
		EventManager(const EventManager&) = default;
		EventManager& operator=(EventManager&&) = default;
		EventManager& operator=(const EventManager&) = default;

		template<typename ... ParameterTypes>
		void push(Event<ParameterTypes ...>& event,  ParameterTypes&& ... parameters)
		{
			using EventType = Event<ParameterTypes ...>;
			using Parameters = std::tuple<ParameterTypes ...>;

			class EventPackage : public EventPackageInterface
			{
				EventType& _event;
				EventPackageInterface* _next;

			public:
				EventPackage(EventType& event, EventPackageInterface* _next = nullptr)
					: _event(event)
					, _next(_next)
				{}

				~EventPackage() = default;
				EventPackage(EventPackage&&) = default;
				EventPackage(const EventPackage&) = default;
				EventPackage& operator=(EventPackage&&) = default;
				EventPackage& operator=(const EventPackage&) = default;

				[[nodiscard]] EventType getEvent() const
				{
					return _event;
				}

				[[nodiscard]] virtual EventPackageInterface* getNext() const override
				{
					return _next;
				}

				virtual void setNext(EventPackageInterface* next) override
				{
					_next = next;
				}

				virtual void operator()() override
				{
					_event();
				}
			};

			class EventPackageWithParameters : public EventPackage
			{
				Parameters _parameters;
			public:
				EventPackageWithParameters(EventType& event, EventPackageInterface* next, Parameters parameters)
					: EventPackage(event, next)
					, _parameters(parameters)
				{}

				~EventPackageWithParameters() = default;
				EventPackageWithParameters(EventPackageWithParameters&&) = default;
				EventPackageWithParameters(const EventPackageWithParameters&) = default;
				EventPackageWithParameters& operator=(EventPackageWithParameters&&) = default;
				EventPackageWithParameters& operator=(const EventPackageWithParameters&) = default;

				virtual void operator()() override
				{
					// This causes the event to actually notify.
					std::apply(EventPackage::getEvent(), _parameters);
				}
			};

			std::byte* back = _back.load();

			EventPackageInterface* previous = nullptr;
			if (_back != _front)
			{
				previous = reinterpret_cast<EventPackageInterface*>(back);
			}

			void* aligned = reinterpret_cast<void*>(back);
			do
			{
				std::size_t space = _end - back;
				// Do we fit at the back?
				if (auto expected = std::align(alignof(EventPackage), sizeof(EventPackage), aligned, space);
					!expected
				)
				{
					aligned = const_cast<void*>(reinterpret_cast<const void*>(_begin));
					space = _front - _begin;
					// How about wrapped at the front?
					if (expected = std::align(alignof(EventPackage), sizeof(EventPackage), aligned, space);
						!expected
					)
					{
						break; // TODO: fail
					}
				}
			} while (!_back.compare_exchange_weak(back, static_cast<std::byte*>(aligned) + sizeof(EventPackage)));

			if constexpr (sizeof...(ParameterTypes) == 0)
			{
				new (aligned) EventPackage{ event, nullptr };
			}
			else {
				new (aligned) EventPackageWithParameters{ event, nullptr, { parameters ... } };
			}

			// Todo: is this a race condition?s
			if (previous) previous->setNext(reinterpret_cast<EventPackageInterface*>(aligned));
		}


		// Not thread safe, must only ever be called from one thread (tick thread?).
		void processTriggeredEvents()
		{
			// This will only work if not including input events.
			// Events can cause other events.
			// Ideally all events which are causes this frame are processed this frame.
			// Similarly, we should only process input for this frame and not next frame.
			// Adding input events in here would intermingle knock-on events and next frame's input.
			while (_front != _back.load())
			{
				auto event_package = reinterpret_cast<EventPackageInterface*>(_front);

				(*event_package)();

				if (event_package->getNext())
					_front = reinterpret_cast<std::byte*>(event_package->getNext());
				else
					break;
			}
		}
	};

	template<typename ... ParameterTypes>
	class Event
	{
		using EventHandlerType = EventHandler<ParameterTypes ...>;
		EventManager&			_event_manager;
		std::set<EventHandlerType*> event_handlers;

	public:
		EventName				event_name;

		Event(EventManager& event_manager)
			: _event_manager(event_manager)
		{}

		~Event() = default;
		Event(Event&&) = default;
		Event(const Event&) = default;
		Event& operator=(Event&&) = default;
		Event& operator=(const Event&) = default;

		void registerEventHandler(EventHandlerType* event_handler)
		{
			event_handlers.insert(event_handler);
		}

		void operator()(ParameterTypes&& ... parameters)
		{
			for (auto& event_handler : event_handlers)
			{
				if constexpr (sizeof...(ParameterTypes) == 0)
				{
					(*event_handler)();
				}
				else
				{
					(*event_handler)(std::forward<ParameterTypes ...>(parameters ...));
				}
			}
		}

		void trigger(ParameterTypes&& ... parameters)
		{
			if constexpr (sizeof...(ParameterTypes) == 0) {
				_event_manager.push(*this);
			}
			else {
				_event_manager.push(*this, std::forward<ParameterTypes ...>(parameters ...));
			}
		}
	};
}
