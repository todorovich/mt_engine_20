// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessageManager;

import <map>;
import <memory>;

export import Engine;
export import WindowsMessageManagerInterface;
export import WindowsMessage;

using namespace mt::error;

export namespace mt::windows
{
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
