// Copyright 2023 Micho Todorovich, all rights reserved.
export module BasicInputManager;

import std;
import Engine;
import ObjectPool;
import Windows;
import InputModel;
import gsl;
import InputManagerInterface;

using namespace windows;
using namespace gsl;
using namespace mt::error;
using namespace mt::input::model;
using namespace mt::task;

namespace mt::input::test
{
	class InputTestEngine;
}

export namespace mt::input
{
	class BasicInputManager : public InputManagerInterface
	{
	public:
		static constexpr std::size_t POOL_SIZE = 2048;

	protected:
		mt::Engine& _engine;

	private:
		mt::memory::ObjectPool<InputMessage, POOL_SIZE> _message_pool;

		std::queue<decltype(_message_pool)::unique_ptr_t> _input_queue;

		std::multimap<InputType, not_null<Task*>>              				button_input_handler;
		std::multimap<InputType, not_null<OneDimensionalInputTask*>>    	one_dimensional_input_handler;
		std::multimap<InputType, not_null<TwoDimensionalInputTask*>>    	two_dimensional_input_handler;
		std::multimap<InputType, not_null<ThreeDimensionalInputTask*>>  	three_dimensional_input_handler;

		// Windows will only send the last key pressed as being held, so if you press A, B, C and hold them all down,
		// you will only get held messages for C. The engine should be propagating held messages each frame for A,B and C though.
		std::set<InputType> _held_buttons{};

		POINT _mouse_return_position{};

		std::atomic<bool> is_accepting_input = true;

	public:
		virtual void processInput() noexcept override;

		friend class mt::input::test::InputTestEngine;

		BasicInputManager(mt::Engine& engine, std::error_condition& error) noexcept
			: _engine(engine)
			, _message_pool(mt::memory::ObjectPool<InputMessage, POOL_SIZE>(engine, error))
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

		virtual std::expected<void, std::error_condition> acceptInput(
			InputType input_type,
			std::variant<std::monostate, InputData1D, InputData2D, InputData3D> data = std::monostate()
		) noexcept override;

		virtual void toggleIsMouseRelative() noexcept override;

		virtual void registerInputHandler(InputHandler input_handler, InputType input_types) noexcept override;

		virtual void unregisterInputHandler(InputHandler input_handler, InputType input_types) noexcept override;

		std::size_t getInputQueueSize() const noexcept {
			return _input_queue.size();
		}

		std::size_t getHandlerMapSize() const noexcept {
			return button_input_handler.size() +
				one_dimensional_input_handler.size() +
				two_dimensional_input_handler.size() +
				three_dimensional_input_handler.size();
		}
	};
}

