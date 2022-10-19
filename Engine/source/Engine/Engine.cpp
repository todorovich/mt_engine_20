module;

#include <windows.h>

module Engine;

import Camera;
import CommandManager;
import DirectXRenderer;
import InputManager;
import LogManager;
import Status;
import TimeManager;
import WindowManager;
import WindowsMessageManager;

import std.core;
import std.filesystem;

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

Engine::Engine()
	: command_manager_(std::make_unique<command::CommandManager>())
	, direct_x_renderer_(std::make_unique<renderer::DirectXRenderer>(*this))
	, input_manager_(std::make_unique<input::InputManager>(*this))
	, log_manager_(std::make_unique<logging::LogManager>())
	, windows_message_manager_(std::make_unique<windows::WindowsMessageManager>(*this))
	, window_manager_(std::make_unique<windows::WindowManager>(*this))
	, time_manager_(std::make_unique<time::TimeManager>())
{
	if (_instance == nullptr)
		_instance = this;
	else
		throw new std::runtime_error("Only one mt:::Engine may exist at a time.");
}

Engine::~Engine()
{
	if (!IsDestroyed())
	{
		Destroy();
	}
};

bool Engine::Initialize(HINSTANCE instance_handle)
{
	windows_message_manager_->Initialize();

	// Will Register Message Handler WNDPROC
	if (!GetWindowManager()->initializeMainWindow(instance_handle))
		return false;

	if (!GetRenderer()->InitializeDirect3d(GetWindowManager()->getMainWindowHandle()))
		return false;

	// Do the initial Resize code. 
	GetWindowManager()->resize(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));

	return true;
}

Status Engine::Run()
{
	auto run = [&]() {

		GetTimeManager()->FindTimer(mt::time::TimeManager::DefaultTimers::FRAME_TIME).startTask();

		//_engine_tick_thread = std::thread(std::ref(Engine::Tick));

		// Message handler must be on same thread as the window (this thread)
		MSG msg = { 0 };

		// TODO: windows messages (input) should be processed on a different thread than the ticks.
		bool quit = false;
		long long last_frame_outputed = 0;

		while (true)
		{
			auto now = time::Clock::now();

			auto last_frame_rendered = GetRenderer()->GetFramesRendered();

			if (last_frame_rendered % 1440 == 0 && last_frame_outputed != last_frame_rendered)
			{
				last_frame_outputed = last_frame_rendered;

				mt::time::Duration average = GetTimeManager()->
					FindTimer(mt::time::TimeManager::DefaultTimers::FRAME_TIME).getAverageTaskInterval();

				OutputDebugStringW(
					(std::to_wstring(GetRenderer()->GetFramesRendered()) + L" frame number : ").c_str()
				);

				OutputDebugStringW(
					(std::to_wstring(static_cast<long double>(average.count() / 1'000'000.0)) + L" ns : ").c_str()
				);

				OutputDebugStringW(
					(std::to_wstring(1'000'000'000.0 / static_cast<double>(average.count())) + L" FPS\n").c_str()
				);
			}

			GetTimeManager()->FindTimer(mt::time::TimeManager::DefaultTimers::WINDOWS_MESSAGE_TIME).doTask(
				[&]() {
					// If there are Window messages then process them.
					while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
					{
						//VK_ACCEPT
						TranslateMessage(&msg);
						DispatchMessage(&msg);
						if (msg.message == WM_QUIT)
						{
							quit = true;
							break;
						}
					}
				}
			);

			if (quit) break;

			GetTimeManager()->FindTimer(mt::time::TimeManager::DefaultTimers::TICK_TIME).doTask(
				[&]() { _Tick(); } // do i need the lambda to capture this?
			);
		};
	};

	GetTimeManager()->FindTimer(mt::time::TimeManager::DefaultTimers::RUN_TIME).doTask(run);
		

	// Join the Tick thread (ensuring it has actually shut down)
	//if (_engine_tick_thread.joinable()) _engine_tick_thread.join();

	//OutputDebugStringW(L"Engine Has Stopped.\n");

	return Status::success;
}

void Engine::Shutdown()
{
	_is_shutting_down = true;

	GetTimeManager()->Pause();

	OutputDebugStringW(L"Engine Shutdown Initiated\n");

	Destroy();
}

void Engine::Destroy() 
{
	if (_instance != nullptr) 
	{
		// Destroy the window
		DestroyWindow(GetWindowManager()->getMainWindowHandle());

		_instance = nullptr;
	}

	OutputDebugStringW(L"Engine Destroyed\n");
}

void Engine::_Tick()
{
	GetTimeManager()->Tick();
		
	GetTimeManager()->FindTimer(mt::time::TimeManager::DefaultTimers::UPDATE_TIME).doTask(
		[&]() {
			if (GetTimeManager()->GetShouldUpdate())
			{
				GetTimeManager()->FindTimer(mt::time::TimeManager::DefaultTimers::UPDATE_TIME).pauseTask();

				
				GetTimeManager()->FindTimer(mt::time::TimeManager::DefaultTimers::INPUT_TIME).doTask(
					[&]() { GetInputManager()->ProcessInput(); }
				);

				GetTimeManager()->FindTimer(mt::time::TimeManager::DefaultTimers::UPDATE_TIME).continueTask();
				
				_Update();
			}
		}
	);

	GetTimeManager()->FindTimer(mt::time::TimeManager::DefaultTimers::RENDER_TIME).doTask(
		[&]() {
			// Render whenever you can, but don't wait.
			if (GetTimeManager()->GetShouldRender() && GetRenderer()->IsCurrentFenceComplete())
			{
				GetRenderer()->Update();
				_Draw();
				GetRenderer()->Render();
				GetRenderer()->IncrementFence();
				GetTimeManager()->RenderComplete();

				mt::time::StopWatch& stop_watch = GetTimeManager()->FindTimer(mt::time::TimeManager::DefaultTimers::FRAME_TIME);

				stop_watch.finishTask();
				stop_watch.startTask();
			}	
		}
	);
}