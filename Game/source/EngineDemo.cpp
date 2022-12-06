
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
	getInputManager()->registerInputHandler(
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::ESCAPE),
		// TODO: make this something I can call on the engine proper.
		[&]() { PostMessage(getWindowManager()->getMainWindowHandle(), WM_CLOSE, 0, 0); }
	);

	getInputManager()->registerInputHandler(
		InputType(InputDevice::MOUSE, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::ONE),
		[&]() { getInputManager()->toggleRelativeMouse(); }
	);

	getInputManager()->registerInputHandler(
		InputType(InputDevice::MOUSE, InputDataType::BUTTON_RELEASED, InputContext::NO_CONTEXT, VirtualKeyCode::ONE),
		[&]() { getInputManager()->toggleRelativeMouse(); }
	);

	auto walk_speed = 0.0001;// *getRenderer()->getTargetRenderInterval().;

	auto walk_forward = [&]() { getRenderer()->getCurrentCamera().walk(walk_speed); };
	getInputManager()->registerInputHandler(
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::W),
		walk_forward
	);
	getInputManager()->registerInputHandler(
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::W),
		walk_forward
	);

	auto walk_backward = [&]() { getRenderer()->getCurrentCamera().walk(-walk_speed); };
	getInputManager()->registerInputHandler(
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::S),
		walk_backward
	);
	getInputManager()->registerInputHandler(
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::S),
		walk_backward
	);

	auto strafe_left = [&]() { getRenderer()->getCurrentCamera().strafe(-walk_speed); };
	getInputManager()->registerInputHandler(
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::A ),
		strafe_left
	);
	getInputManager()->registerInputHandler(
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::A),
		strafe_left
	);

	auto strafe_right = [&]() { getRenderer()->getCurrentCamera().strafe(walk_speed); };
	getInputManager()->registerInputHandler(
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::D),
		strafe_right
	);
	getInputManager()->registerInputHandler(
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::D),
		strafe_right
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
		InputType(InputDevice::MOUSE, InputDataType::TWO_DIMENSIONAL, InputContext::RELATIVE, VirtualKeyCode::NO_KEY),
		mouse_look
	);
}