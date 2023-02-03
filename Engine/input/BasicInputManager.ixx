// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

// Copyright 2023 Micho Todorovich, all rights reserved.
export module BasicInputManager;

export import InputManagerInterface;

export import gsl;
export import Engine;
export import Task;

import ObjectPool;

using namespace gsl;
using namespace mt::task;
using namespace mt::input::model;

export namespace mt::input
{
	class BasicInputManager : public InputManagerInterface
	{
		// TODO this can be overflowed. There should be a way to check how many windows messages there are, and to
		// 	only fetch at most this many before running the frame (and processing the input)
		mt::memory::ObjectPool<InputMessage, 2048> _message_pool;

		std::queue<not_null<InputMessage*>> _input_queue;

		std::multimap<InputType, not_null<Task*>>              				button_input_handler;
		std::multimap<InputType, not_null<OneDimensionalInputTask*>>    	one_dimensional_input_handler;
		std::multimap<InputType, not_null<TwoDimensionalInputTask*>>    	two_dimensional_input_handler;
		std::multimap<InputType, not_null<ThreeDimensionalInputTask*>>  	three_dimensional_input_handler;

		// Windows will only send the last key pressed as being held, so if you press A, B, C and hold them all down,
		// you will only get held messages for C. The engine should be propagating held messages each frame for A,B and C though.
		std::set<InputType> _held_buttons;

		POINT _mouse_return_position;

	protected:
		mt::Engine& _engine;

		virtual void processInput() noexcept override;

	public:
		BasicInputManager(mt::Engine& engine) noexcept
			: _engine(engine)
		{};

		virtual ~BasicInputManager() noexcept = default;

		BasicInputManager(const BasicInputManager &other) noexcept = delete;

		BasicInputManager(BasicInputManager&& other) noexcept = default;

		BasicInputManager& operator=(const BasicInputManager &other) noexcept = delete;

		BasicInputManager& operator=(BasicInputManager&& other) noexcept = default;

		virtual void acceptInput(
			InputType input_type,
			std::variant<std::monostate, InputData1D, InputData2D, InputData3D> data = std::monostate()
		) noexcept override;

		virtual void toggleRelativeMouse() noexcept override;

		virtual void registerInputHandler(InputHandler input_handler, InputType input_types) noexcept override;
	};
}

