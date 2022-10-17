// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module InputMessage;

export import std.core;

export namespace mt::input
{
    class InputManager;

    class InputMessage
    {
    protected:

        void _MouseMove(__int32 x, __int32 y);

        void _MouseDown(WPARAM btnState);

        void _MouseUp(WPARAM btnState);

        void _KeyDown(WPARAM vk_key);

        void _KeyUp(WPARAM vk_key);

        InputManager* _input_manager;

        // TODO: Consider Variant instead?
        union {
            double d;
            float f[2];
            __int64 i64;
            __int32 i32[2];
            __int16 i16[4];
            __int8 i8[8];
        };

    public:
        InputMessage(InputManager* input_manager, double dbl)
            : _input_manager(input_manager)
            , d(dbl) 
        {}

        InputMessage(InputManager* input_manager, float f1, float f2)
            : _input_manager(input_manager)
            , f{f1, f2} {}

        InputMessage(InputManager* input_manager = nullptr, __int64 i = 0)
            : _input_manager(input_manager)
            , i64(i) {}

        InputMessage(InputManager* input_manager, __int32 i0, __int32 i1)
            : _input_manager(input_manager)
            , i32{i0, i1} {}

        InputMessage(InputManager* input_manager, __int16 i0, __int16 i1, __int16 i2, __int16 i3)
            : _input_manager(input_manager)
            , i16{i0, i1, i2, i3} {}

        InputMessage(
            InputManager* input_manager,
            __int8 i0, __int8 i1, __int8 i2, __int8 i3,
            __int8 i4, __int8 i5, __int8 i6, __int8 i7
        )
            : _input_manager(input_manager)
            , i8{i0, i1, i2, i3, i4, i5, i6, i7} {}

        InputMessage(const InputMessage &other)
            : _input_manager(other._input_manager)
            , i64(other.i64) {}

        InputMessage(InputMessage &&other)
            : _input_manager(other._input_manager)
            , i64(std::move(other.i64)) {}

        ~InputMessage() {
            i64 = 0;
        }

        InputMessage &operator=(const InputMessage &other) {
            i64 = other.i64;

            return *this;
        }

        InputMessage &operator=(InputMessage &&other) {
            i64 = std::move(other.i64);

            return *this;
        }

        virtual void Execute() {};
    };
}