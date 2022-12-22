module;

#include <catch.hpp>
#include <windows.h>

export module StopWatchTests;

import <chrono>;

import Engine;
import StopWatch;

using namespace std::literals;

TEST_CASE("StopWatch Unit Tests", "[StopWatch]")
{
    auto hwnd = GetModuleHandle(nullptr);
    auto instance = (HINSTANCE)GetWindowLong((HWND)hwnd, GWLP_HINSTANCE);

    mt::Engine engine = mt::Engine(instance);
    mt::Game game = mt::Game();

    auto thread = new std::jthread([&]() {
        engine.run(game);
    });

    std::this_thread::sleep_for(2s);

    engine.shutdown();

    REQUIRE(engine.isDestroyed());
}