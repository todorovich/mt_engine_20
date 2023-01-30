module;
#include <DirectXMath.h>
#include <Windows.h>
// Fuck macros. This wasted like 30 minutes of my time.
#undef RELATIVE

module EngineDemo;

import <chrono>;

import Engine;
import WindowManager;

using namespace mt::input;

using namespace std::literals;

using mt::input::InputDevice::KEYBOARD;
using mt::input::InputDevice::MOUSE;

using mt::input::InputDataType::BUTTON_PRESSED;
using mt::input::InputDataType::BUTTON_RELEASED;
using mt::input::InputDataType::BUTTON_HELD;
using mt::input::InputDataType::TWO_DIMENSIONAL;

using mt::input::InputContext::NO_CONTEXT;
using mt::input::InputContext::RELATIVE;

using mt::input::VirtualKeyCode::ESCAPE;

void mt::EngineDemo::map_input_controls() noexcept
{
	// TODO: make this something I can call on the engine proper.
	auto quit = [](mt::Engine& engine) noexcept {
		PostMessage(engine.getWindowManager()->getMainWindowHandle(), WM_CLOSE, 0, 0);
	};
	_engine.getInputManager()->registerInputHandler(quit, { KEYBOARD, BUTTON_PRESSED, NO_CONTEXT, ESCAPE });

	auto toggle_relative_mouse = [](mt::Engine& engine) noexcept {
		engine.getInputManager()->toggleRelativeMouse();
	};
	_engine.getInputManager()->registerInputHandler(
		toggle_relative_mouse,
		InputType(MOUSE, BUTTON_PRESSED, NO_CONTEXT, VirtualKeyCode::ONE),
		InputType(MOUSE, BUTTON_RELEASED, NO_CONTEXT, VirtualKeyCode::ONE)
	);

	constexpr float walk_speed =1.0f;
	//constexpr float walk_speed = 1.0f * 1s / getRenderer()->getRenderInterval();

	auto walk_forward = [](mt::Engine& engine) noexcept {
		auto adjusted_walk_speed = walk_speed / (1s / engine.getTimeManager()->getRenderInterval());
		engine.getRenderer()->getCurrentCamera().walk(adjusted_walk_speed);
	};
	_engine.getInputManager()->registerInputHandler(
		walk_forward,
		InputType(KEYBOARD, BUTTON_PRESSED, NO_CONTEXT, VirtualKeyCode::W),
		InputType(KEYBOARD, BUTTON_HELD, 	NO_CONTEXT, VirtualKeyCode::W)
	);

	auto walk_backward = [](mt::Engine& engine) noexcept {
		auto adjusted_walk_speed = -walk_speed / (1s / engine.getTimeManager()->getRenderInterval());
		engine.getRenderer()->getCurrentCamera().walk(adjusted_walk_speed);
	};
	_engine.getInputManager()->registerInputHandler(
		walk_backward,
		InputType(KEYBOARD, BUTTON_PRESSED, NO_CONTEXT, VirtualKeyCode::S),
		InputType(KEYBOARD, BUTTON_HELD, 	NO_CONTEXT, VirtualKeyCode::S)
	);

	auto strafe_left = [](mt::Engine& engine) noexcept {
		auto adjusted_walk_speed = -walk_speed / (1s / engine.getTimeManager()->getRenderInterval());
		engine.getRenderer()->getCurrentCamera().strafe(adjusted_walk_speed);
	};
	_engine.getInputManager()->registerInputHandler(
		strafe_left,
		InputType(KEYBOARD, BUTTON_PRESSED, NO_CONTEXT, VirtualKeyCode::A),
		InputType(KEYBOARD, BUTTON_HELD, 	NO_CONTEXT, VirtualKeyCode::A)
	);

	auto strafe_right = [](mt::Engine& engine)noexcept {
		auto adjusted_walk_speed = walk_speed / (1s / engine.getTimeManager()->getRenderInterval());
		engine.getRenderer()->getCurrentCamera().strafe(adjusted_walk_speed);
	};
	_engine.getInputManager()->registerInputHandler(
		strafe_right,
		InputType(KEYBOARD, BUTTON_PRESSED, NO_CONTEXT, VirtualKeyCode::D),
		InputType(KEYBOARD, BUTTON_HELD, 	NO_CONTEXT, VirtualKeyCode::D)
	);

	auto fly_up = [](mt::Engine& engine) noexcept {
		auto adjusted_walk_speed = walk_speed / (1s / engine.getTimeManager()->getRenderInterval());
		engine.getRenderer()->getCurrentCamera().fly(adjusted_walk_speed);
	};
	_engine.getInputManager()->registerInputHandler(
		fly_up,
		InputType(KEYBOARD, BUTTON_PRESSED, NO_CONTEXT, VirtualKeyCode::SPACE),
		InputType(KEYBOARD, BUTTON_HELD, 	NO_CONTEXT, VirtualKeyCode::SPACE)
	);

	auto fly_down = [](mt::Engine& engine) noexcept {
		auto adjusted_walk_speed = -walk_speed / (1s / engine.getTimeManager()->getRenderInterval());
		engine.getRenderer()->getCurrentCamera().fly(adjusted_walk_speed);
	};
	_engine.getInputManager()->registerInputHandler(
		fly_down,
		InputType(KEYBOARD, BUTTON_PRESSED, NO_CONTEXT, VirtualKeyCode::CONTROL),
		InputType(KEYBOARD, BUTTON_HELD, 	NO_CONTEXT, VirtualKeyCode::CONTROL)
	);

	auto mouse_look = [](mt::Engine& engine, int x, int y) noexcept {
		auto& camera = engine.getRenderer()->getCurrentCamera();

		// TODO: This should be fov dependant.
		// Make each pixel correspond to 1/100th of a degree.
		float dx = DirectX::XMConvertToRadians(0.01f * static_cast<float>(x));
		float dy = DirectX::XMConvertToRadians(0.01f * static_cast<float>(y));

		camera.pitch(dy);
		camera.rotateY(dx);
	};
	_engine.getInputManager()->registerInputHandler(
		mouse_look,
		InputType(MOUSE, TWO_DIMENSIONAL, RELATIVE, VirtualKeyCode::NO_KEY)
	);
}