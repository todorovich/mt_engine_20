module;

#include <catch.hpp>
#include <windows.h>

export module EngineTest;

import <chrono>;
import <thread>;

import Engine;

using namespace std::literals::chrono_literals;

TEST_CASE("Start the Engine", "[input]") 
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
