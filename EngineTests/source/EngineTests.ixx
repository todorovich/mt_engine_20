module;

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <Windows.h>

export module EngineTest;

#pragma warning( push )
#pragma warning( disable : 5050 )
export import std.core;
#pragma warning( pop )

import Engine;
import Time;
import InputManager;


using namespace std::literals::chrono_literals;

TEST_CASE("Start the Engine", "[input]") 
{
    auto hwnd = GetModuleHandle(nullptr);
    auto instance = (HINSTANCE)GetWindowLong((HWND)hwnd, GWLP_HINSTANCE);

    mt::Engine engine = mt::Engine();

    engine.initialize(instance);

    auto thread = new std::jthread([&]() { engine.run(); });

    std::this_thread::sleep_for(1s);

    engine.shutdown();

    REQUIRE(engine.isDestroyed());
}
