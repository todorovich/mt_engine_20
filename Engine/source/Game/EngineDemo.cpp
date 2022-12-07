module;
#include <DirectXMath.h>
#include <Windows.h>
// Fuck macros. This wasted like 30 minutes of my time.
#undef RELATIVE

module EngineDemo;

import Engine;
import InputManager;
import WindowManager;
import DirectXRenderer;

using namespace mt::input;

void mt::EngineDemo::map_input_controls()
{
	auto quit = [&]() { PostMessage(getWindowManager()->getMainWindowHandle(), WM_CLOSE, 0, 0); };
	getInputManager()->registerInputHandler(
		// TODO: make this something I can call on the engine proper.
		quit,
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::ESCAPE)
	);

	auto toggle_relative = [&]() { getInputManager()->toggleRelativeMouse(); };
	getInputManager()->registerInputHandler(
		toggle_relative, 
		InputType(InputDevice::MOUSE, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::ONE),
		InputType(InputDevice::MOUSE, InputDataType::BUTTON_RELEASED, InputContext::NO_CONTEXT, VirtualKeyCode::ONE)
	);

	constexpr float walk_speed = 0.0001f;
	//float walk_speed = 1.0f * getRenderer()->getTargetRenderInterval();

	auto walk_forward = [&]() { getRenderer()->getCurrentCamera().walk(walk_speed); };
	getInputManager()->registerInputHandler(
		walk_forward,
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::W),
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::W)
	);

	auto walk_backward = [&]() { getRenderer()->getCurrentCamera().walk(-walk_speed); };
	getInputManager()->registerInputHandler(
		walk_backward,
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::S),
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::S)
	);

	auto strafe_left = [&]() { getRenderer()->getCurrentCamera().strafe(-walk_speed); };
	getInputManager()->registerInputHandler(
		strafe_left,
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::A),
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::A)
	);

	auto strafe_right = [&]() { getRenderer()->getCurrentCamera().strafe(walk_speed); };
	getInputManager()->registerInputHandler(
		strafe_right,
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::D),
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::D)
	);

	auto mouse_look = [&](int x, int y) {
		auto& camera = getRenderer()->getCurrentCamera();

		// TODO: This should be fov dependant.
		// Make each pixel correspond to 1/10th of a degree.
		float dx = DirectX::XMConvertToRadians(0.1f * static_cast<float>(x));
		float dy = DirectX::XMConvertToRadians(0.1f * static_cast<float>(y));

		camera.pitch(dy);
		camera.rotateY(dx);
	};

	getInputManager()->registerInputHandler(
		mouse_look,
		InputType(InputDevice::MOUSE, InputDataType::TWO_DIMENSIONAL, InputContext::RELATIVE, VirtualKeyCode::NO_KEY)
	);
}