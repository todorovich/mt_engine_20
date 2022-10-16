#include <windows.h>
//#include <WinUser.h>
//#include "wingdi.h"

module Engine;

import std.core;

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

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return mt::Engine::GetWindowsMessageManager().handle_message(hwnd, msg, wParam, lParam);
}

using namespace mt;

std::unique_ptr<Engine> mt::Engine::_instance = std::make_unique<Engine>();

Status Engine::_Run()
{
	//_engine_tick_thread = std::thread(std::ref(Engine::Tick));

	// Message handler must be on same thread as the window (this thread)
	MSG msg = { 0 };

	auto now = time::Clock::now();

	GetTimeManager().GetTotalUpTimeChronometer().Start(now);
	GetTimeManager().GetUpdateChronometer().Start(now);
	GetTimeManager().GetRenderChronometer().Start(now);
	GetTimeManager().GetFrameChronometer().Start(now);
	GetTimeManager().GetIdleChronometer().Start(now);
	GetTimeManager().GetStatisticsChronometer().Start(now);
	GetTimeManager().GetWindowsMessageChronometer().Start(now);
	GetTimeManager().GetInputChronometer().Start(now);
	GetTimeManager().GetTickChronometer().Start(now);
	GetTimeManager().GetInBetweenTicksChronometer().Start(now);

	GetTimeManager().GetUpdateChronometer().Pause(now);
	GetTimeManager().GetRenderChronometer().Pause(now);
	GetTimeManager().GetStatisticsChronometer().Pause(now);
	GetTimeManager().GetWindowsMessageChronometer().Pause(now);
	GetTimeManager().GetInputChronometer().Pause(now);
	GetTimeManager().GetTickChronometer().Pause(now);
	{

		bool quit = false;
		while (!quit)
		{
			now = time::Clock::now();
			GetTimeManager().GetWindowsMessageChronometer().Continue(now);
			GetTimeManager().GetIdleChronometer().Pause(now);
			{
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
			now = time::Clock::now();
			GetTimeManager().GetIdleChronometer().Continue(now);
			GetTimeManager().GetWindowsMessageChronometer().Pause(now);

			GetTimeManager().GetInBetweenTicksChronometer().Pause();
			{
				GetEngine()._Tick();
			}
			GetTimeManager().GetInBetweenTicksChronometer().Continue();
		};
	}
	now = time::Clock::now();
	GetTimeManager().GetTotalUpTimeChronometer().Stop(now);
	GetTimeManager().GetUpdateChronometer().Stop(now);
	GetTimeManager().GetRenderChronometer().Stop(now);
	GetTimeManager().GetFrameChronometer().Stop(now);
	GetTimeManager().GetIdleChronometer().Stop(now);
	GetTimeManager().GetStatisticsChronometer().Stop(now);
	GetTimeManager().GetWindowsMessageChronometer().Stop(now);
	GetTimeManager().GetInputChronometer().Stop(now);
	GetTimeManager().GetTickChronometer().Stop(now);
	GetTimeManager().GetInBetweenTicksChronometer().Stop(now);
	// Join the Tick thread (ensuring it has actually shut down)
	//if (_engine_tick_thread.joinable()) _engine_tick_thread.join();

	//OutputDebugStringW(L"Engine Has Stopped.\n");

	return Status::success;
}

void Engine::_Tick()
{

	GetTimeManager().Tick();

	if (GetTimeManager().GetEndOfFrame())
	{
		GetTimeManager().GetIdleChronometer().Pause();

		_UpdateFrameStatisticsNoTimeCheck(_was_rendered_this_frame);

		auto now = time::Clock::now();
		GetTimeManager().GetFrameChronometer().Lap(now);
		GetTimeManager().GetIdleChronometer().Lap(now);
		GetTimeManager().GetStatisticsChronometer().Lap(now);
		GetTimeManager().GetWindowsMessageChronometer().Lap(now);
		GetTimeManager().GetTickChronometer().Lap(now);
		GetTimeManager().GetInBetweenTicksChronometer().Lap(now);

		GetTimeManager().FrameComplete();

		_was_rendered_this_frame = false;

		GetTimeManager().GetIdleChronometer().Continue();
	}

	if (GetTimeManager().GetShouldUpdate())
	{
		GetTimeManager().GetIdleChronometer().Pause();

		// Input
		GetTimeManager().GetInputChronometer().Continue();
		{
			input_manager_.ProcessInput();
			GetTimeManager().GetInputChronometer().Lap();
		}
		GetTimeManager().GetInputChronometer().Pause();

		// Update
		GetTimeManager().GetUpdateChronometer().Continue();
		{
			_Update();
			direct_x_renderer_.Update();
		}
		GetTimeManager().GetUpdateChronometer().Pause();

		GetTimeManager().UpdateComplete();

		GetTimeManager().GetUpdateChronometer().Lap();

		GetTimeManager().GetIdleChronometer().Continue();
	}

	if (GetTimeManager().GetShouldRender())
	{
		// Render whenever you can, but don't wait.
		if (GetRenderer().IsCurrentFenceComplete())
		{
			auto now = time::Clock::now();
			GetTimeManager().GetIdleChronometer().Pause(now);
			GetTimeManager().GetRenderChronometer().Continue(now);
			{
				_Draw();
				direct_x_renderer_.Render();
				direct_x_renderer_.IncrementFence();
				_was_rendered_this_frame = true;
			}
			GetTimeManager().GetRenderChronometer().Pause();

			GetTimeManager().RenderComplete();

			GetTimeManager().GetRenderChronometer().Lap();

			GetTimeManager().GetIdleChronometer().Continue();
		}
	}
	GetTimeManager().GetIdleChronometer().Continue();
}

bool Engine::_Initialize(HINSTANCE hInstance)
{
	_instance_handle = hInstance;

	if (!_InitializeMainWindow())
		return false;

	if (!direct_x_renderer_.InitializeDirect3d(_main_window_handle))
		return false;

	// Do the initial Resize code.
	Resize(_window_width, _window_height);

	time_manager_.Initialize();

	return true;
}

bool Engine::_InitializeMainWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _instance_handle;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"MainWnd";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, _window_width, _window_height };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	_main_window_handle = CreateWindow(L"MainWnd", _main_window_caption.c_str(),
		WS_MAXIMIZE, 0, 0, width, height, nullptr, nullptr, _instance_handle, 0);

	if (!_main_window_handle)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}


	SetWindowLong(_main_window_handle, GWL_STYLE, 0); //remove all window styles, check MSDN for details

	ShowWindow(_main_window_handle, SW_SHOWMAXIMIZED); //display window

	UpdateWindow(_main_window_handle);

	return true;
}

