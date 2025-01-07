export module StandardEngine;

import std;

import RendererInterface;
import TimeManagerInterface;
import InputManagerInterface;
import WindowManagerInterface;
import WindowsMessageManagerInterface;

import Engine;

import MakeUnique;
import Error;

using mt::input::InputManagerInterface;
using mt::renderer::RendererInterface;
using mt::windows::WindowsMessageManagerInterface;
using mt::windows::WindowManagerInterface;
using mt::time::TimeManagerInterface;

using namespace mt::error;
using namespace mt::memory;

export namespace mt 
{
    class StandardEngine : public Engine
    {
        std::shared_ptr<InputManagerInterface> _getInputManager(std::error_condition& error_condition) noexcept;
        std::shared_ptr<TimeManagerInterface> _getTimeManager(std::error_condition& error_condition) noexcept;
        std::shared_ptr<WindowManagerInterface> _getWindowManager(std::error_condition& error_condition) noexcept;
        std::shared_ptr<RendererInterface> _getRenderer(std::error_condition& error_condition) noexcept;

        StandardEngine(const StandardEngine& other) noexcept = default;
        StandardEngine(StandardEngine&& other) noexcept = default;
        StandardEngine& operator=(const StandardEngine& other) noexcept = default;
        StandardEngine& operator=(StandardEngine&& other) noexcept = default;

    public:
        StandardEngine(std::error_condition& error_condition) noexcept
            : Engine()
        {
            auto input_manager = _getInputManager(error_condition);
            if (error_condition.value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED)) return;
            setInputManager(input_manager);

            auto time_manager = _getTimeManager(error_condition);
            if (error_condition.value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED)) return;
            setTimeManager(time_manager);

            auto window_manager = _getWindowManager(error_condition);
            if (error_condition.value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED)) return;
            setWindowManager(window_manager);

            auto renderer = _getRenderer(error_condition);
            if (error_condition.value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED)) return;
            setRenderer(renderer);
        }

        ~StandardEngine() noexcept = default;
    };
}