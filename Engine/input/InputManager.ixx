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
        ObjectPool<InputMessage, 1024> _message_pool;

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
        InputManager(mt::Engine& engine)
            : _engine(engine)
        {}

        ~InputManager() = default;

        InputManager(const InputManager &other) = delete;
        
        InputManager(InputManager&& other) = default;

        InputManager& operator=(const InputManager &other) = delete;

        InputManager& operator=(InputManager&& other) = default;

        void processInput(); // friend engine, make protected?

        void acceptInput(
            InputType input_type, std::variant<std::monostate, InputData1D, InputData2D, InputData3D> data = std::monostate()
        );

        using InputHandler = 
            std::variant<button_function*, one_dimensional_function*, two_dimensional_function*, three_dimensional_function*>;
        
        void registerInputHandler(InputHandler input_handler, InputType input_types);

        template <typename First>
        void registerInputHandler(InputHandler input_handler, const First& first)
        {
            registerInputHandler(input_handler, first);
        }

        template <typename First, typename... Rest> 
        void registerInputHandler(InputHandler input_handler, const First& first, const Rest&... rest)
        {
            registerInputHandler(input_handler, first);
            registerInputHandler(input_handler, rest...); // recursive call using pack expansion syntax
        }

        void toggleRelativeMouse();
    };
}