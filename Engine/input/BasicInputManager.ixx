// Copyright 2023 Micho Todorovich, all rights reserved.
export module BasicInputManager;

import std;
import Engine;
import ObjectPool;
import Windows;

using namespace windows;
using namespace gsl;
using namespace mt::error;
using namespace mt::input::model;
using namespace mt::task;

export namespace mt::input
{
	class BasicInputManager : public InputManagerInterface
	{
		static const std::size_t POOL_SIZE = 2048;

		mt::memory::ObjectPool<InputMessage, POOL_SIZE> _message_pool;

		std::queue<mt::memory::ObjectPool<InputMessage, POOL_SIZE>::unique_ptr_t> _input_queue;

		std::multimap<InputType, not_null<Task*>>              				button_input_handler;
		std::multimap<InputType, not_null<OneDimensionalInputTask*>>    	one_dimensional_input_handler;
		std::multimap<InputType, not_null<TwoDimensionalInputTask*>>    	two_dimensional_input_handler;
		std::multimap<InputType, not_null<ThreeDimensionalInputTask*>>  	three_dimensional_input_handler;

		// Windows will only send the last key pressed as being held, so if you press A, B, C and hold them all down,
		// you will only get held messages for C. The engine should be propagating held messages each frame for A,B and C though.
		std::set<InputType> _held_buttons{};

		POINT _mouse_return_position{};

		std::atomic<bool> is_accepting_input = true;
	protected:
		mt::Engine& _engine;

		virtual void processInput() noexcept override;

	public:
		BasicInputManager(mt::Engine& engine, std::error_condition& error) noexcept
			: _engine(engine)
			, _message_pool(mt::memory::ObjectPool<InputMessage, POOL_SIZE>(error))
		{

		};

		virtual ~BasicInputManager() noexcept = default;

		BasicInputManager(const BasicInputManager &other) noexcept = delete;

		BasicInputManager(BasicInputManager&& other) noexcept = default;

		BasicInputManager& operator=(const BasicInputManager &other) noexcept = delete;

		BasicInputManager& operator=(BasicInputManager&& other) noexcept = default;

		virtual bool isAcceptingInput() const noexcept
		{
			return is_accepting_input;
		}

		virtual void acceptInput(
			InputType input_type,
			std::variant<std::monostate, InputData1D, InputData2D, InputData3D> data = std::monostate()
		) noexcept override;

		virtual void toggleIsMouseRelative() noexcept override;

		virtual void registerInputHandler(InputHandler input_handler, InputType input_types) noexcept override;
	};
}

