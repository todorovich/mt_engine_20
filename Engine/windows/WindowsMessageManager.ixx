// Copyright 2024 Micho Todorovich, all rights reserved.
export module WindowsMessageManager;

import std;

export import Engine;
export import WindowsMessageManagerInterface;
export import WindowsMessage;

import Windows;

using namespace windows;
using namespace mt::error;

export namespace mt::windows
{
	class WindowsMessageLoopTask : public mt::task::Task
	{
		mt::Engine& _engine;

		std::atomic<bool> received_quit = false;
		std::atomic<bool> should_destroy_window = false;
		std::atomic<bool> should_toggle_cursor = false;
		bool isCursorShowing = true;

	public:
		WindowsMessageLoopTask(mt::Engine& engine)
			: _engine(engine)
		{}

		std::expected<void, std::error_condition> operator()() noexcept
		{
			long long last_frame_outputed = 0;

			auto frame_time = _engine.getTimeManager()->findStopWatch(mt::time::DefaultTimers::FRAME_TIME);
			auto windows_message_time =
				_engine.getTimeManager()->findStopWatch(mt::time::DefaultTimers::WINDOWS_MESSAGE_TIME);

			// This can fail in theory, but I don't want to crash if it does.
			HRESULT hr = SetThreadDescription(GetCurrentThread(),L"mt::Engine Windows Message Thread");
			// TODO Check this result and do something

			while (!received_quit)
			{
				auto last_frame_rendered = _engine.getRenderer()->getFramesRendered();

				if (last_frame_rendered % 1440 == 0 && last_frame_outputed != last_frame_rendered)
				{
					last_frame_outputed = last_frame_rendered;

					std::chrono::steady_clock::duration average = frame_time->getAverageTaskInterval();

					OutputDebugString(
						(std::to_wstring(_engine.getRenderer()->getFramesRendered()) + L" frame number : ").c_str()
					);

					OutputDebugString(
						(std::to_wstring(static_cast<long double>(average.count() / 1'000'000.0)) + L" ns : ").c_str()
					);

					OutputDebugString(
						(std::to_wstring(1'000'000'000.0 / static_cast<double>(average.count())) + L" FPS\n").c_str()
					);
				}

				windows_message_time->startTask();

				MSG msg = { 0 };
				// If there are Window.ixx messages then process them.
				while (_engine.getInputManager()->isAcceptingInput() && PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
				{
					//VK_ACCEPT
					TranslateMessage(&msg);
					DispatchMessage(&msg);
					if (msg.message == WM_QUIT)
					{
						if (!_engine.isShuttingDown()) _engine.shutdown();
						received_quit.store(true);
						break;
					}
				}

				windows_message_time->finishTask();

				if (auto handle = _engine.getWindowManager()->getWindow()->getHandle();
					handle && should_destroy_window.load()
				)
				{
					// This has to be done on the same thread that created the window. hence the design.
					if (auto result = DestroyWindow(static_cast<HWND>(handle)); result != 0) {
						should_destroy_window.store(false);
					}
					else
					{
						// TODO: do something...?
						auto error = GetLastError();
					}
				}

				if (should_toggle_cursor)
				{
					isCursorShowing = !isCursorShowing;
					ShowCursor(isCursorShowing);
					should_toggle_cursor.store(false);
				}
			};


			return {};
		}

		void destroyMainWindow() { should_destroy_window.store(true); }
		bool hasReceivedQuit() { return received_quit.load(); }
		void toggleShowCursor() { should_toggle_cursor.store(!should_toggle_cursor); }
	};

	class WindowsMessageManager : public WindowsMessageManagerInterface
	{
		Engine& _engine;

		std::map<int, std::unique_ptr<WindowsMessage>> _message_handler_map;

		mt::windows::WindowsMessageLoopTask _windows_message_loop_task;

	public:
		WindowsMessageManager(Engine& engine, std::error_condition& error);
		~WindowsMessageManager() = default;
		WindowsMessageManager(const WindowsMessageManager&) noexcept = default;
		WindowsMessageManager(WindowsMessageManager&&) noexcept = default;
		WindowsMessageManager& operator=(const WindowsMessageManager&) noexcept = default;
		WindowsMessageManager& operator=(WindowsMessageManager&&) noexcept = default;

		mt::windows::WindowsMessageLoopTask* getWindowsMessageLoopTask() { return &_windows_message_loop_task; }

		virtual LRESULT handle_message(
			const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam
		) override;

		virtual mt::task::Task* getMessageLoopTask() noexcept override { return &_windows_message_loop_task; };

		// TODO: these are used as passthrough to the windows message loop
		//  expose the windows message loop instead.
		virtual void destroyMainWindow() noexcept override;
		virtual bool hasReceivedQuit() noexcept override;
		virtual void toggleShowCursor() noexcept override;
	};
}
