// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <catch.hpp>
#include <windows.h>

export module EngineTest;

import <chrono>;
import <thread>;

import Engine;
import Game;
import MakeUnique;

using namespace std::literals::chrono_literals;

TEST_CASE("Start the Engine", "[input]") 
{
	auto engine = mt::Engine();

	auto thread = std::jthread(
		[&]() {
			// returns an expected, which casts to bool.
			REQUIRE(engine.run(mt::memory::make_unique_nothrow<mt::Game>()));
		}
	);

	auto time = engine.getTimeManager()->now();
	const auto frames_to_render = 5;
	while(
		engine.getRenderer()->getFramesRendered() <= frames_to_render &&
		(engine.getTimeManager()->now() - time) < (engine.getTimeManager()->getRenderInterval() * (frames_to_render + 1))
	) {}

	engine.shutdown();
}
