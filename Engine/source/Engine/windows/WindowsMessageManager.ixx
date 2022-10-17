// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessageManager;

export import WindowsMessage;

export import std.core;

export namespace mt { class Engine; }

export namespace mt::windows
{
	class WindowsMessageManager
	{
		Engine& _engine;

		friend Engine;

	protected:
		void Initialize();

	public:
		WindowsMessageManager(Engine& engine);
	
		~WindowsMessageManager();
	
		LRESULT handle_message(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
	
		std::map<int, WindowsMessage*> _message_handler_map;
	};
}