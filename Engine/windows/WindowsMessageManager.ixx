// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessageManager;

export import <expected>;

export import Engine;
export import WindowsMessageManagerInterface;
export import WindowsMessage;

import <map>;
import <memory>;

using namespace mt::error;

export namespace mt::windows
{
	class WindowsMessageLoopTask : public mt::task::Task
	{
		mt::Engine& _engine;

		bool receivedQuit = false;

	public:
		WindowsMessageLoopTask(mt::Engine& engine)
			: _engine(engine)
		{}

		std::expected<void, mt::error::Error> operator()() noexcept
		{
			// This can fail in theory, but I don't want to crash if it does.
			HRESULT hr = SetThreadDescription(GetCurrentThread(),L"Windows Message Thread");

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
					receivedQuit = true;
					break;
				}
			}

			return {};
		}

		bool hasReceivedQuit() { return receivedQuit; }
	};

	class WindowsMessageManager : public WindowsMessageManagerInterface
	{
		Engine& _engine;

		std::map<int, std::unique_ptr<WindowsMessage>> _message_handler_map;

	protected:


	public:
		WindowsMessageManager(Engine& engine, Error& error);
		~WindowsMessageManager() = default;
		WindowsMessageManager(const WindowsMessageManager&) noexcept = default;
		WindowsMessageManager(WindowsMessageManager&&) noexcept = default;
		WindowsMessageManager& operator=(const WindowsMessageManager&) noexcept = default;
		WindowsMessageManager& operator=(WindowsMessageManager&&) noexcept = default;

		virtual LRESULT handle_message(
			const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam
		) override;
	};
}
