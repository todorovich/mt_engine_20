// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <catch.hpp>
#include <windows.h>

export module EventTests;

export import Event;

using namespace mt::event;

struct EventHandler1 : public EventHandler<>
{
public:
	virtual void operator()() override
	{
		OutputDebugStringW(L"Event Handler 1 called\n");
	}
};

struct EventHandler2: public EventHandler<>
{
	virtual void operator()() override
	{
		OutputDebugStringW(L"Event Handler 2 called\n");
	}
};


TEST_CASE("New Test", "[New Test Category]")
{
	EventManager event_manager;

	Event<> event = Event<>(event_manager);

	EventHandler1 event_handler_1;
	EventHandler2 event_handler_2;

	event.registerEventHandler(&event_handler_1);
	event.registerEventHandler(&event_handler_2);

	event.trigger();

	event_manager.processTriggeredEvents();

    REQUIRE(false);
}