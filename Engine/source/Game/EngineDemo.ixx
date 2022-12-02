module;

#include <windows.h>
#include <crtdbg.h>
#include <DirectXMath.h>

// Fuck macros. This wasted like 30 minutes of my time.
#undef RELATIVE

export module EngineDemo;

import Engine;
import DirectXUtility;

import InputManager;
import WindowManager;
import DirectXRenderer;


export namespace mt
{
	export class EngineDemo : public Engine
	{
	public:
		EngineDemo() = default;
		~EngineDemo() = default;

		virtual bool initialize(HINSTANCE hInstance) override;

	private:
		virtual void _update() override;
		virtual void _draw() override;

	};

	bool EngineDemo::initialize(HINSTANCE hInstance)
	{
		if (!Engine::initialize(hInstance))
			return false;

		getInputManager()->registerInputHandler(
			mt::input::InputType(
				mt::input::InputDevice::KEYBOARD,
				mt::input::InputDataType::BUTTON_PRESSED,
				mt::input::InputContext::NO_CONTEXT,
				mt::input::VirtualKeyCode::ESCAPE
			),
			// TODO: make this something I can call on the engine proper.
			[&]() { PostMessage(getWindowManager()->getMainWindowHandle(), WM_CLOSE, 0, 0); }
		);

		getInputManager()->registerInputHandler(
			mt::input::InputType(
				mt::input::InputDevice::MOUSE,
				mt::input::InputDataType::BUTTON_PRESSED,
				mt::input::InputContext::NO_CONTEXT,
				mt::input::VirtualKeyCode::ONE
			),
			[&]() { getInputManager()->toggleRelativeMouse(); }
		);

		getInputManager()->registerInputHandler(
			mt::input::InputType(
				mt::input::InputDevice::MOUSE,
				mt::input::InputDataType::BUTTON_RELEASED,
				mt::input::InputContext::NO_CONTEXT,
				mt::input::VirtualKeyCode::ONE
			),
			[&]() { getInputManager()->toggleRelativeMouse(); }
		);

		getInputManager()->registerInputHandler(
			mt::input::InputType(
				mt::input::InputDevice::MOUSE,
				mt::input::InputDataType::TWO_DIMENSIONAL,
				mt::input::InputContext::RELATIVE,
				mt::input::VirtualKeyCode::NO_KEY
			),
			[&](int x, int y) {
				auto& camera = getRenderer()->getCurrentCamera();

				// TODO: This should be fov dependant.
				// Make each pixel correspond to 1/10th of a degree.
				float dx = DirectX::XMConvertToRadians(0.1f * static_cast<float>(x));
				float dy = DirectX::XMConvertToRadians(0.1f * static_cast<float>(y));

				camera.pitch(dy);
				camera.rotateY(dx);
			}
		);

		return true;
	}

	void EngineDemo::_update() {}

	void EngineDemo::_draw() {}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	try
	{
		mt::EngineDemo engine_demo = mt::EngineDemo();

		engine_demo.initialize(hInstance);

		engine_demo.run();

		engine_demo.destroy();

		return 0;
	}
	catch (mt::renderer::DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}