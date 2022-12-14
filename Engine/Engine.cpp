module;

#include <windows.h>

module Engine;

import <string_view>;
import <chrono>;

import Camera;
import DirectXRenderer;
import InputManager;
import StopWatch;
import TimeManager;
import WindowManager;
import WindowsMessageManager;

using namespace std::literals;

const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)  
struct THREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.  
	LPCSTR szName; // Pointer to _name (in user addr space).  
	DWORD dwThreadID; // Thread ID (-1=caller thread).  
	DWORD dwFlags; // Reserved for future use, must be zero.  
};
#pragma pack(pop)  

void SetThreadName(DWORD dwThreadID, const char* threadName) {
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;
#pragma warning(push)  
#pragma warning(disable: 6320 6322)  
	__try {
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
	}
#pragma warning(pop)  
}

const std::string_view mt::time::TimeManager::DefaultTimers::RUN_TIME= "Run Time"sv;
const std::string_view mt::time::TimeManager::DefaultTimers::WINDOWS_MESSAGE_TIME = "Windows Message Time"sv;
const std::string_view mt::time::TimeManager::DefaultTimers::TICK_TIME = "Engine Tick Time"sv;
const std::string_view mt::time::TimeManager::DefaultTimers::UPDATE_TIME = "Update Time"sv;
const std::string_view mt::time::TimeManager::DefaultTimers::INPUT_TIME = "Input Time"sv;
const std::string_view mt::time::TimeManager::DefaultTimers::RENDER_TIME = "Render Time"sv;
const std::string_view mt::time::TimeManager::DefaultTimers::FRAME_TIME = "Frame Time"sv;

using namespace mt;

Engine* Engine::_instance = nullptr;

Engine::Engine(HINSTANCE instance_handle)
	: _direct_x_renderer(std::make_unique<renderer::DirectXRenderer>(*this))
	, _input_manager(std::make_unique<input::InputManager>(*this))
	, _windows_message_manager(std::make_unique<windows::WindowsMessageManager>(*this))
	, _window_manager(std::make_unique<windows::WindowManager>(*this))
	, _time_manager(std::make_unique<time::TimeManager>(*this))
{
	if (_instance == nullptr)
		_instance = this;
	else
		throw new std::runtime_error("Only one mt:::Engine may exist at a time.");

	_windows_message_manager->initialize();
	 
	// Will Register Message Handler WNDPROC
	if (!getWindowManager()->initializeMainWindow(instance_handle))
		throw new std::runtime_error("Could not initialize main window");

	if (auto expected = getRenderer()->initializeDirect3d(getWindowManager()->getMainWindowHandle()); !expected)
		throw new std::runtime_error("Could not initialize direct3d");

	// Do the initial Resize code.
	if (auto expected =
			getWindowManager()->resize(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));
		!expected
	)
		throw new std::runtime_error("Could not resize the window");
}

Engine::~Engine() noexcept
{
	if (!isDestroyed())
	{
		destroy();
	}
};

#pragma warning (push)
#pragma warning (disable: 4715)
std::expected<void, Error> Engine::run(Game& game) noexcept
{
	auto run_time = getTimeManager()->findStopWatch(mt::time::TimeManager::DefaultTimers::RUN_TIME);
	run_time->startTask();

	auto input_time = getTimeManager()->findStopWatch(mt::time::TimeManager::DefaultTimers::INPUT_TIME);
	auto frame_time = getTimeManager()->findStopWatch(mt::time::TimeManager::DefaultTimers::FRAME_TIME);
	auto update_time = getTimeManager()->findStopWatch(mt::time::TimeManager::DefaultTimers::UPDATE_TIME);
	auto render_time = getTimeManager()->findStopWatch(mt::time::TimeManager::DefaultTimers::RENDER_TIME);
	auto tick_time = getTimeManager()->findStopWatch(mt::time::TimeManager::DefaultTimers::TICK_TIME);
	auto windows_message_time = getTimeManager()->findStopWatch(mt::time::TimeManager::DefaultTimers::WINDOWS_MESSAGE_TIME);

	frame_time->startTask();
	// TODO: windows messages (input) should be processed on a different thread than the ticks.
	long long last_frame_outputed = 0;

	while (true)
	{
		auto now = std::chrono::steady_clock::now();

		auto last_frame_rendered = getRenderer()->getFramesRendered();

		if (last_frame_rendered % 1440 == 0 && last_frame_outputed != last_frame_rendered)
		{
			last_frame_outputed = last_frame_rendered;

			std::chrono::steady_clock::duration average = frame_time->getAverageTaskInterval();

			OutputDebugStringW(
				(std::to_wstring(getRenderer()->getFramesRendered()) + L" frame number : ").c_str()
			);

			OutputDebugStringW(
				(std::to_wstring(static_cast<long double>(average.count() / 1'000'000.0)) + L" ns : ").c_str()
			);

			OutputDebugStringW(
				(std::to_wstring(1'000'000'000.0 / static_cast<double>(average.count())) + L" FPS\n").c_str()
			);
		}

		windows_message_time->doTask(
			[](mt::Engine& engine) noexcept -> std::expected<void, mt::Error> {
				MSG msg = {0};
				// If there are Window messages then process them.
				while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
				{
					//VK_ACCEPT
					TranslateMessage(&msg);
					DispatchMessage(&msg);
					if (msg.message == WM_QUIT)
					{
						if (!engine.isShuttingDown()) engine.shutdown();
						break;
					}
				}

				return {};
			}
		);

		if (this->_is_shutting_down) break;

		if (auto expected = _tick(tick_time, update_time, render_time, frame_time, input_time, game); !expected)
			return std::unexpected(expected.error());
	};

	run_time->finishTask();
}
#pragma warning ( pop )

void Engine::shutdown() noexcept
{
	if (_is_shutting_down == false)
	{
		_is_shutting_down = true;

		getTimeManager()->pause();

		OutputDebugStringW(L"Engine Shutdown Initiated\n");

		destroy();
	}
	else
	{
		OutputDebugStringW(L"Engine Shutdown Already Initiated\n");
	}
}

void Engine::destroy() noexcept
{
	if (_instance != nullptr) 
	{
		// Destroy the window
		DestroyWindow(getWindowManager()->getMainWindowHandle());

		_instance = nullptr;

		OutputDebugStringW(L"Engine Destroyed\n");
	}
	else
		OutputDebugStringW(L"Engine Already Destroyed\n");
}

#pragma warning (push)
#pragma warning (disable: 4715)
std::expected<void, Error> Engine::_tick(
	mt::time::StopWatch* tick_time, 
	mt::time::StopWatch* update_time, 
	mt::time::StopWatch* render_time, 
	mt::time::StopWatch* frame_time,
	mt::time::StopWatch* input_time,
	mt::Game& game
) noexcept
{
	tick_time->startTask();

	getTimeManager()->tick();
		
	update_time->startTask();
	if (getTimeManager()->getShouldUpdate())
	{
		game.physicsUpdate();

		getTimeManager()->updateComplete();
	}
	update_time->finishTask();

	render_time->startTask();
	// Render whenever you can, but don't wait.
	if (getTimeManager()->getShouldRender())
	{
		input_time->startTask();
		getInputManager()->processInput(); 
		game.inputUpdate();
		input_time->finishTask();

		game.renderUpdate();
		getRenderer()->update();
		if (auto expected = getRenderer()->render(); !expected) return std::unexpected(expected.error());
		getTimeManager()->renderComplete();

		frame_time->finishTask();
		frame_time->startTask();
	}	
	render_time->finishTask();

	tick_time->finishTask();
}
#pragma warning ( pop )