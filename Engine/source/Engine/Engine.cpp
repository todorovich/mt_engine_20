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

const std::string_view mt::time::TimeManager::DefaultTimers::FRAME_TIME = "Frame Time"sv;
const std::string_view mt::time::TimeManager::DefaultTimers::WINDOWS_MESSAGE_TIME = "Windows Message Time"sv;
const std::string_view mt::time::TimeManager::DefaultTimers::TICK_TIME = "Engine Tick Time"sv;
const std::string_view mt::time::TimeManager::DefaultTimers::UPDATE_TIME = "Update Time"sv;
const std::string_view mt::time::TimeManager::DefaultTimers::INPUT_TIME = "Input Time"sv;
const std::string_view mt::time::TimeManager::DefaultTimers::RENDER_TIME = "Render Time"sv;

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
	//_engine_tick_thread = std::thread(std::ref(Engine::Tick));

	// Message handler must be on same thread as the window (this thread)
	MSG msg = { 0 };


	// TODO: windows messages (input) should be processed on a different thread than the ticks.
	bool quit = false;
	while (!quit)
	{
		auto now = time::Clock::now();

		GetTimeManager()->FindTimer(mt::time::TimeManager::DefaultTimers::FRAME_TIME).doTask(
			[&]() {
				GetTimeManager()->FindTimer(mt::time::TimeManager::DefaultTimers::WINDOWS_MESSAGE_TIME).doTask(
					[&]() {
						// If there are Window messages then process them.
						while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
							if (msg.message == WM_QUIT)
							{
								quit = true;
							}
						}
					}
				);

				GetTimeManager()->FindTimer(mt::time::TimeManager::DefaultTimers::TICK_TIME).doTask(
					[&]() { _Tick(); } // do i need the lambda to capture this?
				);
			}
		);
	};

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
				GetTimeManager()->FindTimer(mt::time::TimeManager::DefaultTimers::UPDATE_TIME).pause_task();

				
				GetTimeManager()->FindTimer(mt::time::TimeManager::DefaultTimers::INPUT_TIME).doTask(
					[&]() { GetInputManager()->ProcessInput(); }
				);

				GetTimeManager()->FindTimer(mt::time::TimeManager::DefaultTimers::UPDATE_TIME).continue_task();
				
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
			}	
		}
	);
}

