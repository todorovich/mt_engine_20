export module Event;

import std;

import EventQueue;
import EventPackageInterface;
import EventHandlerInterface;
import Name;

using namespace std::literals;

using namespace mt::utility;

export namespace mt::event
{
	template<typename ... ParameterTypes>
	class Event
	{
	private:
		EventQueue& _event_queue;

		std::set<EventHandler<ParameterTypes ...>*> event_handlers;

	public:
		Name _event_name;

		class EventPackage : public EventPackageInterface
		{
			Event<ParameterTypes ...>& _event;
			std::tuple<ParameterTypes ...> _parameters;

		public:
			EventPackage(Event<ParameterTypes ...>& event, std::tuple<ParameterTypes ...> parameters) noexcept
				: EventPackageInterface(sizeof(EventPackage))
				, _event(event)
				, _parameters(parameters)
			{}

			~EventPackage() noexcept final = default;
			EventPackage(EventPackage&&) noexcept = default;
			EventPackage(const EventPackage&) noexcept = default;
			EventPackage& operator=(EventPackage&&) noexcept = default;
			EventPackage& operator=(const EventPackage&) noexcept = default;

			void operator()() noexcept override
			{
				std::apply<Event<ParameterTypes...>&, std::tuple<ParameterTypes ...>>
					(_event, std::forward<std::tuple<ParameterTypes ...>>(_parameters));
			}
		};

		explicit Event(EventQueue& event_manager, std::wstring name) noexcept
			: _event_queue(event_manager)
			, _event_name(name)
		{}

		~Event() noexcept = default;
		Event(Event&&) noexcept = default;
		Event(const Event&) noexcept = default;
		Event& operator=(Event&&) noexcept = default;
		Event& operator=(const Event&) noexcept = default;
		Event& operator<=>(const Event&) const noexcept = default;

		void registerEventHandler(EventHandler<ParameterTypes ...>* event_handler) noexcept
		{
			event_handlers.insert(event_handler);
		}

		void deregisterEventHandler(EventHandler<ParameterTypes ...>* event_handler) noexcept
		{
			event_handlers.remove(event_handler);
		}

		void operator()([[maybe_unused]] ParameterTypes&& ... parameters) noexcept
		{
			for (auto& event_handler : event_handlers)
			{
				(*event_handler)(parameters...);
			}
		}

		[[nodiscard]] std::expected<void, std::error_condition> trigger(ParameterTypes ... parameters) noexcept
		{
			return _event_queue.push(
				Event::EventPackage(
					*this,
					std::tuple<ParameterTypes...>(parameters...)
				)
			);
		}
	};
}
