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

	getInputManager()->registerInputHandler(
		[&]() { getInputManager()->toggleRelativeMouse(); },
		InputType(InputDevice::MOUSE, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::ONE),
		InputType(InputDevice::MOUSE, InputDataType::BUTTON_RELEASED, InputContext::NO_CONTEXT, VirtualKeyCode::ONE)
	);

	constexpr float walk_speed = 0.0001f;
	//float walk_speed = 1.0f * getRenderer()->getTargetRenderInterval();

	getInputManager()->registerInputHandler(
		[&]() { getRenderer()->getCurrentCamera().walk(walk_speed); },
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::W),
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::W)
	);

	getInputManager()->registerInputHandler(
		[&]() { getRenderer()->getCurrentCamera().walk(-walk_speed); },
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::S),
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::S)
	);

	getInputManager()->registerInputHandler(
		[&]() { getRenderer()->getCurrentCamera().strafe(-walk_speed); },
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::A),
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::A)
	);

	getInputManager()->registerInputHandler(
		[&]() { getRenderer()->getCurrentCamera().strafe(walk_speed); },
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::D),
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::D)
	);

	getInputManager()->registerInputHandler(
		[&]() { getRenderer()->getCurrentCamera().fly(walk_speed); },
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::SPACE),
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::SPACE)
	);

	getInputManager()->registerInputHandler(
		[&]() { getRenderer()->getCurrentCamera().fly(-walk_speed); },
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::CONTROL),
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::CONTROL)
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