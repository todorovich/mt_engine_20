// Copyright 2023 Micho Todorovich, all rights reserved.
export module InputManagerInterface;

import std;

export import InputModel;

using namespace mt::input::model;

namespace mt::input::test
{
	class InputManagerTest;
}

export namespace mt::input
{
    class InputManagerInterface
    {
        bool _isMouseRelative = false;

	protected:
		void _setIsMouseRelative(bool isMouseRelative = true)
		{
			_isMouseRelative = isMouseRelative;
		}

    public:
        friend mt::input::test::InputManagerTest;

        InputManagerInterface() noexcept = default;

        virtual ~InputManagerInterface() noexcept = default;

        InputManagerInterface(const InputManagerInterface &other) noexcept = delete;

        InputManagerInterface(InputManagerInterface&& other) noexcept = default;

        InputManagerInterface& operator=(const InputManagerInterface &other) noexcept = delete;

        InputManagerInterface& operator=(InputManagerInterface&& other) noexcept = default;

		bool getIsMouseRelative() const { return _isMouseRelative; }

        // Accessor to check if the mouse is relative
        bool isMouseRelative() const noexcept {
            return _isMouseRelative;
        }

		virtual bool isAcceptingInput() const noexcept = 0;

		virtual void processInput() noexcept = 0;

        [[no_discard]] virtual std::expected<void, std::error_condition> acceptInput(
            InputType input_type,
			std::variant<std::monostate, InputData1D, InputData2D, InputData3D> data = std::monostate()
        ) noexcept = 0;

		virtual void toggleIsMouseRelative() noexcept  = 0;

        virtual void registerInputHandler(InputHandler input_handler, InputType input_types) noexcept = 0;

		// Templated functions for processing varargs of input types, so that more than one trigger may be associated
		// with each InputHandler
        template <typename First>
        void registerInputHandler(InputHandler&& input_handler, First&& first) noexcept
        {
            registerInputHandler(std::forward<InputHandler>(input_handler), std::forward<First>(first));
        }

        template <typename First, typename... Rest> 
        void registerInputHandler(InputHandler&& input_handler, First&& first, Rest&&... rest) noexcept
        {
            registerInputHandler(std::forward<InputHandler>(input_handler), std::forward<First>(first));
            registerInputHandler(
				std::forward<InputHandler>(input_handler),
				std::forward<Rest>(rest)...
			); // recursive call using pack expansion syntax
        }

        virtual void unregisterInputHandler(InputHandler input_handler, InputType input_types) noexcept = 0;

        virtual std::size_t getInputQueueSize() const noexcept = 0;
        virtual std::size_t getHandlerMapSize() const noexcept = 0;
    };
}