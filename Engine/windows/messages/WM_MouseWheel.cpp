// Copyright 2022 Micho Todorovich, all rights reserved.
module WindowsMessages.MouseWheel;

import Engine;
import InputModel;
import Windows;

using namespace windows;
using namespace mt::input::model;

LRESULT mt::windows::WM_MouseWheel::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
)
{
	// wParam is the WHEEL_DELTA
	_engine->getInputManager()->acceptInput(
		InputType(
			InputDevice::MOUSE, InputDataType::ONE_DIMENSIONAL, InputContext::NO_CONTEXT
		),
		InputData1D(static_cast<int>(wParam))
	);

	return 0;
}