module;

#include <expected>

export module EventManagerInterface;

export import <system_error>;
export import <memory>;

export import Error;
export import EventQueue;
export import Name;

import <map>;

using namespace mt::utility;
using namespace mt::error;

namespace mt::event
{
	class EventManagerInterface
	{
		std::map<Name, std::unique_ptr<mt::event::EventQueue>> eventQueues{};

		EventManagerInterface() noexcept = default;
		EventManagerInterface(const EventManagerInterface&) noexcept = default;
		EventManagerInterface(EventManagerInterface&&) noexcept = default;
		EventManagerInterface& operator=(const EventManagerInterface&) noexcept = default;
		EventManagerInterface& operator=(EventManagerInterface&&) noexcept = default;
		auto operator<=>(const EventManagerInterface&) const noexcept = default;

		[[nodiscard]] std::expected<mt::event::EventQueue*, std::error_condition> createEventQueue(
			Name name, std::size_t size
		)
		{
			auto pair = eventQueues.try_emplace(name, std::make_unique<EventQueue>(size));
			if (pair.second)
				return pair.first->second.get();
			else
				return std::unexpected{MakeErrorCondition(ErrorCode::EVENT_QUEUE_ALREADY_EXISTS)};
		}

		[[nodiscard]] std::expected<mt::event::EventQueue*, std::error_condition> getEventQueue(Name name)
		{
			return eventQueues.find(name)->second.get();
		}
	};

}
