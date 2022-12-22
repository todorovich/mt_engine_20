// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessageManager;

export import WindowsMessage;

export namespace mt { class Engine; }

import <map>;

export namespace mt::windows
{
	class WindowsMessageManager
	{
		friend Engine;
		
		Engine& _engine;

	public:

		WindowsMessageManager(Engine& engine);
	
		~WindowsMessageManager();
	
		void initialize();

		LRESULT handle_message(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
	
		std::map<int, WindowsMessage*> _message_handler_map;
	};
}