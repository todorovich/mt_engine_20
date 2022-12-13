// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessageManager;

export import WindowsMessage;

#pragma warning( push )
#pragma warning( disable : 5050 )
export import std.core;
#pragma warning( pop )

export namespace mt { class Engine; }

export namespace mt::windows
{
	class WindowsMessageManager
	{
		friend Engine;
		
		Engine& _engine;

	protected:
		void initialize();

	public:
		WindowsMessageManager(Engine& engine);
	
		~WindowsMessageManager();
	
		LRESULT handle_message(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
	
		std::map<int, WindowsMessage*> _message_handler_map;
	};
}