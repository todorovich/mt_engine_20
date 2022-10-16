// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module InputManager;

export import InputMessage;
export import ObjectPool;

export namespace mt::input
{
    class InputManager
    {
        ObjectPool<InputMessage, 1024> _message_pool;

        std::queue<InputMessage *> _input_queue;

        POINT _mouse_position;

        std::set<WPARAM> _held_keys_and_buttons;

    public:
        InputManager() = default;

        ~InputManager() = default;

        InputManager(const InputManager &other) = delete;

        InputManager &operator=(const InputManager &other) = delete;

        void ProcessInput();

        void MouseMove(WPARAM btnState, int x, int y);

        void MouseDown(WPARAM btnState, int x, int y);

        void MouseUp(WPARAM btnState, int x, int y);

        void KeyDown(WPARAM vk_key, LPARAM flags);

        void KeyUp(WPARAM vk_key, LPARAM flags);

        friend InputMessage;

    protected:

        void _MouseMove(__int32 x, __int32 y);

        void _MouseDown(WPARAM btnState);

        void _MouseUp(WPARAM btnState);

        void _KeyDown(WPARAM vk_key);

        void _KeyUp(WPARAM vk_key);
    };
}