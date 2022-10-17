// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module InputManager;

export import KeyboardInputMessage;
export import MouseInputMessage;
export import ObjectPool;

export namespace mt { class Engine; }

export namespace mt::input
{
    class InputManager
    {
        using InputMessageVariant = std::variant<MouseInputMessage, KeyboardInputMessage>;

        ObjectPool<InputMessageVariant, 1024> _message_pool;

        std::queue<InputMessageVariant*> _input_queue;

        POINT _mouse_position;

        std::set<KeyboardKeys> _held_keys;
        std::set<MouseButtons> _held_buttons;

        mt::Engine& _engine;

    public:
        InputManager(mt::Engine& engine)
            : _engine(engine)
        {}

        ~InputManager() = default;

        InputManager(const InputManager &other) = delete;

        InputManager &operator=(const InputManager &other) = delete;

        void ProcessInput(); // friend engine, make protected?

        void _ProcessMouseInput(MouseInputMessage& mouse_input_message);

        void _ProcessKeyboardInput(KeyboardInputMessage& keyboard_input_message);

        void KeyboardEvent(KeyboardKeys key, KeyState key_state);

        void MouseEvent(__int32 x, __int32 y, bool button1, bool button2, bool button3, bool button4, bool button5);
    };
}