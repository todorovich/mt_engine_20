// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <catch2/catch_test_macros.hpp>
#include <Windows.h>
#include <expected>

export module EventTests;

import <cstddef>;
import <list>;
import Event;
import EventQueue;
import EventHandlerInterface;

using namespace mt::event;

struct EventHandler1 : public EventHandler<>
{
	std::list<int>* executedEvents{};
public:

	explicit EventHandler1(std::list<int>* list)
		: executedEvents(list)
	{}

	void operator()() noexcept override
	{
		executedEvents->push_back(1);
		OutputDebugStringW(L"Event Handler 1 called\n");
	}
};

struct EventHandler2 : public EventHandler<int>
{
	std::list<int>* executedEvents{};

	explicit EventHandler2(std::list<int>* list)
		: executedEvents(list)
	{}

	void operator()(int i) noexcept override
	{
		executedEvents->push_back(2);
		OutputDebugStringW(L"Event Handler 2 called with: ");
		OutputDebugStringW(std::to_wstring(i).c_str());
		OutputDebugStringW(L"\n");
	}
};

struct EventHandler3 : public EventHandler<int, int>
{
	std::list<int>* executedEvents{};
	int a{};
public:

	explicit EventHandler3(std::list<int>* list)
		: executedEvents(list)
	{}

	void operator()(int x, int y) noexcept override
	{
		executedEvents->push_back(3);
		OutputDebugStringW(L"Event Handler 3 called\n");
	}
};

TEST_CASE("Event Test Size Assumptions", "[events]")
{
	REQUIRE(4 == sizeof(int));

	REQUIRE(8 == sizeof(std::max_align_t));

	REQUIRE(1 == sizeof(std::tuple<>));
	REQUIRE(4 == sizeof(std::tuple<int>));
	REQUIRE(8 == sizeof(std::tuple<int,int>));
	REQUIRE(12 == sizeof(std::tuple<int,int,int>));

	REQUIRE(32 == sizeof(Event<>::EventPackage));

	REQUIRE(32 == sizeof(Event<int>::EventPackage));

	REQUIRE(32 == sizeof(Event<int, int>::EventPackage));

	REQUIRE(40 == sizeof(Event<int, int, int>::EventPackage));
}

TEST_CASE("Allocation Test", "[events]")
{
	EventQueue event_manager;
	std::list<int> executedEvents;

	Event<> event1 = Event<>(event_manager, L"Name");
	EventHandler1 event_handler_1{&executedEvents};
	event1.registerEventHandler(&event_handler_1);
	REQUIRE(event1.trigger());

	Event<int> event2 = Event<int>(event_manager, L"Name");
	EventHandler2 event_handler_2{&executedEvents};
	event2.registerEventHandler(&event_handler_2);
	int i = 1;
	REQUIRE(event2.trigger(i));

	REQUIRE(event1.trigger());

	REQUIRE(event2.trigger(1));

	Event<int,int> event3 = Event<int,int>(event_manager, L"Name");
	EventHandler3 event_handler_3{&executedEvents};
	event3.registerEventHandler(&event_handler_3);
	REQUIRE(event3.trigger(1,i));
	REQUIRE(event3.trigger(std::move(i),1));
	event_manager.processTriggeredEvents();

    REQUIRE(std::list {1, 2, 1, 2, 3, 3} == executedEvents);
}

static void requireNotEnoughMemory(std::expected<void, std::error_condition> expected)
{
	REQUIRE(!expected);
	auto error = expected.error();
	REQUIRE(error.category() == std::generic_category());
	REQUIRE(error.value() == static_cast<int>(std::errc::not_enough_memory));
}

TEST_CASE("Not Enough memory", "[events]")
{
	EventQueue event_manager{16};
	std::list<int> executedEvents;

	Event<int> event2 = Event<int>(event_manager, L"Name");
	EventHandler2 event_handler_2{&executedEvents};
	event2.registerEventHandler(&event_handler_2);

	requireNotEnoughMemory(event2.trigger(1));
}

TEST_CASE("Exactly Full Roll Over Test", "[events]")
{
	EventQueue event_manager{32};
	std::list<int> executedEvents;

	Event<int> event2 = Event<int>(event_manager, L"Name");
	EventHandler2 event_handler_2{&executedEvents};
	event2.registerEventHandler(&event_handler_2);

	REQUIRE(event2.trigger(1));

	for (auto i = 0; i < 10; ++i)
	{
		event_manager.processTriggeredEvents();

		// this one requires a roll-over;
		REQUIRE(event2.trigger(1));
		REQUIRE(32 == event_manager.getUsedSpace());
		REQUIRE(0 == event_manager.getFreeSpace());
	}

	requireNotEnoughMemory(event2.trigger(2));
}

TEST_CASE("Event Manager Capacity", "[events]")
{
	EventQueue event_manager{32};
	auto used = event_manager.getUsedSpace();
	auto free = event_manager.getFreeSpace();
	REQUIRE(event_manager.getCapacity() ==  used + free);
}

TEST_CASE("Not Enough Room Roll Over Test", "[events]")
{
	EventQueue event_manager{40};
	std::list<int> executedEvents;

	Event<> event1 = Event<>(event_manager, L"Name");
	EventHandler1 event_handler_1{&executedEvents};
	event1.registerEventHandler(&event_handler_1);

	REQUIRE(event1.trigger());
	REQUIRE(32 == event_manager.getUsedSpace());
	REQUIRE(8 == event_manager.getFreeSpace());

	for (auto i = 0; i < 10; ++i)
	{
		event_manager.processTriggeredEvents();

		// there is not enough space in the buffer to allocate this from 8 bytes in, so it should roll over.
		REQUIRE(event1.trigger());
		REQUIRE(32 == event_manager.getUsedSpace());
		REQUIRE(8 == event_manager.getFreeSpace());
	}

	requireNotEnoughMemory(event1.trigger());
}