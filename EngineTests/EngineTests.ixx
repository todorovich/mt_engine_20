// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <catch.hpp>

export module EngineTest;

import std;

import Engine;
import StandardEngine;
import Game;
import MakeUnique;
import TimeManagerInterface;

using namespace std::literals::chrono_literals;

TEST_CASE("Start the Engine", "[input]") 
{
	auto expected = mt::memory::ExpectMakeUnique<mt::StandardEngine>();

	REQUIRE(expected);
	REQUIRE(*expected != nullptr);

	auto&& engine = std::move(expected.value());


	auto thread = std::jthread(
		[&]() {
			// returns an expected, which casts to bool.
			REQUIRE(engine->run(mt::memory::make_unique_nothrow<mt::Game>()));
		}
	);

	auto time = engine->getTimeManager()->now();
	const auto frames_to_render = 5;
	while(
		engine->getRenderer()->getFramesRendered() <= frames_to_render &&
		(engine->getTimeManager()->now() - time) < (engine->getTimeManager()->getRenderInterval() * (frames_to_render + 1))
	) {}

	engine->shutdown();
}
