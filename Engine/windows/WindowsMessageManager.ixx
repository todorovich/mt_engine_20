// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessageManager;

import <map>;
import <memory>;

export import WindowsMessage;

export namespace mt { class Engine; }

export namespace mt::windows
{
	class WindowsMessageManager
	{
		friend Engine;
		
		Engine* _engine;

		std::map<int, std::unique_ptr<WindowsMessage>> _message_handler_map;
	public:

		WindowsMessageManager(Engine* engine);
		~WindowsMessageManager() = default;
		WindowsMessageManager(const WindowsMessageManager&) noexcept = default;
		WindowsMessageManager(WindowsMessageManager&&) noexcept = default;
		WindowsMessageManager& operator=(const WindowsMessageManager&) noexcept = default;
		WindowsMessageManager& operator=(WindowsMessageManager&&) noexcept = default;


		LRESULT handle_message(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
	};
}