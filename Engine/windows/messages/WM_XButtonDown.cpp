module;

#include <windows.h>
#include <windowsx.h>

module WindowsMessages.XButtonDown;

import Engine;

LRESULT mt::windows::WM_XButtonDown::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	bool button_1 = 0x00010000 & wParam;
	bool button_2 = 0x00020000 & wParam;

	if (button_1)
	{
		_engine->getInputManager()->acceptInput(
			mt::input::InputType(
				mt::input::InputDevice::MOUSE, mt::input::InputDataType::BUTTON_PRESSED, mt::input::InputContext::NO_CONTEXT, mt::input::VirtualKeyCode::FOUR
			)
		);

		return true; // return true if processed.
	}
	else if (button_2)
	{
		_engine->getInputManager()->acceptInput(
			mt::input::InputType(
				mt::input::InputDevice::MOUSE, mt::input::InputDataType::BUTTON_PRESSED, mt::input::InputContext::NO_CONTEXT, mt::input::VirtualKeyCode::FIVE
			)
		);

		return true; // return true if processed.
	}
	else
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}