void Engine::_UpdateFrameStatisticsNoTimeCheck(bool was_rendered)
{
	/*GetTimeManager().GetIdleChronometer().Pause();
	GetTimeManager().GetStatisticsChronometer().Continue();

	static const float ns_to_ms = 1e-6f;

	auto& game_chrono = Engine::GetTimeManager().GetTotalUpTimeChronometer();
	auto& update_chrono = Engine::GetTimeManager().GetUpdateChronometer();
	auto& render_chrono = Engine::GetTimeManager().GetRenderChronometer();
	auto& frame_chrono = Engine::GetTimeManager().GetFrameChronometer();
	auto& idle_chrono = Engine::GetTimeManager().GetIdleChronometer();
	auto& stats_chrono = Engine::GetTimeManager().GetStatisticsChronometer();
	auto& wm_chrono = Engine::GetTimeManager().GetWindowsMessageChronometer();
	auto& input_chrono = Engine::GetTimeManager().GetInputChronometer();
	auto& tick_chrono = Engine::GetTimeManager().GetTickChronometer();
	auto& in_between_chrono = Engine::GetTimeManager().GetInBetweenTicksChronometer();

	float ns_to_s = 1e-9f;

	const auto emsp = "&emsp;";
	const auto red_span = "<span style=\"color:Red\">";
	const auto span_end = "</span>";

	io::Indenter indenter;

	archiver << '\n' << "<table id=\"outer\">" << '\n';

	indenter++;
	archiver << indenter << "<tr>" << '\n';

	indenter++;
	archiver << indenter << "<td style=\"vertical-align:top\">" << '\n';

	indenter++;
	archiver << indenter << "<table class=\"mt\">" << '\n';

	indenter++;
	archiver << indenter << "<tr>" << '\n';

	indenter++;
	archiver
		<< indenter << "<th></th>" << '\n'
		<< indenter << "<th>Duration Since Started</th>" << '\n'
		<< indenter << "<th>Average Duration Active</th>" << '\n'
		<< indenter << "<th>Average Duration Paused</th>" << '\n'
		<< indenter << "<th>Average Total Duration</th>" << '\n'
		<< indenter << "<th>Last Active Duration</th>" << '\n'
		<< indenter << "<th>Last Paused Duration</th>" << '\n'
		<< indenter << "<th>Last Total Duration</th>" << '\n';

	indenter--;
	archiver
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	archiver
		<< indenter << "<td>" << game_chrono.GetName() << "</td>" << '\n'
		<< indenter << "<td>" << game_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << game_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << game_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << game_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << game_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << game_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << game_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';

	indenter--;
	archiver
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	archiver
		<< indenter << "<td>" << update_chrono.GetName() << "</td>" << '\n'
		<< indenter << "<td>" << update_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << update_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << update_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << update_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << update_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << update_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << update_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';

	indenter--;
	archiver
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	archiver
		<< indenter << "<td>" << render_chrono.GetName() << "</td>" << '\n'
		<< indenter << "<td>" << render_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << render_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << render_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << render_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << render_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << render_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << render_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';


	indenter--;
	archiver
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	archiver
		<< indenter << "<td>" << frame_chrono.GetName() << "</td>" << '\n'
		<< indenter << "<td>" << frame_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << frame_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << frame_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << frame_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << frame_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << frame_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << frame_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';

	indenter--;
	archiver
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	archiver
		<< indenter << "<td>" << idle_chrono.GetName() << "</td>" << '\n'
		<< indenter << "<td>" << idle_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << idle_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << idle_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << idle_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << idle_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << idle_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << idle_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';

	indenter--;
	archiver
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	archiver
		<< indenter << "<td>" << stats_chrono.GetName() << "</td>" << '\n'
		<< indenter << "<td>" << stats_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << stats_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << stats_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << stats_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << stats_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << stats_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << stats_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';

	indenter--;
	archiver
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	archiver
		<< indenter << "<td>" << wm_chrono.GetName() << "</td>" << '\n'
		<< indenter << "<td>" << wm_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << wm_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << wm_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << wm_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << wm_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << wm_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << wm_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';

	indenter--;
	archiver
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	archiver
		<< indenter << "<td>" << input_chrono.GetName() << "</td>" << '\n'
		<< indenter << "<td>" << input_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << input_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << input_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << input_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << input_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << input_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << input_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';

	indenter--;
	archiver
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	archiver
		<< indenter << "<td>" << tick_chrono.GetName() << "</td>" << '\n'
		<< indenter << "<td>" << tick_chrono.GetDurationSinceStarted().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << tick_chrono.GetAverageActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << tick_chrono.GetAveragePausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << tick_chrono.GetAverageTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << tick_chrono.GetLastActiveDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << tick_chrono.GetLastPausedDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << tick_chrono.GetLastTotalDuration().count() * ns_to_ms << "<i>ms</i>" << "</td>" << '\n';

	indenter--;
	archiver << indenter << "</tr>" << '\n';

	indenter--;
	archiver << indenter << "</table>" << '\n';

	indenter--;
	archiver
		<< indenter << "</td>" << '\n'
		<< indenter << "<td style=\"vertical-align:top\">" << '\n';

	indenter++;
	archiver
		<< indenter << "<table class=\"mt\">" << '\n';

	indenter++;
	archiver
		<< indenter << "<tr>" << '\n';

	indenter++;
	archiver
		<< indenter << "<th></th>" << '\n'
		<< indenter << "<th>Update</th>" << '\n'
		<< indenter << "<th>Render</th>" << '\n'
		<< indenter << "<th>Frame</th>" << '\n'
		<< indenter << "<th>Idle</th>" << '\n'
		<< indenter << "<th>Statistics</th>" << '\n'
		<< indenter << "<th>Windows Messages</th>" << '\n'
		<< indenter << "<th>Input</th>" << '\n'
		<< indenter << "<th>Tick</th>" << '\n'
		<< indenter << "<th>In-Between Ticks</th>" << '\n';

	indenter--;
	archiver
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	archiver
		<< indenter << "<td>Average</td>" << '\n'
		<< indenter << "<td>" << (update_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n';

	if (was_rendered)
	{
		archiver << indenter << "<td>" << (render_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n';
	}
	else
	{
		archiver << indenter << "<td> --<i>ms</i>" << "</td>" << '\n';
	}


	archiver
		<< indenter << "<td>" << (frame_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << (idle_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << (stats_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << (wm_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << (input_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << (tick_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << (in_between_chrono.GetAverageActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n';

	indenter--;
	archiver
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	archiver
		<< indenter << "<td>Last Sample</td>" << '\n'
		<< indenter << "<td>" << (update_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << (render_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << (frame_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << (idle_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << (stats_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << (wm_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << (input_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << (tick_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n'
		<< indenter << "<td>" << (in_between_chrono.GetLastActiveDuration().count() * ns_to_ms) << "<i>ms</i>" << "</td>" << '\n';

	indenter--;
	archiver << indenter << "</tr>" << '\n';

	indenter--;
	archiver << indenter << "</table>" << '\n';

	indenter--;
	archiver << indenter << "</td>" << '\n';

	indenter--;
	archiver << indenter << "</tr>" << '\n';

	indenter--;
	archiver << indenter << "</table>" << '\n';

	archiver.flush();

	_time_since_stat_update = 0ns;

	GetTimeManager().GetStatisticsChronometer().Pause();
	GetTimeManager().GetIdleChronometer().Continue();*/
}

void Engine::_resize(int width, int height)
{
	// This flag should prevent futher rendering after the current frame finishes
	_set_is_resizing(true);

	// wait until rendering is finished.
	while (direct_x_renderer_.GetIsRendering()) {};

	_window_width = width;
	_window_height = height;
	_window_aspect_ratio = static_cast<float>(_window_width) / _window_height;

	direct_x_renderer_.Resize(width, height);

	// Trigger callback
	_OnResize();
	// Continue rendering.
	_set_is_resizing(false);
}
