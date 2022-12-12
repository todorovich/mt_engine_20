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
import TimeManager;

using namespace mt::input;

using namespace std::literals::chrono_literals;

void mt::EngineDemo::map_input_controls()
{
	// TODO: make this something I can call on the engine proper.
	auto quit = [](mt::Engine& engine) { 
		PostMessage(engine.getWindowManager()->getMainWindowHandle(), WM_CLOSE, 0, 0);
	};
	getInputManager()->registerInputHandler(
		quit,
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::ESCAPE)
	);

	auto toggle_relative_mouse = [](mt::Engine& engine) {
		engine.getInputManager()->toggleRelativeMouse();
	};
	getInputManager()->registerInputHandler(
		toggle_relative_mouse,
		InputType(InputDevice::MOUSE, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::ONE),
		InputType(InputDevice::MOUSE, InputDataType::BUTTON_RELEASED, InputContext::NO_CONTEXT, VirtualKeyCode::ONE)
	);

	constexpr float walk_speed =1.0f;
	//constexpr float walk_speed = 1.0f * 1s / getRenderer()->getTargetRenderInterval();

	auto walk_forward = [](mt::Engine& engine) {
		auto adjusted_walk_speed = walk_speed / (1s / engine.getTimeManager()->getTargetRenderInterval());
		engine.getRenderer()->getCurrentCamera().walk(adjusted_walk_speed);
	};
	getInputManager()->registerInputHandler(
		walk_forward,
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::W),
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::W)
	);

	auto walk_backward = [](mt::Engine& engine) {
		auto adjusted_walk_speed = -walk_speed / (1s / engine.getTimeManager()->getTargetRenderInterval());
		engine.getRenderer()->getCurrentCamera().walk(adjusted_walk_speed);
	};
	getInputManager()->registerInputHandler(
		walk_backward,
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::S),
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::S)
	);

	auto strafe_left = [](mt::Engine& engine) {
		auto adjusted_walk_speed = -walk_speed / (1s / engine.getTimeManager()->getTargetRenderInterval());
		engine.getRenderer()->getCurrentCamera().strafe(adjusted_walk_speed);
	};
	getInputManager()->registerInputHandler(
		strafe_left,
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::A),
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::A)
	);

	auto strafe_right = [](mt::Engine& engine) {
		auto adjusted_walk_speed = walk_speed / (1s / engine.getTimeManager()->getTargetRenderInterval());
		engine.getRenderer()->getCurrentCamera().strafe(adjusted_walk_speed);
	};
	getInputManager()->registerInputHandler(
		strafe_right,
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::D),
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::D)
	);

	auto fly_up = [](mt::Engine& engine) {
		auto adjusted_walk_speed = walk_speed / (1s / engine.getTimeManager()->getTargetRenderInterval());
		engine.getRenderer()->getCurrentCamera().fly(adjusted_walk_speed);
	};
	getInputManager()->registerInputHandler(
		fly_up,
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::SPACE),
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::SPACE)
	);

	auto fly_down = [](mt::Engine& engine) {
		auto adjusted_walk_speed = -walk_speed / (1s / engine.getTimeManager()->getTargetRenderInterval());
		engine.getRenderer()->getCurrentCamera().fly(adjusted_walk_speed);
	};
	getInputManager()->registerInputHandler(
		fly_down,
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::CONTROL),
		InputType(InputDevice::KEYBOARD, InputDataType::BUTTON_HELD, InputContext::NO_CONTEXT, VirtualKeyCode::CONTROL)
	);

	auto mouse_look = [](mt::Engine& engine, int x, int y) {
		auto& camera = engine.getRenderer()->getCurrentCamera();

		// TODO: This should be fov dependant.
		// Make each pixel correspond to 1/100th of a degree.
		float dx = DirectX::XMConvertToRadians(0.01f * static_cast<float>(x));
		float dy = DirectX::XMConvertToRadians(0.01f * static_cast<float>(y));

		camera.pitch(dy);
		camera.rotateY(dx);
	};
	getInputManager()->registerInputHandler(
		mouse_look,
		InputType(InputDevice::MOUSE, InputDataType::TWO_DIMENSIONAL, InputContext::RELATIVE, VirtualKeyCode::NO_KEY)
	);
}