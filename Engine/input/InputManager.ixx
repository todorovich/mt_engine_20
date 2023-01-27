// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module InputManager;

export import <map>;
export import <set>;
export import <variant>;

export import InputModel;
export import ObjectPool;

// Todo: Relative Mouse Position and locking mouse.
export namespace mt::input
{
    class InputManager
    {
		// TODO this can be overflowed. There should be a way to check how many windows messages there are, and to
		// 	only fetch at most this many before running the frame (and processing the input)
        ObjectPool<InputMessage, 2048> _message_pool;

        std::queue<InputMessage*> _input_queue;

        std::multimap<InputType, button_function*>              button_input_handler;
        std::multimap<InputType, one_dimensional_function*>     one_dimensional_input_handler;
        std::multimap<InputType, two_dimensional_function*>     two_dimensional_input_handler;
        std::multimap<InputType, three_dimensional_function*>   three_dimensional_input_handler;

        // Windows will only send the last key pressed as being held, so if you press A, B, C and hold them all down,
        // you will only get held messages for C. The engine should be propagating held messages each frame for A,B and C though.
        std::set<InputType> _held_buttons;

        mt::Engine& _engine;

        bool isMouseRelative = false;

        POINT _mouse_return_position;

    public:
        InputManager(mt::Engine& engine) noexcept
            : _engine(engine)
        {}

        ~InputManager() noexcept = default;

        InputManager(const InputManager &other) noexcept = delete;
        
        InputManager(InputManager&& other) noexcept = default;

        InputManager& operator=(const InputManager &other) noexcept = delete;

        InputManager& operator=(InputManager&& other) noexcept = default;

        void processInput() noexcept; // friend engine, make protected?

        void acceptInput(
            InputType input_type,
			std::variant<std::monostate, InputData1D, InputData2D, InputData3D> data = std::monostate()
        ) noexcept;

        using InputHandler = 
            std::variant<button_function*, one_dimensional_function*, two_dimensional_function*, three_dimensional_function*>;
        
        void registerInputHandler(InputHandler input_handler, InputType input_types) noexcept;

        template <typename First>
        void registerInputHandler(InputHandler input_handler, const First& first) noexcept
        {
            registerInputHandler(input_handler, first);
        }

        template <typename First, typename... Rest> 
        void registerInputHandler(InputHandler input_handler, const First& first, const Rest&... rest) noexcept
        {
            registerInputHandler(input_handler, first);
            registerInputHandler(input_handler, rest...); // recursive call using pack expansion syntax
        }

        void toggleRelativeMouse() noexcept;
    };
}