export module InputTestEngine;

import std;

import Engine;
import MakeUnique; 
import BasicInputManager;
import WindowsWindowManager;
import MakeUnique;

export namespace mt::input::test
{
    class InputTestEngine : public mt::Engine
    {
    public:
        InputTestEngine(std::error_condition& error) noexcept
            : mt::Engine()
        {
            // Initialize only the BasicInputManager
            auto inputManager = mt::memory::make_shared_nothrow<mt::input::BasicInputManager>(*this, error);
            setInputManager(inputManager);

			auto windowManager = mt::memory::make_shared_nothrow<mt::windows::WindowsWindowManager>(*this, error, 1920, 1080);
            setWindowManager(windowManager);
        }
    };
}
