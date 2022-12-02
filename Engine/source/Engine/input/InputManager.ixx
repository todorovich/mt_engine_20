// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module InputManager;

export import std.core;

export import ObjectPool;
export import Time;

export import InputModel;

export namespace mt { class Engine; }

// Todo: Relative Mouse Position and locking mouse.
export namespace mt::input
{
    class InputManager
    {
        ObjectPool<InputMessage, 1024> _message_pool;

        std::queue<InputMessage*> _input_queue;

        std::map<InputType, std::function<void()>>              button_input_handler;
        std::map<InputType, std::function<void(int)>>           one_dimensional_input_handler;
        std::map<InputType, std::function<void(int, int)>>      two_dimensional_input_handler;
        std::map<InputType, std::function<void(int, int, int)>> three_dimensional_input_handler;

        //? I want this right?
        std::set<InputType> _held_buttons;

        mt::Engine& _engine;

        bool isMouseRelative = false;

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
            std::variant<std::function<void()>, std::function<void(int)>, std::function<void(int, int)>, std::function<void(int, int, int)>>;

        void registerInputHandler(InputType input_type, InputHandler input_handler);

        void toggleRelativeMouse()
        {
            if (isMouseRelative)
            {
                isMouseRelative = false;
                ShowCursor(true);
            }
            else
            {
                isMouseRelative = true;
                ShowCursor(false);
            }
        }
    };
}