//void Engine::_UpdateFrameStatisticsNoTimeCheck(bool was_rendered)
//{
//	/*GetTimeManager().GetIdleChronometer().Pause();
//	GetTimeManager().GetStatisticsChronometer().Continue();
//
//	static const float ns_to_ms = 1e-6f;
//
//	auto& game_chrono = Engine::GetTimeManager().GetTotalUpTimeChronometer();
//	auto& update_chrono = Engine::GetTimeManager().GetUpdateChronometer();
//	auto& render_chrono = Engine::GetTimeManager().GetRenderChronometer();
//	auto& frame_chrono = Engine::GetTimeManager().GetFrameChronometer();
//	auto& idle_chrono = Engine::GetTimeManager().GetIdleChronometer();
//	auto& stats_chrono = Engine::GetTimeManager().GetStatisticsChronometer();
//	auto& wm_chrono = Engine::GetTimeManager().GetWindowsMessageChronometer();
//	auto& input_chrono = Engine::GetTimeManager().GetInputChronometer();
//	auto& tick_chrono = Engine::GetTimeManager().GetTickChronometer();
//	auto& in_between_chrono = Engine::GetTimeManager().GetInBetweenTicksChronometer();
//
//	float ns_to_s = 1e-9f;
//
//	const auto emsp = "&emsp;";
//	const auto red_span = "<span style=\"color:Red\">";
//	const auto span_end = "</span>";
//
//	io::Indenter indenter;
//
//	archiver << '\n' << "<table id=\"outer\">" << '\n';
//
//	indenter++;
//	archiver << indenter << "<tr>" << '\n';
//
//	indenter++;
//	archiver << indenter << "<td style=\"vertical-align:top\">" << '\n';
//
//	indenter++;
//	archiver << indenter << "<table class=\"mt\">" << '\n';
//
//	indenter++;
//	archiver << indenter << "<tr>" << '\n';
//
//	indenter++;
//	archiver
//		<< indenter << "<th></th>" << '\n'
//		<< indenter << "<th>Duration Since Started</th>" << '\n'
//		<< indenter << "<th>Average Duration Active</th>" << '\n'
//		<< indenter << "<th>Average Duration Paused</th>" << '\n'
//		<< indenter << "<th>Average Total Duration</th>" << '\n'
//		<< indenter << "<th>Last Active Duration</th>" << '\n'
//		<< indenter << "<th>Last Paused Duration</th>" << '\n'
//		<< indenter << "<th>Last Total Duration</th>" << '\n';
//
//	indenter--;
//	archiver
//		<< indenter << "</tr>" << '\n'
//		<< indenter << "<tr>" << '\n';
//
//	indenter++;
//	archiver
//		<< indenter << "<td>" << game_chrono.GetName() << "</td>" << '\n'
//		<< indenter << "<td>" << game_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << game_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << game_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << game_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << game_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << game_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << game_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';
//
//	indenter--;
//	archiver
//		<< indenter << "</tr>" << '\n'
//		<< indenter << "<tr>" << '\n';
//
//	indenter++;
//	archiver
//		<< indenter << "<td>" << update_chrono.GetName() << "</td>" << '\n'
//		<< indenter << "<td>" << update_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << update_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << update_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << update_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << update_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << update_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << update_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';
//
//	indenter--;
//	archiver
//		<< indenter << "</tr>" << '\n'
//		<< indenter << "<tr>" << '\n';
//
//	indenter++;
//	archiver
//		<< indenter << "<td>" << render_chrono.GetName() << "</td>" << '\n'
//		<< indenter << "<td>" << render_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << render_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << render_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << render_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << render_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << render_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << render_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';
//
//
//	indenter--;
//	archiver
//		<< indenter << "</tr>" << '\n'
//		<< indenter << "<tr>" << '\n';
//
//	indenter++;
//	archiver
//		<< indenter << "<td>" << frame_chrono.GetName() << "</td>" << '\n'
//		<< indenter << "<td>" << frame_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << frame_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << frame_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << frame_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << frame_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << frame_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << frame_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';
//
//	indenter--;
//	archiver
//		<< indenter << "</tr>" << '\n'
//		<< indenter << "<tr>" << '\n';
//
//	indenter++;
//	archiver
//		<< indenter << "<td>" << idle_chrono.GetName() << "</td>" << '\n'
//		<< indenter << "<td>" << idle_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << idle_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << idle_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << idle_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << idle_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << idle_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << idle_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';
//
//	indenter--;
//	archiver
//		<< indenter << "</tr>" << '\n'
//		<< indenter << "<tr>" << '\n';
//
//	indenter++;
//	archiver
//		<< indenter << "<td>" << stats_chrono.GetName() << "</td>" << '\n'
//		<< indenter << "<td>" << stats_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << stats_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << stats_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << stats_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << stats_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << stats_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << stats_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';
//
//	indenter--;
//	archiver
//		<< indenter << "</tr>" << '\n'
//		<< indenter << "<tr>" << '\n';
//
//	indenter++;
//	archiver
//		<< indenter << "<td>" << wm_chrono.GetName() << "</td>" << '\n'
//		<< indenter << "<td>" << wm_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << wm_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << wm_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << wm_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << wm_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << wm_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << wm_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';
//
//	indenter--;
//	archiver
//		<< indenter << "</tr>" << '\n'
//		<< indenter << "<tr>" << '\n';
//
//	indenter++;
//	archiver
//		<< indenter << "<td>" << input_chrono.GetName() << "</td>" << '\n'
//		<< indenter << "<td>" << input_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << input_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << input_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << input_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << input_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << input_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << input_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';
//
//	indenter--;
//	archiver
//		<< indenter << "</tr>" << '\n'
//		<< indenter << "<tr>" << '\n';
//
//	indenter++;
//	archiver
//		<< indenter << "<td>" << tick_chrono.GetName() << "</td>" << '\n'
//		<< indenter << "<td>" << tick_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << tick_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << tick_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << tick_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << tick_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << tick_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << tick_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';
//
//	indenter--;
//	archiver << indenter << "</tr>" << '\n';
//
//	indenter--;
//	archiver << indenter << "</table>" << '\n';
//
//	indenter--;
//	archiver
//		<< indenter << "</td>" << '\n'
//		<< indenter << "<td style=\"vertical-align:top\">" << '\n';
//
//	indenter++;
//	archiver
//		<< indenter << "<table class=\"mt\">" << '\n';
//
//	indenter++;
//	archiver
//		<< indenter << "<tr>" << '\n';
//
//	indenter++;
//	archiver
//		<< indenter << "<th></th>" << '\n'
//		<< indenter << "<th>Update</th>" << '\n'
//		<< indenter << "<th>Render</th>" << '\n'
//		<< indenter << "<th>Frame</th>" << '\n'
//		<< indenter << "<th>Idle</th>" << '\n'
//		<< indenter << "<th>Statistics</th>" << '\n'
//		<< indenter << "<th>Windows Messages</th>" << '\n'
//		<< indenter << "<th>Input</th>" << '\n'
//		<< indenter << "<th>Tick</th>" << '\n'
//		<< indenter << "<th>In-Between Ticks</th>" << '\n';
//
//	indenter--;
//	archiver
//		<< indenter << "</tr>" << '\n'
//		<< indenter << "<tr>" << '\n';
//
//	indenter++;
//	archiver
//		<< indenter << "<td>Average</td>" << '\n'
//		<< indenter << "<td>" << (update_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n';
//
//	if (was_rendered)
//	{
//		archiver << indenter << "<td>" << (render_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n';
//	}
//	else
//	{
//		archiver << indenter << "<td> --<i>ms</i>" << "</td>" << '\n';
//	}
//
//
//	archiver
//		<< indenter << "<td>" << (frame_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << (idle_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << (stats_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << (wm_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << (input_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << (tick_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << (in_between_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n';
//
//	indenter--;
//	archiver
//		<< indenter << "</tr>" << '\n'
//		<< indenter << "<tr>" << '\n';
//
//	indenter++;
//	archiver
//		<< indenter << "<td>Last Sample</td>" << '\n'
//		<< indenter << "<td>" << (update_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << (render_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << (frame_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << (idle_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << (stats_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << (wm_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << (input_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << (tick_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
//		<< indenter << "<td>" << (in_between_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n';
//
//	indenter--;
//	archiver << indenter << "</tr>" << '\n';
//
//	indenter--;
//	archiver << indenter << "</table>" << '\n';
//
//	indenter--;
//	archiver << indenter << "</td>" << '\n';
//
//	indenter--;
//	archiver << indenter << "</tr>" << '\n';
//
//	indenter--;
//	archiver << indenter << "</table>" << '\n';
//
//	archiver.flush();
//
//	_time_since_stat_update = 0ns;
//
//	GetTimeManager().GetStatisticsChronometer().Pause();
//	GetTimeManager().GetIdleChronometer().Continue();*/
//}
