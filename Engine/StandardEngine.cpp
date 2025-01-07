module;

#include <windows.h>

module StandardEngine;

import std;

import RendererInterface;
import TimeManagerInterface;
import InputManagerInterface;
import WindowManagerInterface;
import WindowsMessageManagerInterface;

import DirectXRenderer;
import BasicInputManager;
import StandardTimeManager;
import WindowsWindowManager;
import MakeUnique;

using mt::input::InputManagerInterface;
using mt::renderer::RendererInterface;
using mt::windows::WindowsMessageManagerInterface;
using mt::windows::WindowManagerInterface;
using mt::time::TimeManagerInterface;
using mt::time::StandardTimeManager;

using namespace mt;

std::shared_ptr<InputManagerInterface> StandardEngine::_getInputManager(std::error_condition& error) noexcept
{
    if (auto input_manager = mt::memory::make_shared_nothrow<input::BasicInputManager>(*this, error);
        input_manager.get() == nullptr || error.value() != static_cast<int>(mt::error::ErrorCode::ERROR_UNINITIALIZED)
        )
    {
        if (input_manager.get() == nullptr)
            crash(MakeErrorCondition(mt::error::ErrorCode::BAD_ALLOCATION));
        else
            crash(error);

        return nullptr;
    }
    else
    {
        return std::move(input_manager);
    }
}

std::shared_ptr<TimeManagerInterface> StandardEngine::_getTimeManager(std::error_condition& error) noexcept
{
    if (auto time_manager = mt::memory::make_shared_nothrow<time::StandardTimeManager>(*this, error);
        time_manager.get() == nullptr || error.value() != static_cast<int>(mt::error::ErrorCode::ERROR_UNINITIALIZED)
        )
    {
        if (time_manager.get() == nullptr)
            crash(MakeErrorCondition(mt::error::ErrorCode::BAD_ALLOCATION));
        else
            crash(error);

        return nullptr;
    }
    else
    {
        return std::move(time_manager);
    }
}

std::shared_ptr<WindowManagerInterface> StandardEngine::_getWindowManager(std::error_condition& error) noexcept
{
    if (auto window_manager = mt::memory::make_shared_nothrow<windows::WindowsWindowManager>(
        *this, error, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)
    );
        window_manager.get() == nullptr || error.value() != static_cast<int>(mt::error::ErrorCode::ERROR_UNINITIALIZED)
        )
    {
        if (window_manager.get() == nullptr)
            crash(MakeErrorCondition(mt::error::ErrorCode::BAD_ALLOCATION));
        else
            crash(error);

        return nullptr;
    }
    else
    {
        return std::move(window_manager);
    }
}

std::shared_ptr<RendererInterface> StandardEngine::_getRenderer(std::error_condition& error) noexcept
{
    if (auto renderer = mt::memory::make_shared_nothrow<renderer::DirectXRenderer>(*this);
        renderer.get() == nullptr || error.value() != static_cast<int>(mt::error::ErrorCode::ERROR_UNINITIALIZED)
    )
    {
        if (renderer.get() == nullptr)
            crash(MakeErrorCondition(mt::error::ErrorCode::BAD_ALLOCATION));
        else
            crash(error);

        return nullptr;
    }
    else
    {
        return std::move(renderer);
    }
}
